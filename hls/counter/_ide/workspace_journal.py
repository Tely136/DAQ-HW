# 2026-09-14T14:09:33.769030900
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.get_component(name="counter")
comp.run(operation="SYNTHESIS")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg.set_value(section="hls", key="clock", value="10ns")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp.run(operation="SYNTHESIS")

vitis.dispose()

