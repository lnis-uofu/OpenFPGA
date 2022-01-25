from __future__ import print_function
from collections import OrderedDict, namedtuple
import itertools
import re
import eblif
import lxml.etree as ET

IoConstraint = namedtuple('IoConstraint', 'name x y z comment')

HEADER_TEMPLATE = """\
#{name:<{nl}} x   y   z    pcf_line
#{s:-^{nl}} --  --  -    ----"""

CONSTRAINT_TEMPLATE = '{name:<{nl}} {x: 3} {y: 3} {z: 2}  # {comment}'
INOUT_REGEX = re.compile(r"^(.+)(_\$inp|_\$out)(.*)$")
NETNAME_REGEX = re.compile(r"(.+?)(\[[0-9]+\]$|$)")


class IoPlace(object):
    def __init__(self):
        self.constraints = OrderedDict()
        self.inputs = set()
        self.outputs = set()
        self.net_to_block = None
        self.net_map = {}
        self.block_to_inst = {}
        self.inout_nets = set()
        self.net_to_pad = set()
        self.net_file_io = set()

    def read_io_loc_pairs(self, blif):
        """
         Read IO_LOC_PAIRS parameters from eblif carrying the information
         which package pin a specified top port is constrained, e.g. O_LOC_PAIRS = "portA:D1"
         In case of differential inputs/outputs there are two pairs of the parameter,
         i.e. IO_LOC_PAIRS = "portA_p:D2,portA_n:D4"
        """
        if 'subckt' not in blif:
            return
        for attr in blif['subckt']:
            if 'param' not in attr:
                continue
            if 'IO_LOC_PAIRS' in attr['param']:
                locs = attr['param']['IO_LOC_PAIRS'][1:-1].split(',')
                if 'NONE' in locs:
                    continue
                for loc in locs:
                    net, pad = loc.split(':')
                    self.net_to_pad.add((net, pad))

    def read_io_list_from_eblif(self, eblif_file):
        blif = eblif.parse_blif(eblif_file)

        self.inputs = set(blif['inputs']['args'])
        self.outputs = set(blif['outputs']['args'])

        # Build a net name map that maps products of an inout port split into
        # their formet name.
        #
        # For example, an inout port 'A' is split into 'A_$inp' and 'A_$out',
        # port B[2] into 'B_$inp[2]' and 'B_$out[2]'.
        self.net_map = {}
        self.inout_nets = set()
        for net in itertools.chain(self.inputs, self.outputs):
            match = INOUT_REGEX.match(net)
            if match:
                alias = match.group(1) + match.group(3)
                self.inout_nets.add(alias)
                self.net_map[net] = alias
            else:
                self.net_map[net] = net
        self.read_io_loc_pairs(blif)

    def load_block_names_from_net_file(self, net_file):
        """
        .place files expect top-level block (cluster) names, not net names, so
        build a mapping from net names to block names from the .net file.
        """
        net_xml = ET.parse(net_file)
        net_root = net_xml.getroot()
        self.net_to_block = {}

        for block in net_root.xpath("//block"):
            instance = block.attrib["instance"]
            if instance != "inpad[0]" and instance != "outpad[0]":
                continue

            top_block = block.getparent()
            assert top_block is not None
            while top_block.getparent() is not net_root:
                assert top_block is not None
                top_block = top_block.getparent()
            self.net_to_block[block.get("name")] = top_block.get("name")

        # Loop over all top-level blocks. Store block name to its instance
        # correspondences.
        for block_xml in net_root.findall("block"):
            name = block_xml.attrib["name"]
            inst = block_xml.attrib["instance"]

            assert name not in self.block_to_inst, block_xml.attrib
            self.block_to_inst[name] = inst

    def load_net_file_ios(self, net_file):
        """
        Loads input and outputs net names from the netlist file.
        """

        def get_ios(net_root, io_types):
            "Get the top level io signals from the netlist XML root."
            for io_type in io_types:
                io = net_root.xpath("/block/{}/text ()".format(io_type))

                if len(io) == 1:
                    yield io[0]

        net_xml = ET.parse(net_file)
        net_root = net_xml.getroot()

        for io_line in get_ios(net_root, ["inputs", "outputs"]):
            io_list = io_line.split(" ")
            for io in io_list:
                self.net_file_io.add(io.replace("out:", ""))

    def get_top_level_block_instance_for_net(self, net_name):
        """
        Returns a name of the top-level block instance for the given net
        name.
        """
        assert self.is_net(net_name)

        # VPR prefixes output constraints with "out:"
        if net_name in self.outputs:
            net_name = 'out:' + net_name

        # This is an inout net
        if net_name in self.inout_nets:
            block_names = set()

            for prefix, suffix in zip(["", "out:"], ["_$inp", "_$out"]):
                match = NETNAME_REGEX.match(net_name)
                name = prefix + match.group(1) + suffix + match.group(2)
                block_names.add(self.net_to_block[name])

            # Both parts of the net should point to the same block
            assert len(block_names) == 1, (net_name, block_names)
            return self.block_to_inst[list(block_names)[0]]

        # A regular net
        else:
            if net_name in self.net_to_block:
                block_name = self.net_to_block[net_name]
                return self.block_to_inst[block_name]
            else:
                return None

    def constrain_net(self, net_name, loc, comment=""):
        assert len(loc) == 3
        assert net_name not in self.constraints

        assert self.is_net(net_name), "net {} not in eblif".format(net_name)

        # VPR prefixes output constraints with "out:"
        if net_name in self.outputs:
            net_name = 'out:' + net_name

        # This is an inout net
        if net_name in self.inout_nets:
            for prefix, suffix in zip(["", "out:"], ["_$inp", "_$out"]):

                match = NETNAME_REGEX.match(net_name)
                name = prefix + match.group(1) + suffix + match.group(2)

                self.constraints[name] = IoConstraint(
                    name=name,
                    x=loc[0],
                    y=loc[1],
                    z=loc[2],
                    comment=comment,
                )

        # A regular net
        else:
            self.constraints[net_name] = IoConstraint(
                name=net_name,
                x=loc[0],
                y=loc[1],
                z=loc[2],
                comment=comment,
            )

    def output_io_place(self, f):
        max_name_length = max(len(c.name) for c in self.constraints.values())
        print(
            HEADER_TEMPLATE.format(
                name="Block Name", nl=max_name_length, s=""
            ),
            file=f
        )

        constrained_blocks = {}

        for vpr_net, constraint in self.constraints.items():
            name = constraint.name
            name = self.net_to_block.get(name) if self.net_to_block else name

            # This block is already constrained, check if there is no
            # conflict there.
            if name in constrained_blocks:
                existing = constrained_blocks[name]

                if existing.x != constraint.x or\
                   existing.y != constraint.y or\
                   existing.z != constraint.z:

                    print(
                        "Error: block '{}' has multiple conflicting constraints!"
                        .format(name)
                    )
                    print("", constrained_blocks[name])
                    print("", constraint)
                    exit(-1)

                # Don't write the second constraing
                continue

            # omit if no corresponding block name for the net
            if name is not None:
                print(
                    CONSTRAINT_TEMPLATE.format(
                        name=name,
                        nl=max_name_length,
                        x=constraint.x,
                        y=constraint.y,
                        z=constraint.z,
                        comment=constraint.comment
                    ),
                    file=f
                )

                # Add to constrained block list
                constrained_blocks[name] = constraint

    def is_net(self, net):
        return net in self.net_map.values()

    def is_net_packed(self, net):
        return net in self.net_file_io

    def get_nets(self):
        for net in self.inputs:
            yield self.net_map[net]
        for net in self.outputs:
            yield self.net_map[net]
