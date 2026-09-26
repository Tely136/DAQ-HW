# 2026-09-24T16:51:42.389197400
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

platform = client.get_component(name="platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa")

status = platform.build()

comp = client.create_app_component(name="stream_gen_read_test",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_ps7_cortexa9_0")

vitis.dispose()

