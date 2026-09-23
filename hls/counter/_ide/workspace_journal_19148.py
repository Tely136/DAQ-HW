# 2026-09-13T11:54:08.112046300
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.get_component(name="counter")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

vitis.dispose()

