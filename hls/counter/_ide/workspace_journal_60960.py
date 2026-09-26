# 2026-09-24T13:25:52.179729900
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.create_hls_component(name = "stream_generator",platform = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\counter\stream_generator\hls_config.cfg")

cfg.set_values(key="syn.file", values=["stream_generator.cpp"])

cfg.set_values(key="syn.file", values=["stream_generator.cpp", "stream_generator.h"])

cfg.set_values(key="tb.file", values=["stream_generator_test.cpp"])

comp = client.get_component(name="stream_generator")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/stream_generator/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="stream_generator")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

comp = client.get_component(name="counter")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="counter")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

vitis.dispose()

