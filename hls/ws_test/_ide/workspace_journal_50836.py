# 2026-10-11T09:47:39.243509100
import vitis

client = vitis.create_client()
client.set_workspace(path="ws_test")

comp = client.create_hls_component(name = "stream_generator",platform = "$COMPONENT_LOCATION/../../../hardware/counter_wrapper.xsa",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="C:\Users\tely1\PhD\DAQ-HW\hls\ws_test\stream_generator\hls_config.cfg")

cfg.set_values(key="syn.file", values=["C:/Users/tely1/PhD/DAQ-HW/hls/stream_generator/stream_generator.cpp", "C:/Users/tely1/PhD/DAQ-HW/hls/stream_generator/stream_generator.h"])

cfg.set_values(key="tb.file", values=["C:/Users/tely1/PhD/DAQ-HW/hls/stream_generator/stream_generator_test.cpp"])

cfg.set_values(key="syn.file", values=["C:/Users/tely1/PhD/DAQ-HW/hls/stream_generator/stream_generator.cpp", "C:/Users/tely1/PhD/DAQ-HW/hls/stream_generator/stream_generator.h", "test.cpp"])

cfg = client.get_config_file(path="/c:/Users/tely1/PhD/DAQ-HW/hls/ws_test/stream_generator/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="stream_generator")

comp = client.get_component(name="stream_generator")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

cfg = client.get_config_file(path="C:\Users\tely1\PhD\DAQ-HW\hls\ws_test\stream_generator\hls_config.cfg")

cfg.set_values(key="syn.file", values=["C:/Users/tely1/PhD/DAQ-HW/hls/stream_generator/stream_generator.cpp", "C:/Users/tely1/PhD/DAQ-HW/hls/stream_generator/stream_generator.h"])

cfg.set_values(key="syn.blackbox.file", values=[])

comp.run(operation="PACKAGE")

comp.run(operation="IMPLEMENTATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

comp.run(operation="IMPLEMENTATION")

vitis.dispose()

