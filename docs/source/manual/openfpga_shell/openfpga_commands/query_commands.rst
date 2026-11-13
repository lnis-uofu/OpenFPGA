.. _openfpga_query_commands:

Query Commands
--------------

The query commands answer questions about the design, so that a script, a program or a language model can look at the results of a stage before it decides what to do next. Each of them prints its answer as JSON and gives it to the program that ran the command, see :ref:`query_answers`. In Python, the answer is the ``data`` of the result:

.. code-block:: python

  shell.timing_summary().data["data"]["setup"]["fmax_mhz"]
  shell.critical_paths(count=3).data["data"]["paths"][0]["slack_ns"]

In Python, the ``Session`` class gives the answers as objects, see :ref:`openfpga_python_session`.

The queries read, and never change, the results of the stage that they need. The shell refuses a query when that stage has not been reached, or when an option or a stage that ran again made it out of date, and names the command to run first. Outside of the flow that is run stage by stage, e.g., after ``vpr``, the queries read the data that VPR holds. The schema of each answer is in ``openfpga_flow/schemas``. Times are in ns, frequencies in MHz, and the answers are in the same units as the reports of VPR: the numbers agree with its log and files.

Queries that list items stop at a limit (``--limit``, 1000 by default), and say so with ``truncated``, so that the answer cannot grow without bounds.

report_session
~~~~~~~~~~~~~~

  Answer with what the flow that is run stage by stage has done: kind ``session``, not read from a stage. It works at any time, and needs no stage.

  - ``reached``: the stages that are valid, in the order of the flow (``DEVICE``, ``PACKED``, ``PLACED``, ``ROUTED``, ``ANALYZED``, ...). A stage that an option or an input made out of date is not in it. The list is empty when the data of VPR that the session worked with was replaced, e.g., by ``vpr`` or by another shell
  - ``inputs``: ``vpr_arch`` (``file`` and ``layout``), ``openfpga_arch`` and ``circuit`` (``file``): where each was read from, as a whole path. ``null`` for an input that is not read
  - ``results``: ``packing``, ``placement`` and ``routing``: the file of the results of a stage that is reached, which is the one that VPR wrote, or the one that ``read_packing``, ``read_placement`` or ``read_routing`` read instead of running the stage. ``null`` for the other stages. The path is whole, from the current directory of the process

  .. code-block::

    report_session

timing_summary
~~~~~~~~~~~~~~

  Answer with the timing of the routed design: kind ``timing_summary``, read from the stage ``ROUTED``. The timing is analyzed again from the routing, as the analysis of VPR does, so the numbers are the ones of the analysis stage, and the analysis does not have to run first. The timing analysis must be on (``general.timing_analysis``).

  - ``setup``: ``worst_negative_slack_ns`` and ``total_negative_slack_ns`` (the sWNS and sTNS of VPR), ``critical_path_delay_ns`` (of the path with the least slack), ``fmax_mhz`` (its inverse) and the clocks of that path
  - ``hold``: ``worst_negative_slack_ns`` and ``total_negative_slack_ns``
  - ``clock_domains``: one entry for each pair of clocks that has a path: ``launch_clock``, ``capture_clock``, ``intra_domain``, ``critical_path_delay_ns``, ``worst_slack_ns`` and ``fmax_mhz`` (only for a path within one clock, ``null`` for others)

  .. code-block::

    timing_summary

critical_paths
~~~~~~~~~~~~~~

  Answer with the worst setup paths of the routed design: kind ``critical_paths``, read from the stage ``ROUTED``. The paths come worst first, as in the timing report of VPR. Each one has its ``startpoint``, ``endpoint``, clocks and ``slack_ns``, and the points of its data path in order: the ``pin``, its ``type``, the ``block`` and ``net`` of the netlist, the ``arrival_ns`` and the ``delay_ns`` since the point before. ``data_arrival_ns`` includes the delay of the clock to the block that launches the path, as in the report of VPR, and ``data_required_ns`` is what the capturing clock requires.

  .. option:: --count <int>

    Number of paths, from 1 to 100. Default: 5

  .. code-block::

    critical_paths --count 10

device
~~~~~~

  Answer with the device that the flow works on: kind ``device``, read from the stage ``DEVICE``, so it is there as soon as the three inputs are read.

  - ``grid``: ``width``, ``height``, ``layers`` and the name of the ``layout`` of the architecture
  - ``channel_width``: ``x``, ``y`` and ``fixed``. It is the widest channel of the routing resource graph that VPR made with the device: the width that was set with ``router.route_chan_width`` (``fixed`` is true), or else the width that VPR estimates the delays of the placement with (``place.place_chan_width``, 100 by default), when the router is free to find its own. ``null`` if there is no graph
  - ``tile_types``: for each type of tile that is on the grid, its ``name``, ``width`` and ``height`` in tiles, ``capacity`` (the blocks one tile holds), ``is_io``, the number of ``input_pins``, ``output_pins`` and ``clock_pins``, ``count`` (the tiles of this type) and ``slots`` (the places for blocks that they make, ``count`` times ``capacity``, as VPR counts them), and its ``sub_tiles`` with their ``capacity`` and the types of blocks that fit (``equivalent_blocks``)
  - ``io_sides``: the I/O tiles on the ``left``, ``right``, ``bottom`` and ``top`` of the perimeter. A corner is not counted
  - ``locations``: only when a type is asked for: ``type``, ``total``, ``returned``, ``truncated`` and ``tiles``, the lower left corner (``x``, ``y``, ``layer``) of each tile of that type

  .. option:: --type <string>

    Also list where the tiles of this type are, e.g., ``io`` or ``clb``. An unknown name is refused, with the names that exist

  .. option:: --limit <int>

    Most locations to list. Default: 1000

  .. code-block::

    device --type io --limit 20

utilization
~~~~~~~~~~~

  Answer with the use of the blocks of the device by the packed design: kind ``utilization``, read from the stage ``PACKED``. For each type of block, the ``used`` blocks of the netlist, the ``available`` places of the device (a tile holds as many blocks as its capacity, so a tile of inputs and outputs holds several, and VPR counts them the same way in the resource usage that it logs), and their ratio. ``device_utilization`` is the figure of VPR, and ``limiting_resources`` lists the block types that limit the size of the device.

  .. code-block::

    utilization

placement
~~~~~~~~~

  Answer with the place of the blocks of the placed design: kind ``placement``, read from the stage ``PLACED``. Each block has its ``name``, ``type``, ``x``, ``y``, ``sub_tile`` and ``layer``, as in the placement file. ``total`` is the number of blocks that match the filters.

  .. option:: --type <string>

    Only the blocks of this type, e.g., ``clb``

  .. option:: --prefix <string>

    Only the blocks whose name starts with this text, e.g., ``out:``, which is how to select a part of the design by the name of its blocks

  .. option:: --limit <int>

    Most blocks to list. Default: 1000

  .. code-block::

    placement --type io --prefix out: --limit 20

net_route
~~~~~~~~~

  Answer with the routing tree of a net: kind ``net_route``, read from the stage ``ROUTED``. The tree is a list of nodes of the routing resource graph, in the order of a depth-first walk, so a node comes after the node that drives it. Each node has its ``index``, its ``parent`` (the index of the node that drives it, ``null`` for the source), the ``rr_node`` number, its ``type`` (``SOURCE``, ``OPIN``, ``CHANX``, ``CHANY``, ``IPIN`` or ``SINK``), its coordinates (``xlow``, ``ylow``, ``xhigh``, ``yhigh``, ``layer``) and ``ptc``, and for a sink the ``sink_pin``, the index of the pin of the net it reaches. These are the nodes of the routing file, without the repeated start of each branch. A net that VPR does not route, e.g., an ideal clock, has no routing and is refused.

  .. option:: --net <string>

    Name of the net

  .. code-block::

    net_route --net n1

congestion
~~~~~~~~~~

  Answer with the use of the routing channels: kind ``congestion``, read from the stage ``ROUTED``. A channel segment is used by the wires of the nets that cross it. The answer gives the ``used`` wires, the ``capacity`` and the ``occupancy`` of the horizontal (``chanx``) and vertical (``chany``) channels, for the whole device in ``summary`` and for each region. A region is a square of tiles; only the regions that have a wire in use are listed. ``overused_nodes`` is the number of wires that more than one net uses, which is none in a legal routing. The wires in use add up to the wirelength of VPR, and the regions, with a size of one tile, to its channel occupancy files.

  .. option:: --region_size <int>

    Number of tiles on a side of a region. Default: 1

  .. code-block::

    congestion --region_size 2

report_cluster
~~~~~~~~~~~~~~

  Answer with the clusters of the packed design: kind ``cluster_report``, read from the stage ``PACKED``. Each cluster has its ``name``, ``type``, ``input_pin_utils`` and ``output_pin_utils``: the pins of the cluster over the pins of its type, as in the packing pin usage report of VPR (the clock pins are input pins).

  .. option:: --query <string>

    Only the clusters whose name matches this shell-style pattern (``*``, ``?`` and ``[...]``), e.g., ``n1*``. All the clusters if it is not set. ``matched`` is the number of clusters that match, and ``total`` the number of clusters

  .. option:: --stats

    Also answer with the statistics of all the clusters: the number of clusters, and for each type the count and the average input and output pin utilization

  .. option:: --limit <int>

    Most clusters to list. Default: 1000

  .. code-block::

    report_cluster --query "out:*" --stats

find
~~~~

  Answer with the blocks and the nets of the packed design whose name matches a pattern: kind ``find``, read from the stage ``PACKED``. ``blocks`` and ``nets`` each have ``total`` (all of them), ``matched``, ``returned``, ``truncated`` and ``items``. A block has its ``name``, ``type`` and, once the design is placed, its place (``placed``, ``x``, ``y``, ``sub_tile`` and ``layer``: ``null`` before that). A net has its ``name``, its ``fanout`` (the number of sinks) and ``ignored``, which is true for a net that VPR does not route, e.g., a clock. The part that was not looked for (see ``--kind``) is ``null``.

  .. option:: --pattern <string>

    Shell-style pattern of the name (``*``, ``?`` and ``[...]``), e.g., ``n1*``

  .. option:: --kind <string>

    Look for ``block`` or for ``net`` only. Default: both

  .. option:: --limit <int>

    Most blocks, and most nets, to list. Default: 1000

  .. code-block::

    find --pattern "out:*" --kind block
