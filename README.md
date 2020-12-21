# Getting Started with OpenFPGA <img src="./docs/source/overview/figures/OpenFPGA_logo.png" width="200" align="right">
[![linux build](https://github.com/LNIS-Projects/OpenFPGA/workflows/linux_build/badge.svg)](https://github.com/LNIS-Projects/OpenFPGA/actions?query=workflow%3Alinux_build)
[![Documentation Status](https://readthedocs.org/projects/openfpga/badge/?version=master)](https://openfpga.readthedocs.io/en/master/?badge=master)

## Introduction

The award-winning OpenFPGA framework is the **first open-source FPGA IP generator** supporting highly-customizable homogeneous FPGA architectures. OpenFPGA provides a full set of EDA support for customized FPGAs, including Verilog-to-bitstream generation and self-testing verification. OpenFPGA opens the door to democratizing FPGA technology and EDA techniques, with agile prototyping approaches and constantly evolving EDA tools for chip designers and researchers.

A quick overview of OpenFPGA tools can be found [**here**](https://openfpga.readthedocs.io/en/master/tutorials/tools/).
We also recommend potential users to checkout the summary of [**technical capabilities**](https://openfpga.readthedocs.io/en/master/overview/tech_highlights.html) before compiling.

## Compilation

Before start, we strongly recommend you to read the required dependencies at [**compilation guidelines**](https://openfpga.readthedocs.io/en/master/tutorials/compile.html).
It also includes detailed information about docker image. 

---

**Compilation Steps:**

```bash
# Clone the repository and go inside it
git clone https://github.com/LNIS-Projects/OpenFPGA.git && cd OpenFPGA
make all
```

---

**Quick Compilation Verification**

To quickly verify the tool is well compiled, user can run the following command from OpenFPGA root repository.
```bash
python3 openfpga_flow/scripts/run_fpga_task.py compilation_verification --debug --show_thread_logs
```

*Python3 and iVerilog v10.1+ are required. GUI will pop-up if enabled during compilation.*

---

**Supported Operating Systems**

We currently target OpenFPGA for:
 1. Ubuntu 18.04
 2. Red Hat 7.5

*The tool was tested with these operating systems. It might work with earlier versions and other distributions.*

## Documentation

OpenFPGA's [full documentation](https://openfpga.readthedocs.io/en/master/) includes tutorials, descriptions of the design flow, and tool options.

## Tutorials

You can find some tutorials in the [**./tutorials**](./docs/source/tutorials/) folder. This will help you get more familiar with the tool and use OpenFPGA under different configurations. 
