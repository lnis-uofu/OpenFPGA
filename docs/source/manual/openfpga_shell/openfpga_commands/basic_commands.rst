.. _openfpga_basic_commands:

Basic Commands
--------------

version
~~~~~~~

  Show OpenFPGA version information

help
~~~~

  Show help desk to list all the available commands

source
~~~~~~

  Run a set of existing commands from a string stream or a file

  .. option:: --command_stream <string>

    A string/file stream which contains the commands to be executed. Use quote(``"``) to group command and semicolumn(``;``) to split between commands. For example,

  .. code-block::

     source --command_stream "help;exit;"

  .. option:: --from_file

    Specify the command stream comes from a file. When selected, the file will be parsed as a regular script following the OpenFPGA script file format. See details in :ref:`openfpga_script_format`

  .. option:: --batch_mode

    Enable batch mode when executing the script from a file. Valid only when ``--from_file`` is enabled.

    .. note:: If you are sourcing a file when running OpenFPGA in script mode, please turn on the batch mode here. See details in :ref:`launch_openfpga_shell`

ext_exec
~~~~~~~~

  Run a system call for a command which is not in OpenFPGA shell

  .. option:: --command <string>

    A string stream which contains the command to be executed. Use quote(``"``) to group command. For example,

  .. code-block::

    ext_exec --command "ls -all"

exit
~~~~

  Exit OpenFPGA shell

