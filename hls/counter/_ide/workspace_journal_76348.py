# 2026-09-05T19:14:34.708396100
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.get_component(name="counter")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="MCS")

cfg.set_value(section="hls", key="syn.csimflags", value="")

cfg.set_value(section="hls", key="syn.cflags", value="")

cfg.set_values(key="syn.file_cflags", values=[])

cfg.set_values(key="syn.file", values=["counter.cpp", "counter.h"])

cfg.set_values(key="syn.file_csimflags", values=[])

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

vitis.dispose()

