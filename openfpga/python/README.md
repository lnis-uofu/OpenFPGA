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
