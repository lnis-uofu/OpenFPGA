foreach i (*.openfpga)
    sed -i 's/--include_timing --include_signal_init --support_icarus_simulator/--include_timing/g' $i
    sed -i 's/simulation_deck\.ini/simulation_deck\.ini --include_signal_init --support_icarus_simulator/g' $i
end
