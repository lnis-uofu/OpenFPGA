import argparse
import xml.etree.ElementTree as ET

parser = argparse.ArgumentParser()
parser.add_argument("--default_bitstream", help="Path to default bitstream file")
parser.add_argument("--auto_bitstream", help="Path to auto bitstream file")
parser.add_argument("--first_bitstream", help="Path to first bitstream file")
parser.add_argument("--last_bitstream", help="Path to last bitstream file")
# parser.add_argument("--unused_input_bitstream", help="Path to unused_input bitstream file")
args = parser.parse_args()

print("Validated the bitstream generated for the unused mux input")
print(f"Default bitstream: {args.default_bitstream}")
print(f"Auto bitstream: {args.auto_bitstream}")
print(f"First bitstream: {args.first_bitstream}")
print(f"Last bitstream: {args.last_bitstream}")
# print(f"Unused input bitstream: {args.unused_input_bitstream}")


def get_unused_muxes(root):
    unused_muxes = {}
    interconnect_muxes = []
    for bitstream_block in root.findall(".//bitstream_block"):
        bitstream = bitstream_block.find("bitstream")
        if bitstream is not None:
            hierarchy = bitstream_block.find("hierarchy")
            output_nets = bitstream_block.find("output_nets")
            input_nets = bitstream_block.find("input_nets")
            bitstream = bitstream_block.find("bitstream")
            bitstream = [tag.attrib["value"] for tag in bitstream]
            if hierarchy is not None and output_nets is not None:
                used = bool(output_nets.find("path").attrib["net_name"] != "unmapped")
                if not used:
                    mux_path = ".".join(
                        [tag.attrib["name"] for tag in hierarchy.findall("instance")]
                    )
                    if input_nets is None:
                        interconnect_muxes.append(mux_path)
                        continue
                    input_nets = [
                        tag.attrib["net_name"] for tag in input_nets.findall("path")
                    ]
                    unused_muxes[mux_path] = input_nets, bitstream
    return unused_muxes, interconnect_muxes

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Parse default bitstream to identify unused muxes and their input nets
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
tree = ET.parse(args.default_bitstream)
root = tree.getroot()

unused_muxes, interconnect_muxes = get_unused_muxes(root)

print(f"Found {len(unused_muxes)} unused muxes in the default bitstream:")
print(f"Found {len(interconnect_muxes)} interconnect muxes in the default bitstream:")

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Validate the unused muxes in the auto bitstream
# It should have the same set of unused muxes as the default bitstream
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
tree = ET.parse(args.auto_bitstream)
root = tree.getroot()
auto_unused_muxes, auto_interconnect_muxes = get_unused_muxes(root)
if set(unused_muxes.keys()) != set(auto_unused_muxes.keys()):
    exit(1)
if set(interconnect_muxes) != set(auto_interconnect_muxes):
    exit(1)

for path in unused_muxes.keys():
    default_bit, last_bit = unused_muxes[path], auto_unused_muxes[path]
    if default_bit[1] != last_bit[1]:
        print(f"Bits Mismatch for mux {path}")
        exit(1)


# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Validate the unused muxes in the first bitstream
# All the unused muxes bit should be set to 0
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
tree = ET.parse(args.first_bitstream)
root = tree.getroot()
first_unused_muxes, first_interconnect_muxes = get_unused_muxes(root)
if set(unused_muxes.keys()) != set(first_unused_muxes.keys()):
    exit(1)

for path in unused_muxes.keys():
    default_bit, first_bit = unused_muxes[path], first_unused_muxes[path]
    expected_bits = ['1'] * len(first_bit[1])
    if first_bit[1] != expected_bits:
        print(f"Bits Mismatch for mux {path} {first_bit[1]} != {expected_bits}")
        exit(1)


# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Validate the unused muxes in the last bitstream
# All the unused muxes bit should be set to 0
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
tree = ET.parse(args.last_bitstream)
root = tree.getroot()
last_unused_muxes, last_interconnect_muxes = get_unused_muxes(root)
assert set(unused_muxes.keys()) == set(last_unused_muxes.keys())

for path in unused_muxes.keys():
    default_bit, last_bit = unused_muxes[path], last_unused_muxes[path]
    expected_bits = ["0"] * len(last_bit[1])
    if last_bit[1] != expected_bits:
        print(f"Bits Mismatch for mux {path} {last_bit[1]} != {expected_bits}")
        exit(1)

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# NotImplemented: Validate the unused muxes in the last bitstream
# All the unused muxes bit should be set to 0, and the interconnect muxes should
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# tree = ET.parse(args.unused_input_bitstream)
# root = tree.getroot()

# for path in unused_muxes.keys():
#     input_nets, bitstream = unused_muxes[path]
#     first_unmapped = input_nets.index("unmapped")
#     expected_bit = f"{bin(first_unmapped)[2:].zfill(len(bitstream))}"
#     expected_bit_str = list(expected_bit)
#     if bitstream != expected_bit_str:
#         print(f"Bits Mismatch for mux {path} {bitstream} != {expected_bit_str}")
#         exit(1)
