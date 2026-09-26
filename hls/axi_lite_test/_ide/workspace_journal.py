# 2026-09-24T12:38:28.318027
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

platform = client.create_platform_component(name = "test_platform",hw_design = "$COMPONENT_LOCATION/../../../vivado_projects/test/test_wrapper.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0",compiler = "gcc")

platform = client.get_component(name="test_platform")
status = platform.build()

comp = client.create_app_component(name="test_gen_reader",platform = "$COMPONENT_LOCATION/../test_platform/export/test_platform/test_platform.xpfm",domain = "standalone_ps7_cortexa9_0")

status = platform.build()

comp = client.get_component(name="test_gen_reader")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

vitis.dispose()

