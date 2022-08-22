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
import xml.etree.ElementTree as etree

#####################################################################
# Error codes
#####################################################################
error_codes = {
  "SUCCESS": 0,
  "ERROR": 1, 
  "OPTION_ERROR": 2, 
  "FILE_ERROR": 3 
}

#####################################################################
# Initialize logger 
#####################################################################
logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO);

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
  NAME_TAG = "capacity" 
  CAPACITY_TAG = "capacity" 

  logging.info("Converting \'" + input_fname + "\'" + " to " + "\'" + output_fname + "\'")
  # Parse the input file
  tree = etree.parse(input_fname)
  root = tree.getroot() 

  # Iterate over <tile> nodes
  if (root.findall(TILE_ROOT_TAG) != 1):
    logging.error("Fail to find a require node (one and only one) <" + TILE_ROOT_TAG + "> under the root node!")
  tile_root = root.find(TILE_ROOT_TAG)
  for tile_node in tile_root.iter(TILE_NODE_TAG): 
    # Create a new child node <sub_tile>
    sub_tile_node = etree.SubElement(tile_node, SUB_TILE_NODE_TAG)
    # Add attributes to the new child node
    sub_tile_node.set(NAME_TAG, tile_node.get(NAME_TAG))
    if tile_node.get(CAPACITY_TAG) is not None:
      sub_tile_node.set(CAPACITY_TAG, tile_node.get(CAPACITY_TAG))
    # Move other subelements to the new child node
    for child in tile_node:
      # Bypass new node
      if (child.tag == SUB_TILE_NODE_TAG):
        continue
      # Add the node to the child node
      sub_tile_node.append(child)
    # Delete the out-of-date attributes
    tile_node.pop(CAPACITY_TAG)
    tile_node.SubElement(

  logging.info("[Done]")


#####################################################################
# Main function
#####################################################################
if __name__ == '__main__':
  # Execute when the module is not initialized from an import statement

  # Parse the options and apply sanity checks
  parser = argparse.ArgumentParser(description='Convert an architecture file from a lower version to a higher version')
  parser.add_argument('--input_file',
                      required=True,
                      help='Path to input architecture file')
  parser.add_argument('--output_file',
                      default="converted_arch.xml",
                      help='Path to output converted architecture file')
  args = parser.parse_args()

  # Run conversion: from v1.1 syntax to v1.2 syntax
  convert_arch_xml_from_v1p1_to_v1p2(args.input_file, args.output_file)
