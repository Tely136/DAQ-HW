# 2026-09-05T08:00:58.440731900
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.get_component(name="counter")
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

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg.set_values(key="tb.file", values=["./counter_test.cpp", "./photon_stream.dat"])

cfg.set_value(section="hls", key="tb.cflags", value="")

cfg.set_values(key="tb.file_cflags", values=[])

cfg.set_value(section="hls", key="syn.csimflags", value="")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

vitis.dispose()

