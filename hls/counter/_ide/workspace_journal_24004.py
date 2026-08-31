# 2026-08-31T13:45:01.144601900
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\counter\counter_test\hls_config.cfg")

cfg.set_values(key="syn.file", values=["counter_test.cpp", "counter_test.h"])

comp = client.get_component(name="counter_test")
comp.run(operation="SYNTHESIS")

vitis.dispose()

