# 2026-09-09T17:15:04.698307400
import vitis

client = vitis.create_client()
client.set_workspace(path="pulse_gen")

comp = client.get_component(name="pulse_generator")
comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

vitis.dispose()

