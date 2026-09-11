# 2026-09-05T19:29:43.213422100
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.get_component(name="counter")
comp.run(operation="SYNTHESIS")

vitis.dispose()

