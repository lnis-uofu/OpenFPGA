.. _file_formats_module_naming_file:

Fabric Module Naming (.xml)
---------------------------

The XML-based description language is used to describe module names for an FPGA fabric, including:

- the built-in name or default name for each module when building an FPGA fabric
- the customized name which is given by users for each module, in place of the built-in names

Using the description language, users can customize the name for each module in an FPGA fabric, excluding testbenches.

Under the root node ``<module_names>``, naming rules can be defined line-by-line through syntax ``<module_name>``.

.. code-block:: xml

  <module_names> 
    <module_name default="<string>" given="<string>"/> 
  </module_names> 

.. note:: If you do not need to rename a module of an FPGA fabric, there is no need to define it explicitly in the naming rules. OpenFPGA can infer it.

Syntax
``````

Detailed syntax are presented as follows.

.. option:: default="<string>"

  Define the default or built-in name of a module. This follows fixed naming rules of OpenFPGA. Suggest to run command :ref:`openfpga_setup_commands_write_module_naming_rules` to obtain an initial version for your fabric. For example, 

  .. code-block:: xml

   default="cbx_1__2_"

.. option:: given="<string>"

  Define the customized name of a module, this is the final name will appear in netlists. For example, 

  .. code-block:: xml

    given="cbx_corner_left_bottom"
