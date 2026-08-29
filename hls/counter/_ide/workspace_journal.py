# 2026-08-28T08:55:26.490853100
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.create_hls_component(name = "counter",part = "xc7z007sclg400-1",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

comp = client.create_hls_component(name = "using_fixed_point",template = "vitis_hls_examples/Modeling/using_fixed_point")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\counter\counter\hls_config.cfg")

cfg.set_values(key="syn.file", values=["C:/Users/tely1/.Xilinx/Vitis/2026.1/vitis_hls_examples/Modeling/using_fixed_point/cpp_ap_fixed.cpp", "C:/Users/tely1/.Xilinx/Vitis/2026.1/vitis_hls_examples/Modeling/using_fixed_point/cpp_ap_fixed.h", "C:/Users/tely1/.Xilinx/Vitis/2026.1/vitis_hls_examples/Modeling/using_fixed_point/cpp_ap_fixed_test.cpp"])

comp = client.get_component(name="counter")
comp.run(operation="C_SIMULATION")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="cpp_ap_fixed")

comp.run(operation="SYNTHESIS")

comp = client.create_hls_component(name = "using_fixed_point",template = "vitis_hls_examples/Modeling/using_fixed_point")

client.sync_git_example_repo(name="vitis_hls_examples")

comp = client.create_hls_component(name = "using_fixed_point",template = "vitis_hls_examples/Modeling/using_fixed_point")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\counter\counter\hls_config.cfg")

cfg.set_values(key="syn.file", values=["C:/Users/tely1/.Xilinx/Vitis/2026.1/vitis_hls_examples/Modeling/using_fixed_point/cpp_ap_fixed.cpp", "C:/Users/tely1/.Xilinx/Vitis/2026.1/vitis_hls_examples/Modeling/using_fixed_point/cpp_ap_fixed.h"])

cfg.set_values(key="syn.blackbox.file", values=[])

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp.run(operation="IMPLEMENTATION")

client.delete_component(name="counter")

vitis.dispose()

