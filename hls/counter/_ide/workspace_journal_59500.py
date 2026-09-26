# 2026-09-24T14:47:52.607670200
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.get_component(name="counter")
comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

comp = client.create_hls_component(name = "stream_reader",platform = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\counter\stream_reader\hls_config.cfg")

cfg.set_values(key="syn.file", values=["stream_reader.cpp"])

cfg.set_values(key="syn.file", values=["stream_reader.cpp", "stream_reader.h"])

cfg.set_values(key="tb.file", values=["strream_reader_test.cpp"])

comp = client.get_component(name="stream_reader")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/stream_reader/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="counter")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

cfg.set_value(section="hls", key="syn.top", value="stream_reader")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

vitis.dispose()

