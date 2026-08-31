# 2026-08-31T15:14:36.393818300
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter_test/hls_config.cfg")

cfg.set_value(section="hls", key="syn.compile.clang_version", value="16")

cfg.set_value(section="hls", key="syn.top", value="test")

comp = client.get_component(name="counter_test")
comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

vitis.dispose()

