# 2026-08-29T19:47:52.035097100
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

status = client.add_local_example_repo(name="hls_intro_examples_te", display_name="hls_intro_examples_te", description="Examples Repository.", type="HLS", local_directory="c:\Users\tely1\.Xilinx\Vitis\2026.1\Vitis-HLS-Introductory-Examples")

client.sync_git_example_repo(name="hls_intro_examples_te")

comp = client.create_hls_component(name = "using_fixed_point",template = "hls_intro_examples_te/Modeling/using_fixed_point")

client.sync_git_example_repo(name="hls_intro_examples_te")

comp = client.create_hls_component(name = "using_fixed_point",template = "hls_intro_examples_te/Modeling/using_fixed_point")

comp = client.create_hls_component(name = "using_fixed_point",template = "hls_intro_examples_te/Modeling/using_fixed_point")

status = client.delete_example_repo(name="hls_intro_examples_te")

comp = client.create_hls_component(name = "fixed_point_example",part = "xc7z007sclg400-1",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/fixed_point_example/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="cpp_ap_fixed")

comp = client.get_component(name="fixed_point_example")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

comp.run(operation="IMPLEMENTATION")

client.delete_component(name="fixed_point_example")

comp = client.create_hls_component(name = "counter_test",part = "xc7z007sclg400-1",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

vitis.dispose()

