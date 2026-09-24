# 2026-09-23T20:01:17.405259400
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

comp = client.get_component(name="generator")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp = client.get_component(name="stream_reader")
comp.run(operation="PACKAGE")

vitis.dispose()

