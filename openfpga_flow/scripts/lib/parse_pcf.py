"""
Supported PCF commands:
* set_io  <net> <pad> - constrain a given <net> to a given physical <pad> in eFPGA pinout.
* set_clk <pin> <net> - constrain a given global clock <pin> to a given <net>
  Every tile where <net> is present will be constrained to use a given global clock.
"""

from collections import namedtuple
import re

PcfIoConstraint = namedtuple('PcfIoConstraint', 'net pad line_str line_num')
PcfClkConstraint = namedtuple('PcfClkConstraint', 'pin net')


def parse_simple_pcf(f):
    """ Parse a simple PCF file object and yield PcfIoConstraint objects. """
    for line_number, line in enumerate(f):
        line_number += 1

        # Remove comments.
        args = re.sub(r"#.*", "", line.strip()).split()

        if not args:
            continue

        # Ignore arguments.
        args = [arg for arg in args if arg[0] != '-']
        assert len(args) == 3, args

        if args[0] == 'set_io':

            yield PcfIoConstraint(
                net=args[1],
                pad=args[2],
                line_str=line.strip(),
                line_num=line_number,
            )

        if args[0] == 'set_clk':

            yield PcfClkConstraint(
                pin=args[1],
                net=args[2],
            )
