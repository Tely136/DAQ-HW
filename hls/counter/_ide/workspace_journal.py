# 2026-09-03T09:16:39.768493100
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

client.delete_component(name="counter_test_app")

client.delete_component(name="componentName")

comp = client.create_hls_component(name = "counter",platform = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\counter\counter\hls_config.cfg")

cfg.set_values(key="syn.file", values=["counter.cpp"])

cfg.set_values(key="syn.file", values=["counter.cpp", "counter_test.cpp"])

cfg.set_values(key="syn.file", values=["counter.cpp", "counter.h"])

comp = client.get_component(name="counter")
comp.run(operation="SYNTHESIS")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="test")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

client.delete_component(name="counter_test")

