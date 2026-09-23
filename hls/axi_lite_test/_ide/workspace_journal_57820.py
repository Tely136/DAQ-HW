# 2026-09-14T14:20:24.499298400
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

comp = client.get_component(name="led_blinker")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

vitis.dispose()

