# 2026-09-02T13:17:37.043750800
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="xbram_example")
comp.build()

client.delete_component(name="xgpio_example")

client.delete_component(name="xgpio_example")

vitis.dispose()

