

#Then we define the different tcl procedures we'll use.
proc create_project {} {
	global vars
	#Go back to the modelsim folder to create the project
	cd $vars(modelsim_project_path)
#Modified modelsim.ini file to be able to use uvm1.2
	set initfile $vars(questa_modelsim_init_path)/modelsim.ini
	exec mkdir -p $vars(modelsim_project_path)/$vars(projectname_modelsim)
	project new $vars(modelsim_project_path)/$vars(projectname_modelsim) $vars(projectname_modelsim) $vars(projectname_modelsim) $initfile 0
}


proc create_project_with_close {} {
	#Get the current project name
	set project_env [project env]
	if {$project_env ne ""} {
		#If string not empty (a project is loaded so close it first)
		project close
		}
	create_project
}


proc add_files_project {} {
	global vars
	#Get the length of the list
	set listlength [llength $vars(verilog_files)]
	#Add the verilog files one by one
	for {set x 0} {$x<$listlength} {incr x} {
		project addfile [lindex $vars(verilog_files) $x]
		}
}



proc simulate {} {
	global vars
	#Start the simulation	
	if {$vars(mapped) == "yes"} {
		vsim $vars(projectname_modelsim).$vars(module_tb_name)_opt -sdftyp /$vars(module_tb_name)/$vars(module_name)=$vars(sdf_file) -sdfnoerror
    #vsim $vars(projectname_modelsim).$vars(module_tb_name)_opt -suppress 3009
	} else {
		vsim $vars(projectname_modelsim).$vars(module_tb_name)_opt -suppress 3009
	}
}

proc simulate_genmode {} {
	global vars
	#Start the simulation	
	if {$vars(mapped) == "yes"} {
		vsim $vars(projectname_modelsim).$vars(module_tb_name)_opt -sdftyp /$vars(module_tb_name)/$vars(module_name)=$vars(sdf_file) -sdfnoerror
    #vsim $vars(projectname_modelsim).$vars(module_tb_name)_opt -suppress 3009
	} else {
		vsim $vars(projectname_modelsim).$vars(module_tb_name)_opt +UVM_TESTNAME=benchmark_gen_test -suppress 3009
	}
}

proc copy_src_to_project {} {
	global vars
	exec cp -rf ../../SRC .
}

proc top_create_new_project {} {
	global vars
	#Create the project
	create_project_with_close
	copy_src_to_project
	#Add the verilog files (for debuging purpose)
	add_files_project
	compile_opt $vars(verilog_files)
	#simulate
	#Add the waves	
	#add wave -position insertpoint sim:/$vars(module_tb_name)/*
	#run the simulation
	#run $vars(simtime) $vars(unit)
	#Fit the window view
	#wave zoom full
	}

proc compile_opt {verilog_files} {
	global vars
	#set files [project filenames]
	#project compileall
  	foreach current_file $verilog_files {
				vlog $current_file -sv
			}
	#Optimizing the design, while keeping full visibility for debugging
	vopt +acc $vars(projectname_modelsim).$vars(module_tb_name) -o $vars(module_tb_name)_opt
}



proc top_rerun_sim {} {
	global vars
	compile_opt $vars(verilog_files_recompile)
  restart -force
	run $vars(simtime) $vars(unit)
	#Fit the window view
	wave zoom full
}

proc top_run_sim {} {
	global vars
	simulate
	#run the simulation
	add_waves
	#add wave -position insertpoint sim:/$vars(module_tb_name)/*
	run $vars(simtime) $vars(unit)
	#Fit the window view
	wave zoom full
}


proc add_waves {} {
	global vars
	add wave -position insertpoint sim:/$vars(module_tb_name)/*
}




####OLD FUNCTIONS####
proc read_saif {} {
	global vars
  set vars(signals) $vars(module_tb_name)/$vars(fifo_instanciated_name)/*
  restart -force
	#power off
	run $vars(read_saif_start) ns
	#add_wves
	power add -ports $vars(signals)
	#power reset $vars(signals)
	power on -ports $vars(signals)
	run 0.5 ns
	power report -file $vars(saif_path)/$vars(projectname_modelsim)_read.rpt -all -bsaif $vars(saif_path)/$vars(projectname_modelsim)_read.saif
  power report
	#power off
}


proc write_saif {} {
	global vars
  set vars(signals) $vars(module_tb_name)/fifo_inst/*
  restart -force
	#power off
	run 17 ns
	#add_waves
	power add $vars(signals)
	#power reset $vars(signals)
	power on $vars(signals)
	run 2 ns
	power report -file $vars(saif_path)/$vars(projectname_modelsim)_write.rpt -all -bsaif $vars(saif_path)/$vars(projectname_modelsim)_write.saif
	#power off
}


proc compile_file {current_file} {
	global vars
		if {$vars(vlog_options) ne ""} {
	vlog $current_file -sv +define+OVL_ASSERT_ON +define+ABV +define+OVL_SVA +define+BD_XPROP_NS_CLK_SOME +define+assertions +incdir+/proj/ch_func_gen_scratch0/VDCI_LIB/egiacomi/_proj_exa_sim_users_egiacomi_VDCI3_$vars(workspace)/build/$vars(build_name)/fake_v_incl +incdir+/proj/verif_release_ro/uvmkit-1.2/4.0.1/uvm/src +incdir+/proj/ch_func_gen_scratch0/VDCI_LIB/egiacomi/_proj_exa_sim_users_egiacomi_VDCI3_$vars(workspace)/build/$vars(build_name) -suppress 2244 -svext=+sceq -suppress 2735
		} else {
			vlog $current_file
		}
	vopt +acc $vars(projectname_modelsim).$vars(module_tb_name) -o $vars(module_tb_name)_opt
}

proc add_waves_old {} {
	global vars
	config wave -signalnamewidth 1
	add wave -divider Global_signals
	add wave -position insertpoint sim:/$vars(module_tb_name)/clk_put
	add wave -position insertpoint sim:/$vars(module_tb_name)/clk_get
	add wave -position insertpoint sim:/$vars(module_tb_name)/rst

	if {$vars(predictive_async) == "yes"} {
		add wave -divider Predictive
		add wave -position insertpoint sim:/$vars(module_tb_name)/master_freqAdj
		add wave -position insertpoint sim:/$vars(module_tb_name)/slave_freqAdj
		add wave -position insertpoint sim:/$vars(module_tb_name)/wla
		add wave -position insertpoint sim:/$vars(module_tb_name)/rla
	}

	add wave -divider Data
	add wave -position insertpoint sim:/$vars(module_tb_name)/datain
	add wave -position insertpoint sim:/$vars(module_tb_name)/dataout
	if {$vars(original_async) == "yes"} {
		add wave -position insertpoint sim:/$vars(module_tb_name)/dataout_original
	}

	add wave -divider Ctrl
	add wave -position insertpoint sim:/$vars(module_tb_name)/req_put
	add wave -position insertpoint sim:/$vars(module_tb_name)/req_get
	add wave -position insertpoint sim:/$vars(module_tb_name)/spaceav
	add wave -position insertpoint sim:/$vars(module_tb_name)/datav


	if {$vars(original_async) == "yes"} {
		add wave -divider Ctrl_Original
		add wave -position insertpoint sim:/$vars(module_tb_name)/req_put
		add wave -position insertpoint sim:/$vars(module_tb_name)/req_get_original
		add wave -position insertpoint sim:/$vars(module_tb_name)/spaceav_original
		add wave -position insertpoint sim:/$vars(module_tb_name)/datav_original
	}

	if {$vars(mapped) == "no"} {
		add wave -divider Debug
		add wave -position insertpoint sim:/$vars(module_tb_name)/$vars(fifo_instanciated_name)/data_storage
	}
}
