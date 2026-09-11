# 2026-09-10T13:55:44.189210100
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

comp = client.get_component(name="counter")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

platform = client.get_component(name="platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa")

status = platform.build()

comp = client.create_app_component(name="hello_world",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_ps7_cortexa9_0",template = "hello_world")

status = platform.build()

comp = client.get_component(name="hello_world")
comp.build()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa")

status = platform.build()

status = platform.build()

comp = client.get_component(name="xbram_example")
comp.build()

client.delete_component(name="xbram_example")

client.delete_component(name="componentName")

comp = client.get_component(name="hello_world")
status = comp.clean()

status = platform.build()

comp.build()

vitis.dispose()

