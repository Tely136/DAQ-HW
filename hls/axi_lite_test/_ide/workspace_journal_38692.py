# 2026-09-09T14:03:30.542881100
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

comp = client.get_component(name="led_blinker")
comp.run(operation="C_SIMULATION")

vitis.dispose()

