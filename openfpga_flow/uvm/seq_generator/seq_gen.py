import os
import argparse
import string
import fileinput
import sys
import re

#Command example : python seq_gen.py -f and2_autocheck_top_tb.v

#parser = argparse.ArgumentParser(description="create new python script from a template.")
#parser.add_argument(
#	"-f",
#	"--filename",
#	help="the name of the tb file in which the bitstream is stored",
#	default="test"
#)
#args=parser.parse_args()

#Adapt the path here to the testbench bs_agent directory you're using
#For the sequence
seq_lib_path="../tb/bs_agent/seq_lib/"
name="prog_seq_generated.sv"
sequence = seq_lib_path + name
#sequence = "./seq.sv"
filename = "../../SRC/and2_autocheck_top_tb.v"
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

#Source file to adapt the testbench parameters ( scalability )
uvm_pkg_template = "uvm_pkg.py"
uvm_pkg = "../tb/uvm_tb.sv"

#String used to extract parameters from the verilog testbench
start1 = "wire [0:"
end1 = "] gfpga_pad_iopad_pad;"
start2= "reg  [0:"
end2= "] address;"
assign_statement = "assign gfpga_pad_iopad_pad["
end_assign = "] ="
prog_statement = "		prog_cycle_task("

# Extract function takes out the programmation out of an autochecking testbench generated with yosys/vpr
# Then write the programming sequence to the sequence placeholder
def extract(file,startExp,endExp):
	results = io_seeker()
	ie_value = results[0]
	gpio_width = results[1]
	index = 0                                
	with open (filename, 'rt') as myfile:
		with open(sequence, "w") as output:
			output.write(template)    
			for line in myfile:
				if startExp in line:
					index = 1
					addr_value = line[line.find(prog_statement)+len(prog_statement):line.rfind(",")]
					data_value = line[line.find(",")+len(","):line.rfind(");")]
				if endExp in line:
					if index ==1:
						index = 0
						output.write("		`uvm_do_with(req,{req.address == " + addr_value + "; req.data_in ==" + data_value + "; req.IE == " + str(gpio_width) + "'b" + ie_value + ";})")
						#output.write("\n		`uvm_do_with(req,{req.address == 16'b0000000000000000; req.data_in == 1'b0; req.IE == 64'hFFFFFFFFFFFDFFFF;})") # Temporary solution
						output.write("\nendtask;")
						output.write("\n\n`endif")
				if index !=0:                   
					output.write(line)


# Get GPIO_WIDTH & ADDR_WIDTH from verilog TB.

def extract_parameters(file):
	global gpio_width
	with open (file, 'rt') as myfile:
		for line in myfile:
			if end1 in line:
				gpio_width = line[line.find(start1)+len(start1):line.rfind(end1)]
			if end2 in line:
				addr_width = line[line.find(start2)+len(start2):line.rfind(end2)]
	print("parameter gpio_width =" + gpio_width)
	print("parameter addr_width =" + addr_width)
	return gpio_width
#	pkg_generator(gpio_width,addr_width)

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




def io_seeker():
#	global ie_value
#	global gpio_width
	global results
	results = [0] * 2
	ie_value = ""
	gpio_widt = extract_parameters(filename)
	gpio_width = int(gpio_widt) + 1
	IE = [0] * gpio_width
	final_config = ""
	with open (filename, 'rt') as myfile:
		for line in myfile:
			for i in range(gpio_width):
				if assign_statement + str(i) + end_assign in line:
					IE[i] = 1
	for i in range(gpio_width):	
		ie_value = ie_value + str(IE[i])
	print(ie_value)
	results[0] = ie_value
	results[1] = gpio_width
#	return ie_value
#	return gpio_width
	return results
					



extract(filename,"prog_cycle_task(","		@(negedge prog_clock[0]);")
#extract_parameters(filename)
#io_seeker()
