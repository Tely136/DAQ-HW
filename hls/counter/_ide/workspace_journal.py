# 2026-09-26T12:41:59.360922500
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="stream_gen_read_test")
comp.build()

comp = client.get_component(name="stream_generator")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

comp = client.get_component(name="stream_reader")
comp.run(operation="PACKAGE")

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa")

status = platform.build()

status = platform.build()

comp = client.get_component(name="stream_gen_read_test")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

comp = client.get_component(name="stream_generator")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa")

status = platform.build()

status = platform.build()

comp = client.get_component(name="stream_gen_read_test")
comp.build()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa")

status = platform.build()

status = platform.build()

comp.build()

