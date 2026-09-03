# 2026-09-02T09:10:08.620667900
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

platform = client.get_component(name="platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa")

status = platform.build()

status = platform.build()

comp = client.get_component(name="xgpio_example")
comp.build()

status = platform.build()

comp = client.get_component(name="xbram_example")
comp.build()

vitis.dispose()

