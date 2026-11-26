.. _openfpga_query_commands:

Query Commands
--------------

The query commands answer questions about the design, so that a script, a program or a language model can look at the results of a stage before it decides what to do next. Each of them prints its answer as JSON and gives it to the program that ran the command, see :ref:`query_answers`. In Python, the answer is the ``data`` of the result:

.. code-block:: python

  shell.timing_summary().data["data"]["setup"]["fmax_mhz"]
  shell.critical_paths(count=3).data["data"]["paths"][0]["slack_ns"]

In Python, the ``Session`` class gives the answers as objects, see :ref:`openfpga_python_session`.

The queries read, and never change, the results of the stage that they need. The shell refuses a query when that stage has not been reached, or when an option or a stage that ran again made it out of date, and names the command to run first. Outside of the flow that is run stage by stage, e.g., after ``vpr``, the queries read the data that VPR holds. The schema of each answer is in ``openfpga_flow/schemas``. Times are in ns, frequencies in MHz, and the answers are in the same units as the reports of VPR: the numbers agree with its log and files.

The queries are commands of the plugin ``ui``, which a build has by default, see :ref:`openfpga_shell_plugins`.

Queries that list items stop at a limit (``--limit``, 1000 by default), and say so with ``truncated``, so that the answer cannot grow without bounds.

report_session
~~~~~~~~~~~~~~

  Answer with what the flow that is run stage by stage has done: kind ``session``, not read from a stage. It works at any time, and needs no stage.

  - ``reached``: the stages that are valid, in the order of the flow (``DEVICE``, ``PACKED``, ``PLACED``, ``ROUTED``, ``ANALYZED``, ...). A stage that an option or an input made out of date is not in it. The list is empty when the data of VPR that the session worked with was replaced, e.g., by ``vpr`` or by another shell
  - ``inputs``: ``vpr_arch`` (``file`` and ``layout``), ``openfpga_arch`` and ``circuit`` (``file``): where each was read from, as a whole path. ``null`` for an input that is not read
  - ``failed``: ``stage`` and ``message`` of the stage (``PACKED``, ``PLACED`` or ``ROUTED``) that failed last and left what it made, e.g., a route that found no legal routing; ``null`` otherwise, and once the stage runs again or is dropped. The queries of the stages before it still answer, and ``congestion`` reads the routing that failed
  - ``results``: ``packing``, ``placement`` and ``routing``: the file of the results of a stage that is reached, which is the one that VPR wrote, or the one that ``read_packing``, ``read_placement`` or ``read_routing`` read instead of running the stage. ``null`` for the other stages. The path is whole, from the current directory of the process

  .. code-block::

    report_session

report_stage
~~~~~~~~~~~~

  Answer with what can be changed for each stage of the flow: kind ``stage_info``, from the table of the application options. It is a bounded list, for the question "what can I change after ``place``?". It reads the session only, so it works at any time and needs no data of VPR.

  For each stage (``DEVICE``, ``PACKED``, ``PLACED``, ``ROUTED``, ``ANALYZED``, ``REPACKED``, ``BITSTREAM``):

  - ``requires``: the stage before it in the flow, and ``commands``: the commands that reach it
  - ``reached`` and ``failed`` (the message, if the stage failed and left its data): as in ``report_session``
  - ``dropped_by_change``: the stages that are dropped when an option of this stage changes, i.e., this one and the ones that depend on it. The earlier stages are kept
  - ``options``: ``total`` (the options of the stage), ``matched`` (by the filters), ``returned``, ``truncated`` and ``items``, which are the entries of ``report_app_option --json`` (name, type, value, default, ``is_default``, help, choices and stage)

  .. option:: --stage <string>

    Only this stage: its name in any case (``PLACED``), or a command that reaches it (``place``, ``read_routing``). An unknown stage is refused, with the names that exist. Default: all

  .. option:: --changed

    Only the options that do not have their default value

  .. option:: --query <string>

    Only the options whose name matches this shell-style pattern, e.g., ``place.*``

  .. option:: --limit <int>

    Most options to list for each stage. Default: 1000

  .. code-block::

    report_stage --stage place --query "place.*" --limit 20
    report_stage --changed

report_run
~~~~~~~~~~

  Answer with what each stage of the flow did, in one place: kind ``run_report``, read from the stage ``DEVICE`` (it works as soon as the inputs are read, and the parts of the stages that are not reached are ``null``). It reads the data of VPR, and for what only the log of VPR says, what the stage printed when a command of the session ran it.

  - ``stages``: the stages that are reached (``PACKED``, ``PLACED``, ``ROUTED``, ``ANALYZED``) with the ``seconds`` that the command took, ``null`` if the results were read from a file
  - ``packing``: the number of ``clusters``, ``atoms`` (the blocks of the circuit) and ``nets``, and the ``blocks`` of each type
  - ``placement``: ``half_perimeter``, the sum over the nets of the half perimeter of the box around their blocks, from the data. From the log of the placement: ``cost``, ``bb_cost``, ``td_cost``, the ``estimated_critical_path_delay_ns`` and ``estimated_fmax_mhz`` that the placer ends with, ``num_temperatures`` and ``swap_attempts``
  - ``routing``: the ``channel_width``, the ``wirelength`` (the tiles that the wires span, which is what VPR adds up) and the number of ``wires``, from the data. From the log of the routing: ``iterations`` and ``critical_path_delay_ns``
  - ``failed``: the stage that failed last, as in ``report_session``

  What comes from a log is ``null`` when the stage was not run by a command (e.g., ``read_placement``), and the tests compare each number with the log of VPR, so that a change of the log is noticed.

  .. code-block::

    report_run

timing_summary
~~~~~~~~~~~~~~

  Answer with the timing of the routed design: kind ``timing_summary``, read from the stage ``ROUTED``. The timing is analyzed again from the routing, as the analysis of VPR does, so the numbers are the ones of the analysis stage, and the analysis does not have to run first. The timing analysis must be on (``general.timing_analysis``).

  - ``setup``: ``worst_negative_slack_ns`` and ``total_negative_slack_ns`` (the sWNS and sTNS of VPR), ``critical_path_delay_ns`` (of the path with the least slack), ``fmax_mhz`` (its inverse) and the clocks of that path
  - ``hold``: ``worst_negative_slack_ns`` and ``total_negative_slack_ns``
  - ``clock_domains``: one entry for each pair of clocks that has a path: ``launch_clock``, ``capture_clock``, ``intra_domain``, ``critical_path_delay_ns``, ``worst_slack_ns``, ``total_negative_slack_ns`` (the sum of the negative slacks of the paths of this pair: the pairs add up to the sTNS of VPR) and ``fmax_mhz`` (only for a path within one clock, ``null`` for others)

  .. code-block::

    timing_summary

critical_paths
~~~~~~~~~~~~~~

  Answer with the worst setup paths of the routed design: kind ``critical_paths``, read from the stage ``ROUTED``. The paths come worst first, as in the timing report of VPR. Each one has its ``startpoint``, ``endpoint``, clocks and ``slack_ns``, and three lists of points in the order of the timing report of VPR, each point with its ``pin``, its ``type``, the ``block`` and ``net`` of the netlist, the ``arrival_ns`` and the ``delay_ns`` since the point before:

  - ``clock_launch_path``: the clock, from its source to the pin of the block that launches the data. Empty when the data starts at an input of the circuit
  - ``points``: the data path. The first point is the output of the block that launches the data, so its ``delay_ns`` is the one of that block (the clock-to-output delay), and its ``arrival_ns`` includes the ``clock_launch_path``
  - ``clock_capture_path``: the clock, from its source to the pin of the block that captures the data. Empty when the data ends at an output of the circuit

  ``data_arrival_ns`` is the arrival of the last data point, and ``data_required_ns`` is what the capturing clock requires (it includes the delay of ``clock_capture_path`` and the setup time of the block).

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

  Answer with the use of the routing channels: kind ``congestion``, read from the stage ``ROUTED``. A channel segment is used by the wires of the nets that cross it. The answer gives ``routed`` (``false`` when it reads the routing that a failed route left, which is allowed after a failure: then the wires that more than one net uses are counted once as wires in use, and ``overused_nodes`` tells how many there are), the ``used`` wires, the ``capacity`` and the ``occupancy`` of the horizontal (``chanx``) and vertical (``chany``) channels, for the whole device in ``summary`` and for each region. A region is a square of tiles; only the regions that have a wire in use are listed. ``overused_nodes`` is the number of wires that more than one net uses, which is none in a legal routing. The wires in use add up to the wirelength of VPR, and the regions, with a size of one tile, to its channel occupancy files.

  .. option:: --region_size <int>

    Number of tiles on a side of a region. Default: 1

  .. option:: --hotspots <int>

    Also list this many of the fullest segments of the channels in ``hotspots``, the fullest first (ties go by the number of wires, then by the position). A segment is a place along a channel, beside a tile (``x``, ``y``), that wires cross: it has its ``channel`` (``chanx`` or ``chany``), the ``used`` wires, the ``capacity`` and ``occupancy``, ``overused_nodes`` (the wires there that more than one net uses), and the nets that cross it: ``num_nets`` and ``nets``, their names (at most 50). It tells what is in the way of a design that does not route

  .. code-block::

    congestion --region_size 2
    congestion --hotspots 5

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

block_info
~~~~~~~~~~

  Answer with one cluster of the packed design: kind ``block_info``, read from the stage ``PACKED``. It has its ``name``, ``type``, and ``location`` (``x``, ``y``, ``sub_tile`` and ``layer``) once the design is placed (``placed`` tells, and ``location`` is ``null`` before that). ``pins`` are the pins of the cluster that are connected: ``port``, ``bit``, ``direction`` (``input``, ``output`` or ``clock``) and the ``net``. ``atoms`` are the blocks of the circuit that the cluster holds, which is how a name of the circuit finds its place in the device: each has its ``name``, ``model`` (e.g., ``.names`` or ``.latch``) and ``path`` in the cluster, as VPR writes it (``clb[0]/fle[2][n1_lut6]/ble6[0]/lut6[0]``, with the mode in brackets).

  .. option:: --block <string>

    Name of the cluster, as in ``find`` and ``placement``

  .. code-block::

    block_info --block n1

net_info
~~~~~~~~

  Answer with one net between the clusters of the packed design: kind ``net_info``, read from the stage ``PACKED``. It has its ``name``, ``fanout`` (the number of sinks), ``ignored`` (true for a net that VPR does not route, e.g., a clock), the ``driver`` and the ``sinks`` (each has the ``block``, ``port`` and ``bit`` of a pin). The next parts come from the stages that are reached, and are ``null`` before them:

  - ``bounding_box`` (placed): ``xmin``, ``ymin``, ``xmax``, ``ymax`` of the blocks that the net connects, and ``half_perimeter``, their width plus height
  - ``routing`` (routed): ``nodes`` of its routing tree, ``wires`` (the nodes of the channels) and ``wirelength``, the tiles that the wires span, which is what VPR adds up in its wirelength
  - ``timing`` (routed, and the timing analysis is on): ``criticality`` of the worst connection of the net, from 0 to 1, and ``slack_ns``, its worst setup slack. It is analyzed again from the routing, as in ``timing_summary``

  .. option:: --net <string>

    Name of the net

  .. code-block::

    net_info --net n1

export_view
~~~~~~~~~~~

  Write the data of a view of the design as a binary file of tables, for the views of the app (``docs/plan_design_views.md``): typed columns that the page maps as arrays with no parse, and text as a blob with its offsets. It answers kind ``view_export``: the ``file``, its ``version``, ``bytes``, ``cached`` and the rows of each of its ``tables``. The version changes when a stage that the kind reads runs again; a file that holds the current version is not written again (``cached`` is true). The kinds, and the stage each needs:

  - ``device`` (DEVICE): the tile types and their sub-tiles, the tiles, the width of each channel
  - ``placement`` (PLACED): the blocks on their slots, fixed or not, and the regions of the placement constraints
  - ``pins`` (DEVICE): the I/O sites with their side, and the ports of the design on them once placed
  - ``routing`` (ROUTED): the route of each net (its nodes, in the order of ``net_route``) and the use of each segment of the channels
  - ``timing`` (ROUTED): the slack and the criticality of each net and the worst slack of each block, by their index (the column ``net`` of ``routing`` and ``clusters``, the column ``id`` of the blocks of ``placement`` and ``clusters``: no names, which those have), and the ``--paths`` worst paths with their points. The analysis is kept while the routing is the same: the tiles of ``timing`` use it, and do not analyze again
  - ``atoms`` (DEVICE): the atoms of a scope with their model, cluster, pins and nets, and the module of the RTL of each: from the name of the net it drives, or from the named atoms it reaches (``module_by`` in the meta counts how)
  - ``clusters`` (PACKED): the clusters of a scope, their atoms, and the nets between them

  The format: the magic ``OFVIEW1`` and a 0, the length of the header (u32, little-endian), the header (JSON: ``kind``, ``version``, ``meta``, ``tables`` with their ``columns``: ``name``, ``dtype``, ``offset``, ``count``), then the columns from the next multiple of 8, each on a multiple of 8. ``openfpga.read_view()`` reads it in Python.

  .. option:: --kind <string>

    The kind of the data

  .. option:: --file <path>

    The file to write

  .. option:: --module <string>

    atoms: the atoms of a module and of the modules in it (``.`` for the top)

  .. option:: --cone <string>

    atoms: the atoms around an atom or a net, ``--depth`` nets away (2)

  .. option:: --blocks <string>

    clusters: these clusters, separated by commas

  .. option:: --limit <int>

    atoms, clusters: the most that an export has (100000); a larger design needs a scope

  .. option:: --paths <int>

    timing: the number of worst paths (20, at most 100)

  .. code-block::

    export_view --kind routing --file views/routing.bin
    export_view --kind atoms --module u_core.u_alu --file views/alu.bin

view_tile
~~~~~~~~~

  Write a tile of a view of the device as a binary file of tables, as ``export_view`` does: the square of ``16 << level`` grid tiles at ``(x, y)``. Level 0 has the objects of the square: ``grid`` the tiles, ``placement`` the blocks, ``routing`` the route nodes (with their net and their place in its tree), ``channels`` the wires of the channels with their use, ``timing`` the blocks with their worst slack. A coarser level has one value per cell of ``2^level`` grid tiles, 16 by 16 cells: ``grid`` the type of most tiles, ``placement`` the share of the slots that are used, ``routing`` and ``channels`` the use of the channels (``value``, and ``peak`` for the fullest segment), ``timing`` the worst slack. What a kind reads again and again (the routes and the wires by tile, the slacks) is made once a version.

  .. option:: --kind <string>

    ``grid``, ``placement``, ``routing``, ``channels`` or ``timing``

  .. option:: --level <int>

    0 for the objects, more for a value per cell

  .. option:: --x <int>

    The column of the tile at its level

  .. option:: --y <int>

    The row of the tile at its level

  .. option:: --file <path>

    The file to write

  .. code-block::

    view_tile --kind channels --level 2 --x 0 --y 1 --file tiles/channels_2_0_1.bin
