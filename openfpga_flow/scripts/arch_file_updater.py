# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Script Name   : arch_file_updater.py
# Description   : This script designed to update architecture files
#                 from a lower version to higher version
# Author        : Xifan Tang
# Email         : xifan@osfpga.org
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
import os
from os.path import dirname, abspath
import argparse
import logging
import shutil
from xml.dom import minidom
from datetime import timedelta
import time
import datetime
import re

#####################################################################
# Error codes
#####################################################################
error_codes = {"SUCCESS": 0, "ERROR": 1, "OPTION_ERROR": 2, "FILE_ERROR": 3}

#####################################################################
# Initialize logger
#####################################################################
logging.basicConfig(format="%(levelname)s: %(message)s", level=logging.INFO)


#####################################################################
# Upgrade an architecture XML file from version 1.1 syntax to version 1.2
# Change rules:
# - Create a new child node of <sub_tile> under each node of <tile>
# - A <sub_tile> node herits the attribute 'name' of its parent <tile>
# - A <sub_tile> node herits the attribute 'capacity' of its parent <tile>
# - Any child nodes under a <tile> are moved to <sub_tile>
# - The attribute 'capacity' of parent <tile> is removed
#####################################################################
def convert_arch_xml_from_v1p1_to_v1p2(input_fname, output_fname):
    # Constants
    TILE_ROOT_TAG = "tiles"
    TILE_NODE_TAG = "tile"
    SUB_TILE_NODE_TAG = "sub_tile"
    NAME_TAG = "name"
    CAPACITY_TAG = "capacity"

    # Log runtime and status
    status = error_codes["SUCCESS"]
    start_time = time.time()

    log_str = "Converting '" + input_fname + "'" + " to " + "'" + output_fname + "'..."
    logging.info(log_str)
    # Parse the input file
    doc = minidom.parse(input_fname)

    # Iterate over <tile> nodes
    num_tile_roots = len(doc.getElementsByTagName(TILE_ROOT_TAG))
    if num_tile_roots != 1:
        logging.info("Found " + str(num_tile_roots) + " <" + TILE_ROOT_TAG + ">")
        logging.error(
            "Fail to find a require node (one and only one) <"
            + TILE_ROOT_TAG
            + "> under the root node!"
        )
        return error_codes["ERROR"]
    tile_root = doc.getElementsByTagName(TILE_ROOT_TAG)[0]
    for tile_node in tile_root.getElementsByTagName(TILE_NODE_TAG):
        # Create a new child node <sub_tile>
        sub_tile_node = doc.createElement(SUB_TILE_NODE_TAG)
        # Add attributes to the new child node
        sub_tile_node.setAttribute(NAME_TAG, tile_node.getAttribute(NAME_TAG))
        if tile_node.hasAttribute(CAPACITY_TAG):
            sub_tile_node.setAttribute(CAPACITY_TAG, tile_node.getAttribute(CAPACITY_TAG))
            # Delete the out-of-date attributes
            tile_node.removeAttribute(CAPACITY_TAG)
        # Move other subelements to the new child node
        for child in tile_node.childNodes:
            # Add the node to the child node
            child_clone = child.cloneNode(deep=True)
            sub_tile_node.appendChild(child_clone)
        # Remove no longer required child nodes
        while tile_node.hasChildNodes():
            tile_node.removeChild(tile_node.firstChild)
        # Append the sub tile child to the tile node
        tile_node.appendChild(sub_tile_node)

    # Output the modified content
    with open(output_fname, "w") as output_xml_f:
        doc.writexml(output_xml_f, indent="", addindent=" ", newl="")
    doc.unlink()

    # Finish up
    end_time = time.time()
    time_diff = timedelta(seconds=(end_time - start_time))
    log_end_str1 = "[Done]"
    log_end_str2 = " took " + str(time_diff)
    logging.info(
        log_end_str1 + "." * (len(log_str) - len(log_end_str1) - len(log_end_str2)) + log_end_str2
    )
    return status


#####################################################################
# Main function
#####################################################################
if __name__ == "__main__":
    # Execute when the module is not initialized from an import statement

    # Parse the options and apply sanity checks
    parser = argparse.ArgumentParser(
        description="Convert an architecture file from a lower version to a higher version"
    )
    parser.add_argument("--input_file", required=True, help="Path to input architecture file")
    parser.add_argument(
        "--output_file",
        default="converted_arch.xml",
        help="Path to output converted architecture file",
    )
    args = parser.parse_args()

    # Run conversion: from v1.1 syntax to v1.2 syntax
    exit(convert_arch_xml_from_v1p1_to_v1p2(args.input_file, args.output_file))
