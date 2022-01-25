#!/usr/bin/env python3
"""
Convert a PCF file into a VPR io.place file.
"""
import argparse
import csv
import sys
import re
from collections import defaultdict

import vpr_io_place
from pinmap_parse import read_pinmapfile_data
from pinmap_parse import vec_to_scalar

from lib.parse_pcf import parse_simple_pcf

# =============================================================================

BLOCK_INSTANCE_RE = re.compile(r"^(?P<name>\S+)\[(?P<index>[0-9]+)\]$")


# =============================================================================
def gen_io_def(args):
    '''
    Generate io.place file from pcf file
    '''
    io_place = vpr_io_place.IoPlace()
    io_place.read_io_list_from_eblif(args.blif)
    io_place.load_block_names_from_net_file(args.net)

    # Load all the necessary data from the pinmap_xml
    io_cells, port_map = read_pinmapfile_data(args.pinmap_xml)

    # Map of pad names to VPR locations.
    pad_map = defaultdict(lambda: dict())

    with open(args.csv_file, mode='r') as csv_fp:
        reader = csv.DictReader(csv_fp)
        for line in reader:
            port_name_list = vec_to_scalar(line['port_name'])
            pin_name = vec_to_scalar(line['mapped_pin'])
            gpio_type = line['GPIO_type']

            if len(port_name_list) != len(pin_name):
                print(
                    'CSV port name "{}" length does not match with mapped pin name "{}" length'
                    .format(line['port_name'], line['mapped_pin']),
                    file=sys.stderr
                )
                sys.exit(1)

            for port, pin in zip(port_name_list, pin_name):
                if port in port_map:
                    curr_map = port_map[port]
                    if gpio_type is None or gpio_type == '':
                        pad_map[pin] = (
                            int(curr_map.x), int(curr_map.y), int(curr_map.z)
                        )
                    else:
                        gpio_pin = pin + ":" + gpio_type.strip()
                        pad_map[gpio_pin] = (
                            int(curr_map.x), int(curr_map.y), int(curr_map.z)
                        )
                else:
                    print(
                        'Port name "{}" specified in csv file "{}" is invalid. {} "{}"'
                        .format(
                            line['port_name'], args.csv_file,
                            "Specify from port names in xml file",
                            args.pinmap_xml
                        ),
                        file=sys.stderr
                    )
                    sys.exit(1)

    for pcf_constraint in parse_simple_pcf(args.pcf):
        if (type(pcf_constraint).__name__ == 'PcfIoConstraint'):
            pad_name = pcf_constraint.pad
            if not io_place.is_net(pcf_constraint.net):
                print(
                    'PCF constraint "{}" from line {} constraints net {} {}:\n{}'
                    .format(
                        pcf_constraint.line_str, pcf_constraint.line_num,
                        pcf_constraint.net, '\n'.join(io_place.get_nets()),
                        "which is not in available netlist"
                    ),
                    file=sys.stderr
                )
                sys.exit(1)

            if pad_name not in pad_map:
                print(
                    'PCF constraint "{}" from line {} constraints pad {} {}:\n{}'
                    .format(
                        pcf_constraint.line_str, pcf_constraint.line_num,
                        pad_name, '\n'.join(sorted(pad_map.keys())),
                        "which is not in available pad map"
                    ),
                    file=sys.stderr
                )
                sys.exit(1)

            # Get the top-level block instance, strip its index
            inst = io_place.get_top_level_block_instance_for_net(
                pcf_constraint.net
            )
            if inst is None:
                continue

            match = BLOCK_INSTANCE_RE.match(inst)
            assert match is not None, inst

            inst = match.group("name")

            # Constraint the net (block)
            locs = pad_map[pad_name]
            io_place.constrain_net(
                net_name=pcf_constraint.net,
                loc=locs,
                comment=pcf_constraint.line_str
            )

    if io_place.constraints:
        io_place.output_io_place(args.output)


# =============================================================================


def main():
    '''
    Convert a PCF file into a VPR io.place file
    '''
    parser = argparse.ArgumentParser(
        description='Convert a PCF file into a VPR io.place file.'
    )
    parser.add_argument(
        "--pcf",
        "-p",
        "-P",
        type=argparse.FileType('r'),
        required=True,
        help='PCF input file'
    )
    parser.add_argument(
        "--blif",
        "-b",
        type=argparse.FileType('r'),
        required=True,
        help='BLIF / eBLIF file'
    )
    parser.add_argument(
        "--output",
        "-o",
        "-O",
        type=argparse.FileType('w'),
        default=sys.stdout,
        help='The output io.place file'
    )
    parser.add_argument(
        "--net",
        "-n",
        type=argparse.FileType('r'),
        required=True,
        help='top.net file'
    )

    parser.add_argument(
        "--pinmap_xml",
        type=str,
        required=True,
        help="Input pin-mapping xml file"
    )
    parser.add_argument(
        "--csv_file",
        type=str,
        required=True,
        help="Input user-defined pinmap CSV file"
    )

    args = parser.parse_args()

    gen_io_def(args)


# =============================================================================

if __name__ == '__main__':
    main()
