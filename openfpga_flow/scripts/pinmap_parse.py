#!/usr/bin/env python3
"""
This script parses given interface pin-mapping xml file, stores the pin-mapping
information w.r.t. its location in the device. It also generates a template
csv file for the end-user of the eFPGA device. User can modify the template
csv file and specify the user-defined pin names to the ports defined in the
template csv file.
"""

import argparse
import csv
import sys

from collections import namedtuple

import lxml.etree as ET


# =============================================================================
class PinMappingData(object):
    """
    Pin mapping data for IO ports in an eFPGA device.

    port_name   - IO port name
    mapped_pin  - User-defined pin name mapped to the given port_name
    x           - x coordinate corresponding to column number
    y           - y coordinate corresponding to row number
    z           - z coordinate corresponding to pin index at current x,y location
    """

    def __init__(self, port_name, mapped_pin, x, y, z):
        self.port_name = port_name
        self.mapped_pin = mapped_pin
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return "{Port_name: '%s' mapped_pin: '%s' x: '%s' y: '%s' z: '%s'}" % (
            self.port_name, self.mapped_pin, self.x, self.y, self.z
        )

    def __repr__(self):
        return "{Port_name: '%s' mapped_pin: '%s' x: '%s' y: '%s' z: '%s'}" % (
            self.port_name, self.mapped_pin, self.x, self.y, self.z
        )


"""
Device properties present in the pin-mapping xml

name    - Device name
family  - Device family name
width   - Device width aka number of cells in a row
heigth  - Device height aka number of cells in a column
z       - Number of cells per row/col
"""
DeviceData = namedtuple("DeviceData", "name family width height z")

# =============================================================================


def parse_io(xml_io, port_map, orientation, width, height, z):
    '''
    Parses IO section of xml file
    '''
    assert xml_io is not None

    pins = {}

    io_row = ""
    io_col = ""
    if orientation in ("TOP", "BOTTOM"):
        io_row = xml_io.get("y")
        if io_row is None:
            if orientation == "TOP":
                io_row = str(int(height) - 1)
            elif orientation == "BOTTOM":
                io_row = "0"
    elif orientation in ("LEFT", "RIGHT"):
        io_col = xml_io.get("x")
        if io_col is None:
            if orientation == "LEFT":
                io_col = "0"
            elif orientation == "RIGHT":
                io_col = str(int(width) - 1)

    for xml_cell in xml_io.findall("CELL"):
        port_name = xml_cell.get("port_name")
        mapped_name = xml_cell.get("mapped_name")
        startx = xml_cell.get("startx")
        starty = xml_cell.get("starty")
        endx = xml_cell.get("endx")
        endy = xml_cell.get("endy")

        # define properties for scalar pins
        scalar_mapped_pins = vec_to_scalar(mapped_name)

        i = 0
        if startx is not None and endx is not None:
            curr_startx = int(startx)
            curr_endx = int(endx)
            y = io_row
            if curr_startx < curr_endx:
                for x in range(curr_startx, curr_endx + 1):
                    for j in range(0, int(z)):
                        pins[x, y, j] = PinMappingData(
                            port_name=port_name,
                            mapped_pin=scalar_mapped_pins[i],
                            x=x,
                            y=y,
                            z=j
                        )
                        port_map[scalar_mapped_pins[i]] = pins[x, y, j]
                        i += 1
            else:
                for x in range(curr_startx, curr_endx - 1, -1):
                    for j in range(0, int(z)):
                        pins[x, y, j] = PinMappingData(
                            port_name=port_name,
                            mapped_pin=scalar_mapped_pins[i],
                            x=x,
                            y=y,
                            z=j
                        )
                        port_map[scalar_mapped_pins[i]] = pins[x, y, j]
                        i += 1
        elif starty is not None and endy is not None:
            curr_starty = int(starty)
            curr_endy = int(endy)
            x = io_col
            if curr_starty < curr_endy:
                for y in range(curr_starty, curr_endy + 1):
                    for j in range(0, int(z)):
                        pins[x, y, j] = PinMappingData(
                            port_name=port_name,
                            mapped_pin=scalar_mapped_pins[i],
                            x=x,
                            y=y,
                            z=j
                        )
                        port_map[scalar_mapped_pins[i]] = pins[x, y, j]
                        i += 1
            else:
                for y in range(curr_starty, curr_endy - 1, -1):
                    for j in range(0, int(z)):
                        pins[x, y, j] = PinMappingData(
                            port_name=port_name,
                            mapped_pin=scalar_mapped_pins[i],
                            x=x,
                            y=y,
                            z=j
                        )
                        port_map[scalar_mapped_pins[i]] = pins[x, y, j]
                        i += 1

    return pins, port_map


# =============================================================================


def vec_to_scalar(port_name):
    '''
    Converts given bus port into its scalar ports
    '''
    scalar_ports = []
    if port_name is not None and ':' in port_name:
        open_brace = port_name.find('[')
        close_brace = port_name.find(']')
        if open_brace == -1 or close_brace == -1:
            print(
                'Invalid portname "{}" specified. Bus ports should contain [ ] to specify range'
                .format(port_name),
                file=sys.stderr
            )
            sys.exit(1)
        bus = port_name[open_brace + 1:close_brace]
        lsb = int(bus[:bus.find(':')])
        msb = int(bus[bus.find(':') + 1:])
        if lsb > msb:
            for i in range(lsb, msb - 1, -1):
                curr_port_name = port_name[:open_brace] + '[' + str(i) + ']'
                scalar_ports.append(curr_port_name)
        else:
            for i in range(lsb, msb + 1):
                curr_port_name = port_name[:open_brace] + '[' + str(i) + ']'
                scalar_ports.append(curr_port_name)
    else:
        scalar_ports.append(port_name)

    return scalar_ports


# =============================================================================


def parse_io_cells(xml_root):
    """
    Parses the "IO" section of the pinmapfile. Returns a dict indexed by IO cell
    names which contains cell types and their locations in the device grid.
    """

    cells = {}
    port_map = {}

    width = xml_root.get("width"),
    height = xml_root.get("height"),
    io_per_cell = xml_root.get("z")

    # Get the "IO" section
    xml_io = xml_root.find("IO")
    if xml_io is None:
        print("ERROR: No mandatory 'IO' section defined in 'DEVICE' section")
        sys.exit(1)

    xml_top_io = xml_io.find("TOP_IO")
    if xml_top_io is not None:
        currcells, port_map = parse_io(
            xml_top_io, port_map, "TOP", width, height, io_per_cell
        )
        cells["TOP"] = currcells

    xml_bottom_io = xml_io.find("BOTTOM_IO")
    if xml_bottom_io is not None:
        currcells, port_map = parse_io(
            xml_bottom_io, port_map, "BOTTOM", width, height, io_per_cell
        )
        cells["BOTTOM"] = currcells

    xml_left_io = xml_io.find("LEFT_IO")
    if xml_left_io is not None:
        currcells, port_map = parse_io(
            xml_left_io, port_map, "LEFT", width, height, io_per_cell
        )
        cells["LEFT"] = currcells

    xml_right_io = xml_io.find("RIGHT_IO")
    if xml_right_io is not None:
        currcells, port_map = parse_io(
            xml_right_io, port_map, "RIGHT", width, height, io_per_cell
        )
        cells["RIGHT"] = currcells

    return cells, port_map


# ============================================================================


def read_pinmapfile_data(pinmapfile):
    """
    Loads and parses a pinmap file
    """

    # Read and parse the XML archfile
    parser = ET.XMLParser(resolve_entities=False, strip_cdata=False)
    xml_tree = ET.parse(pinmapfile, parser)
    xml_root = xml_tree.getroot()

    if xml_root.get("name") is None:
        print(
            "ERROR: No mandatory attribute 'name' specified in 'DEVICE' section"
        )
        sys.exit(1)

    if xml_root.get("family") is None:
        print(
            "ERROR: No mandatory attribute 'family' specified in 'DEVICE' section"
        )
        sys.exit(1)

    if xml_root.get("width") is None:
        print(
            "ERROR: No mandatory attribute 'width' specified in 'DEVICE' section"
        )
        sys.exit(1)

    if xml_root.get("height") is None:
        print(
            "ERROR: No mandatory attribute 'height' specified in 'DEVICE' section"
        )
        sys.exit(1)

    if xml_root.get("z") is None:
        print(
            "ERROR: No mandatory attribute 'z' specified in 'DEVICE' section"
        )
        sys.exit(1)

    # Parse IO cells
    io_cells, port_map = parse_io_cells(xml_root)

    return io_cells, port_map


# =============================================================================


def generate_pinmap_csv(pinmap_csv_file, io_cells):
    '''
    Generates pinmap csv file
    '''
    with open(pinmap_csv_file, "w", newline='') as csvfile:
        fieldnames = [
            'orientation', 'row', 'col', 'pin_num_in_cell', 'port_name',
            'mapped_pin', 'GPIO_type', 'Associated Clock', 'Clock Edge'
        ]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        writer.writeheader()
        for orientation, pin_map in io_cells.items():
            for pin_loc, pin_obj in pin_map.items():
                writer.writerow(
                    {
                        'orientation': orientation,
                        'row': str(pin_obj.y),
                        'col': str(pin_obj.x),
                        'pin_num_in_cell': str(pin_obj.z),
                        'port_name': pin_obj.mapped_pin,
                    }
                )


# =============================================================================


def main():
    '''
    Processes interface mapping xml file and generates template csv file
    '''
    # Parse arguments
    parser = argparse.ArgumentParser(
        description='Process interface mapping xml file to generate csv file.'
    )

    parser.add_argument(
        "--pinmapfile",
        "-p",
        "-P",
        type=str,
        required=True,
        help="Input pin-mapping XML file"
    )
    parser.add_argument(
        "--csv_file",
        "-c",
        "-C",
        type=str,
        default="template_pinmap.csv",
        help="Output template pinmap CSV file"
    )

    args = parser.parse_args()

    # Load all the necessary data from the pinmapfile
    io_cells, port_map = read_pinmapfile_data(args.pinmapfile)

    # Generate the pinmap CSV
    generate_pinmap_csv(args.csv_file, io_cells)


if __name__ == "__main__":
    main()
