# 2026-09-23T17:19:48.059036900
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="ram_write_with_ip")
comp.build()

comp = client.create_hls_component(name = "generator",platform = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\axi_lite_test\generator\hls_config.cfg")

cfg.set_values(key="syn.file", values=["generator.cpp"])

cfg.set_values(key="tb.file", values=["generator_test.cpp"])

comp = client.get_component(name="generator")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/axi_lite_test/generator/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="generator")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\axi_lite_test\generator\hls_config.cfg")

cfg.set_values(key="syn.file", values=["generator.cpp", "generator.h"])

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp = client.create_hls_component(name = "stream_reader",platform = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\axi_lite_test\stream_reader\hls_config.cfg")

cfg.set_values(key="syn.file", values=["stream_reader.cpp"])

cfg.set_values(key="syn.file", values=["stream_reader.cpp", "stream_reader.h"])

cfg.set_values(key="tb.file", values=["stream_reader_test.h"])

cfg.set_values(key="tb.file", values=["stream_reader_test.cpp"])

comp = client.get_component(name="stream_reader")
comp.run(operation="C_SIMULATION")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/axi_lite_test/stream_reader/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="reader")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

vitis.dispose()

