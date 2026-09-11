# 2026-09-05T11:44:36.724484300
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

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg.set_value(section="hls", key="tb.cflags", value="")

cfg.set_value(section="hls", key="syn.csimflags", value="")

cfg.set_values(key="tb.file_cflags", values=[])

cfg.set_values(key="tb.file", values=["./counter_test.cpp", "./photon_stream.dat", "./golden.dat"])

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

cfg.set_value(section="hls", key="clock", value="1GHz")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

vitis.dispose()

