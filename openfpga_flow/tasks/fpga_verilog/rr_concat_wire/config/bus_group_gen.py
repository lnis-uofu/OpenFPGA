#####################################################################
# A script to create a bus group file based on an input verilog file
# The bug group file is an input required by OpenFPGA
#####################################################################
import os
from os.path import dirname, abspath
import argparse
import logging
import subprocess
import hashlib
import yaml
import pyverilog
from pyverilog.dataflow.dataflow_analyzer import VerilogDataflowAnalyzer
from xml.dom import minidom

#####################################################################
# Error codes
#####################################################################
error_codes = {
  "SUCCESS": 0,
  "MD5_ERROR": 1, 
  "OPTION_ERROR": 2, 
  "FILE_ERROR": 3 
}

#####################################################################
# Initialize logger 
#####################################################################
logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO);

#####################################################################
# Generate the string for a Verilog port
#####################################################################
def gen_verilog_port_str(port_name, msb, lsb):
  port_str = str(port_name) + "[" + str(msb) + ":" + str(lsb) + "]"
  return port_str 

#####################################################################
# Generate the string for a flatten Verilog port
#####################################################################
def gen_flatten_verilog_port_str(port_name, pin_id):
  port_str = str(port_name) + "_" + str(pin_id) + "_"
  return port_str 

#####################################################################
# Parse a verilog file and collect bus port information
#####################################################################
def parse_verilog_file_bus_ports(verilog_files, top_module):
  # Check if verilog file exists
  verilog_file_list = []
  for verilog_f in verilog_files:
    print(verilog_f)
    verilog_f_abspath = os.path.abspath(verilog_f["name"])
    if not os.path.exists(verilog_f_abspath):
      raise IOError("file not found: " + verilog_f_abspath)

    verilog_file_list.append(verilog_f_abspath)

  # Parse verilog file
  analyzer = VerilogDataflowAnalyzer(verilog_file_list, top_module)
  analyzer.generate()
  # Get port information
  terms = analyzer.getTerms()
  # Create XML tree
  xml = minidom.Document()
  bus_group = xml.createElement("bus_group")
  xml.appendChild(bus_group)
  for tk, tv in sorted(terms.items(), key=lambda x: str(x[0])):
    logging.debug(tv.name)
    logging.debug(tv.termtype)
    logging.debug("[" + str(tv.lsb) + ":" + str(tv.msb) + "]")
  for tk, tv in sorted(terms.items(), key=lambda x: str(x[0])):
    # Skip ports that do not belong to top module
    if (top_module != str(tv.name).split(".")[-2]):
      continue
    port_name = str(tv.name).split(".")[-1]
    
    # Skip minus lsb or msb, which are in don't care set
    if (("Minus" == str(tv.lsb)) or ("Minus" == str(tv.msb))):
      continue
    port_lsb = int(str(tv.lsb))
    port_msb = int(str(tv.msb))
    # Only care input and outports 
    if ((not ("Input" in tv.termtype)) and (not ("Output" in tv.termtype))):
      continue
    # Only care bus (msb - lsb > 0)
    if (abs(port_lsb - port_msb) == 0):
      continue
    # Reaching here, this is a bus port we need
    # Get the second part of the name, which is the port name
    cur_bus = xml.createElement("bus")
    cur_bus.setAttribute("name", gen_verilog_port_str(port_name, port_msb, port_lsb)) 
    # Get if this is little endian or not
    cur_bus.setAttribute("big_endian", "false")
    if (port_lsb > port_msb):
      cur_bus.setAttribute("big_endian", "true")
    bus_group.appendChild(cur_bus)
    # Add all the pins
    for ipin in range(min([port_msb, port_lsb]), max([port_msb, port_lsb]) + 1):
      cur_pin = xml.createElement("pin")
      cur_pin.setAttribute('id', str(ipin))
      cur_pin.setAttribute('name', gen_flatten_verilog_port_str(port_name, ipin))
      cur_bus.appendChild(cur_pin)

  return xml, bus_group

#####################################################################
# Generate bus group files with a given task list
#####################################################################
def generate_bus_group_files(task_db):
  # Iterate over all the tasks
  for verilog_fname in task_db.keys():
    space_limit = 120
    log_str = "Parsing verilog file: "
    top_module_name = task_db[verilog_fname]["top_module"]
    logging_space = "." * (space_limit - len(log_str) - len(top_module_name))
    logging.info(log_str + top_module_name) 
    xml, bus_group_data = parse_verilog_file_bus_ports(task_db[verilog_fname]["source"], top_module_name)
    logging.info(log_str + top_module_name + logging_space + "Done") 
    # Write bus ports to an XML file
    bus_group_frelname = task_db[verilog_fname]["bus_group_file"]
    bus_group_fname = os.path.abspath(bus_group_frelname)
    log_str = "Writing bus group file:"
    logging_space = "." * (space_limit - len(log_str) - len(bus_group_frelname))
    logging.info(log_str + bus_group_frelname) 
    xml_str = xml.toprettyxml(indent="\t")
    with open(bus_group_fname, "w") as bus_group_f:
      bus_group_f.write(xml_str)
    logging.info(log_str + bus_group_frelname + logging_space + "Done") 

#####################################################################
# Read task list from a yaml file
#####################################################################
def read_yaml_to_task_database(yaml_filename):
  task_db = {}
  with open(yaml_filename, 'r') as stream:
    try:
      task_db = yaml.load(stream, Loader=yaml.FullLoader)
      logging.info("Found " + str(len(task_db)) + " tasks to create symbolic links")
    except yaml.YAMLError as exc:
      logging.error(exc)
      exit(error_codes["FILE_ERROR"]);

  return task_db

#####################################################################
# Write result database to a yaml file
#####################################################################
def write_result_database_to_yaml(result_db, yaml_filename):
  with open(yaml_filename, 'w') as yaml_file:
    yaml.dump(result_db, yaml_file, default_flow_style=False)

#####################################################################
# Main function
#####################################################################
if __name__ == '__main__':
  # Execute when the module is not initialized from an import statement

  # Parse the options and apply sanity checks
  parser = argparse.ArgumentParser(description='Create bus group files for Verilog inputs')
  parser.add_argument('--task_list',
                      required=True,
                      help='Configuration file in YAML format which contains a list of input Verilog and output bus group files')
  args = parser.parse_args()

  # Create a database for tasks
  task_db = {}
  task_db = read_yaml_to_task_database(args.task_list)

  # Generate links based on the task list in database
  generate_bus_group_files(task_db)
  logging.info("Created " + str(len(task_db)) + " bus group files")
  exit(error_codes["SUCCESS"])
