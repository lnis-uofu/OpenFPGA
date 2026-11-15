.. _openfpga_mcp_server:

MCP Server
----------

``openfpga-mcp`` is a server of the `Model Context Protocol <https://modelcontextprotocol.io>`_ that lets a language-model client run the flow of a :ref:`Session <openfpga_python_session>` and read its results. It is an optional part of the Python module: it adds nothing to the C++ build, and it is installed with an extra.

.. code-block:: shell

  ~/openfpga-env/bin/pip install '.[mcp]'      # Python 3.10 or later
  ~/openfpga-env/bin/openfpga-mcp --root ~/workspace --input-dir openfpga_flow/vpr_arch --input-dir openfpga_flow/openfpga_arch

The client starts the command and speaks to it on its standard input and output. The configuration of most clients is a file with the command and its arguments:

.. code-block:: json

  {
    "mcpServers": {
      "openfpga": {
        "command": "openfpga-mcp",
        "args": ["--root", "/home/me/openfpga-workspace",
                 "--input-dir", "/home/me/openfpga/openfpga_flow/vpr_arch"]
      }
    }
  }

Each session is a worker process with a directory of its own in the workspace, so that a crash of VPR ends one session and not the server. The tools open and close sessions, read the inputs, run a stage, set options and constraints, and ask the queries (timing, utilization, placement, congestion, ``find``, the information of a block or a net); a failure comes back as diagnostics, not as an error. Every answer is cut to a size (4,000 characters by default) and says when it was cut and how to ask for the rest.

The server reads and writes only inside the workspace and the input folders it was given, runs no command of the shell, and listens on the local machine only (``--transport http``). The tools, the limits and the safety rules are in ``openfpga/python/mcp/DESIGN.md`` and ``README.md`` in the source tree.
