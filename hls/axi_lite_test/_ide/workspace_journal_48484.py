# 2026-09-24T10:53:50.592172900
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

comp = client.get_component(name="stream_reader")
comp.run(operation="SYNTHESIS")

comp = client.get_component(name="generator")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp = client.get_component(name="stream_reader")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp = client.get_component(name="generator")
comp.run(operation="PACKAGE")

comp = client.get_component(name="stream_reader")
comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

comp = client.get_component(name="generator")
comp.run(operation="CO_SIMULATION")

vitis.dispose()

