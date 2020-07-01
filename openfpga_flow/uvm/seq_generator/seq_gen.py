import os
import argparse
import string
import fileinput
import sys
import re

#Command example : python seq_gen.py -s simulation_deck.ini -b prog.bit

parser = argparse.ArgumentParser(description="create new python script from a template.")
parser.add_argument(
	"-s",
	"--simulation_deck",
	help="the name of the simulation deck file in which parameters are stored",
	default="test"
)
parser.add_argument(
	"-b",
	"--prog",
	help="The name of the programmation file in which the fpga config is stored",
	default="test"
)
args=parser.parse_args()
sim_file = args.simulation_deck
prog_file = args.prog

gpio_start = "GPIO_WIDTH = "
addr_start = "ADDR_WIDTH = "
io_start = "IO = "
#Source file to adapt the testbench parameters ( scalability )
uvm_pkg_template = "uvm_pkg.py"
uvm_pkg = "./uvm_tb.sv"

#Template and param to build uvm seq
template = '''`ifndef "prog_seq_gen"
`define "prog_seq_gen"


class prog_seq_gen extends bs_base_seq;
   `uvm_object_utils(prog_seq_gen)
   
   /**
    * Default constructor.
    */
   extern function new(string name="prog_seq_gen");
   /**
    * Sequence body
    */
   extern virtual task body();

endclass : prog_seq_gen

function prog_seq_gen::new(string name = "prog_seq_gen");

      super.new(name);

endfunction // new

task prog_seq_gen::body();
'''

sequence = "./seq.sv"
cycle_task ="		prog_cycle_task("


def extract_parameters(file):
	with open (file, 'rt') as myfile:
		for line in myfile:
			if gpio_start in line:
				gpio_width = line[line.find(gpio_start)+len(gpio_start):line.rfind("")]
			if addr_start in line:
				addr_width = line[line.find(addr_start)+len(addr_start):line.rfind("")]
			if io_start in line:
				io_value = line[line.find(io_start)+len(io_start):line.rfind("")]
	print("parameter gpio_width =" + gpio_width)
	print("parameter addr_width =" + addr_width)
	pkg_generator(gpio_width,addr_width)
	extract(prog_file,gpio_width,io_value,addr_width)

# Modify parameters in the UVM testbench
def pkg_generator(gpio_width,addr_width):
    with open(uvm_pkg_template,"r") as f:
	template_text = f.read()
    data = {
    "gpio_width": gpio_width,
    "addr_width": addr_width
    }

    template = string.Template(template_text)
    result = template.substitute(data)
    with open(uvm_pkg, "w") as f:
	f.write(result)

def extract(file,gpio_width,io_value,addr_width):       
	with open (file, 'rt') as myfile:
		with open(sequence, "w") as output:
			output.write(template)    
			for line in myfile:
					addr_value = line[line.find("")+len(""):line.rfind(" ")]
					data_value = line[line.find(" ")+len(" "):line.rfind("")]
					final_line = cycle_task + addr_width[:-1] +"'b" + addr_value + ", 1'b" + data_value[:-1] + ");\n"
					output.write(final_line)
			output.write("		`uvm_do_with(req,{req.address == " + addr_width[:-1] +"'b" + addr_value + "; req.data_in ==" + data_value[:-1] + "; req.IE == " + gpio_width[:-1] + "'b" + io_value[:-1] + ";})")
			output.write("\nendtask;")
			output.write("\n\n`endif")                   
					

extract_parameters(sim_file)
