# Examples_README

## Example_1

The goal of this example is just to make a first step into the software. The .blif contains only an inverter.
The .xml is currently on <layout auto="1.0"/> which means that the size depends on the .blif. Since the .blif is 
almost empty, only 1 CLB will be generated. 

![alt text](https://github.com/LNIS-Projects/OpenFPGA/blob/master/examples/figures/example_1.png "Example_1_FPGA")

Schematic of the FPGA generated during example_1.
The CLB integrates a 4-inputs LUT, a FF and a MUX. 

---

**Things to understand in this example**

Everything won't be explained in detail but few important structures (some common with the VPR project) are to be explained in order to build good architectures.

```xml
<architecture>
	<models>
		... add models such as the io pads.
	</models>
	<spice_settings>
		... all tech and spice parameters are defined here.
		<module_spice_models>
			... define the Basic Elements of the architecture and the modules that cannot 
			be generated (i.e. the Flip-Flop) but need to be called.
		</module_spice_models>
	</spice_settings>
	<cblocks>
		... complex blocks
		<complexblocklist>
			... here we define the hierarchy of the primitive blocks and interconnect them 
			together
			<pb_type>
				... defines the primitive block
			</pb_type>
			<interconnect>
				...
			</interconnect>
		</complexblocklist>
	</cblocks>
</architecture>
```


## Example_2

Example_2's goal is to introduce the slices, the interconnections which can be generated from it and the manual mode of the layout.
In this case, we generate a 3x3 FPGA with 4 slices. The LUTs are 6-inputs ones similarly to the ones used in the industry.
There is a feedbeck-loop from the output of the slices to the input MUXs

![alt text](https://github.com/LNIS-Projects/OpenFPGA/blob/master/examples/figures/example_2_the_CLB.png "Example_2_CLB")

Schematic showing the CLB generated in this example.

---

**Things to understand in this example**

```xml
<layout width="3" height="3"/> <!-- Manual mode of the layout allowing us to choose the number of CLBs -->

<complete name="crossbar" input="clb.I fle[3:0].out" output="fle[3:0].in" spice_model_name="mux_2level"> 
	<!-- Defines how we apply the feedback on the inputs of the slices -->
	<delay_constant max="53.44e-12" in_port="clb.I" out_port="fle[3:0].in" />
	<delay_constant max="53.44e-12" in_port="fle[3:0].out" out_port="fle[3:0].in" />
</complete>
```
 



