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
import xml.etree.ElementTree as ET

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
  logging.info("Converting \'" + input_fname + "\'" + " to " + "\'" + output_fname + "\'")
  # Parse the input file
  tree = ET.parse(input_fname)
  root = tree.getroot() 

  # Iterate over <tile> nodes
  tile_root_count = 0
  for tile_root in root.iter("tiles"):
    tile_root_count += 1
  if (tile_root_count != 1):
  logging.error("Fail to find a require node (one and only one) <tiles> under the root node!")
    

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
