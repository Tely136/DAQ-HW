# 2026-08-31T17:07:12.946462300
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.get_component(name="counter_test")
comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

