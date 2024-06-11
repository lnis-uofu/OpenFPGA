# Getting Started with OpenFPGA <img src="./docs/source/overview/figures/OpenFPGA_logo.png" width="200" align="right">
[![Test](https://github.com/lnis-uofu/OpenFPGA/actions/workflows/build.yml/badge.svg)](https://github.com/lnis-uofu/OpenFPGA/actions/workflows/build.yml)
[![Cell Library Tests](https://github.com/lnis-uofu/OpenFPGA/actions/workflows/cell_lib_test.yml/badge.svg)](https://github.com/lnis-uofu/OpenFPGA/actions/workflows/cell_lib_test.yml)
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)
[![Documentation Status](https://readthedocs.org/projects/openfpga/badge/?version=master)](https://openfpga.readthedocs.io/en/master/?badge=master)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/lnis-uofu/OpenFPGA/master?urlpath=vscode)

Version: see [`VERSION.md`](VERSION.md)

## Introduction

The award-winning OpenFPGA framework is the **first open-source FPGA IP generator with silicon proofs** supporting highly-customizable FPGA architectures. OpenFPGA provides complete EDA support for customized FPGAs, including Verilog-to-bitstream generation and self-testing verification. OpenFPGA opens the door to democratizing FPGA technology and EDA techniques with agile prototyping approaches and constantly evolving EDA tools for chip designers and researchers.

> [!TIP]
> If this is your first time working with OpenFPGA, we strongly recommend you watch the [introduction video about OpenFPGA](https://youtu.be/ocODUGcYGqo)

A quick overview of OpenFPGA tools can be found [**here**](https://openfpga.readthedocs.io/en/master/tutorials/getting_started/tools/).
We also recommend potential users check out the summary of [**technical capabilities**](https://openfpga.readthedocs.io/en/master/overview/tech_highlights/#) before compiling.

> [!TIP]
> Before asking for help, please checkout the [Frequently Asked Questions](https://github.com/lnis-uofu/OpenFPGA/discussions/937)

## Compilation

> [!NOTE]
> A tutorial video about how to compile can be found [here](https://youtu.be/F9sMRmDewM0)

Detailed guidelines are available at [**compilation guidelines**](https://openfpga.readthedocs.io/en/master/tutorials/getting_started/compile/).
Before starting, we strongly recommend you read the required dependencies and ensure that they are correctly installed.
It also includes detailed information about the docker image.

## Documentation

OpenFPGA's [full documentation](https://openfpga.readthedocs.io/en/master/) includes tutorials, descriptions of the design flow, and tool options.

## Tutorials

You can find a set of [tutorials](https://openfpga.readthedocs.io/en/master/tutorials/), with which you get familiar with the tool and use OpenFPGA for various purposes.

## Backward Compatibility

If you were using an old version of OpenFPGA and are now interested to move to the latest version, please check out the [developer guidelines](https://openfpga.readthedocs.io/en/master/dev_manual/back_compatible/).

## License

All the codes are under MIT license, with the exception of submodules, e.g., VTR, Yosys and Yosys-plugin, which are distributed under its own (permissive) terms. See their full license for details.

## How to Cite

Please use the following paper as a general citation for OpenFPGA:

X. Tang, E. Giacomin, B. Chauviere, A. Alacchi and P. -E. Gaillardon, "OpenFPGA: An Open-Source Framework for Agile Prototyping Customizable FPGAs," in IEEE Micro, vol. 40, no. 4, pp. 41-48, 1 July-Aug. 2020, doi: 10.1109/MM.2020.2995854.

Bibtex:

```
@ARTICLE{9098028,  author={Tang, Xifan and Giacomin, Edouard and Chauviere, Baudouin and Alacchi, Aurélien and Gaillardon, Pierre-Emmanuel},  journal={IEEE Micro},   title={OpenFPGA: An Open-Source Framework for Agile Prototyping Customizable FPGAs},   year={2020},  volume={40},  number={4},  pages={41-48},  doi={10.1109/MM.2020.2995854}}
```

A list of related publications can be found [here](https://openfpga.readthedocs.io/en/master/appendix/reference/).

## Contributing to OpenFPGA

Please read the [contributor guidelines](https://openfpga.readthedocs.io/en/master/dev_manual/contributor_guide/) if you would like to contribute to OpenFPGA.
