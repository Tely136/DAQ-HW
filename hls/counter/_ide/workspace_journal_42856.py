# 2026-09-05T19:36:00.569666100
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.get_component(name="counter")
comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

vitis.dispose()

vitis.dispose()

