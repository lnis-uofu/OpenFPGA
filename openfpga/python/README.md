# openfpga

The OpenFPGA shell as a Python module (`import openfpga`): read the inputs, run
the stages of the flow (`pack`, `place`, `route`, `analysis`) one at a time,
look at the results as typed objects, save the flow and go on from a checkpoint,
and run flows in worker processes that cannot take the interpreter down.

```python
import openfpga

with openfpga.RemoteSession(cwd="run1") as session:
    session.options["router.route_chan_width"] = 100
    session.read_vpr_arch("arch.xml", layout="2x2")
    session.read_openfpga_arch("openfpga_arch.xml")
    session.read_circuit("design.blif")
    session.run_flow()
    print(session.timing_summary().setup.fmax_mhz)
```

It is built from the source of OpenFPGA (a checkout with its submodules):

```shell
pip install .
```

The build needs a C++17 compiler, CMake, SWIG, the Python development headers,
and the Tcl development headers (`tcl-dev`/`tcl-devel`) that VPR's SDC parser
uses. See the documentation of OpenFPGA, *Python Session*, for the use.

## Files of this directory

The module is made of the classes that SWIG generates from `openfpga/src/openfpga_python.i`
and of the Python code in this directory, which the `.i` file includes (`%pythoncode`):

- `openfpga_python_diagnostics.py`: `Diagnostic` and `parse_diagnostics()`, what went wrong as data.
- `openfpga_python_session.py`: `Session`, the typed answers of the queries, the options and the checkpoints.
- `openfpga_python_worker.py`: `RemoteSession`, a `Session` in a process of its own.

These files are not modules that can be imported by themselves: they use the names that the `.i` file defines
(`OpenfpgaShell`, `OpenfpgaError`) and end up in the one module `openfpga`. The CMake file lists them as
dependencies, so that the module is generated again when one of them changes.
