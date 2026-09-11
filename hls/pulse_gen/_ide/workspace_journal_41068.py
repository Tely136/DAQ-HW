# 2026-09-09T14:19:20.405149100
import vitis

client = vitis.create_client()
client.set_workspace(path="pulse_gen")

comp = client.create_hls_component(name = "pulse_generator",part = "xc7a35tcpg236-1",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\pulse_gen\pulse_generator\hls_config.cfg")

cfg.set_values(key="syn.file", values=["pulse_generator.cpp"])

cfg.set_values(key="syn.file", values=["pulse_generator.cpp", "pulse_generator.h"])

cfg.set_values(key="tb.file", values=["pulse_generator_test.cpp"])

comp = client.get_component(name="pulse_generator")
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

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/pulse_gen/pulse_generator/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="pulse_generator")

vitis.dispose()

vitis.dispose()

