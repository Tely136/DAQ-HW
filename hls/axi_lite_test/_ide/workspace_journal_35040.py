# 2026-09-08T20:44:12.127093100
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

comp = client.create_hls_component(name = "led_blinker",platform = "$COMPONENT_LOCATION/../../../cora-base/cora_base_wrapper.xsa",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\axi_lite_test\led_blinker\hls_config.cfg")

cfg.set_values(key="syn.file", values=["blinker.cpp"])

cfg.set_values(key="tb.file", values=["blinker_test.cpp"])

cfg.set_values(key="syn.file", values=["blinker.cpp", "blinker.h"])

comp = client.get_component(name="led_blinker")
comp.run(operation="SYNTHESIS")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/axi_lite_test/led_blinker/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="blinker")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

vitis.dispose()

