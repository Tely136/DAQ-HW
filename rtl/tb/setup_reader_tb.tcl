# Run from the Vivado Tcl console with the simulation project open:
# source C:/Users/tely1/PhD/DAQ-HW/rtl/tb/setup_reader_tb.tcl
set tb_dir [file dirname [file normalize [info script]]]
set bd_file [get_files */design_1.bd]
if {[llength $bd_file] != 1} { error "Open the simulation project containing design_1.bd first" }
open_bd_design $bd_file
validate_bd_design
save_bd_design
generate_target all $bd_file
# Regenerate after the external ports were renamed (old wrapper used *_0).
set wrappers [make_wrapper -files $bd_file -top -force]
foreach wrapper $wrappers {
    if {[llength [get_files -quiet $wrapper]] == 0} { add_files -norecurse $wrapper }
}
set tb_file [file join $tb_dir reader_tb.sv]
if {[llength [get_files -quiet $tb_file]] == 0} {
    add_files -fileset sim_1 -norecurse $tb_file
}
set_property top reader_tb [get_filesets sim_1]
set_property xsim.simulate.runtime 0ns [get_filesets sim_1]
update_compile_order -fileset sim_1
puts "Ready: launch_simulation, then run all. Expected final counts: 3 and 5."
