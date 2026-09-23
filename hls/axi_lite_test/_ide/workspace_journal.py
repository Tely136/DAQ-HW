# 2026-09-23T09:16:09.907735900
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

comp = client.create_hls_component(name = "using_axi_master",template = "vitis_hls_examples/Interface/Memory/using_axi_master")

client.delete_component(name="led_blinker")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0",compiler = "gcc")

platform = client.get_component(name="platform")
status = platform.build()

status = platform.build()

comp = client.get_component(name="xgpio_tapp_example")
comp.build()

status = platform.build()

comp = client.get_component(name="xgpio_intr_tapp_example")
comp.build()

status = platform.build()

comp = client.get_component(name="xbram_example")
comp.build()

comp = client.create_app_component(name="ram_write",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_ps7_cortexa9_0")

status = platform.build()

comp = client.get_component(name="ram_write")
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

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

client.delete_component(name="xbram_example")

client.delete_component(name="xgpio_intr_tapp_example")

client.delete_component(name="xgpio_tapp_example")

advanced_options = client.create_advanced_options_dict(dt_overlay="0")

platform = client.create_platform_component(name = "platform_linux",hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa",os = "linux",cpu = "ps7_cortexa9",domain_name = "linux_ps7_cortexa9",advanced_options = advanced_options)

client.delete_component(name="platform_linux")

status = platform.build()

comp.build()

comp = client.create_hls_component(name = "ram_write_ip",platform = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\axi_lite_test\ram_write_ip\hls_config.cfg")

cfg.set_values(key="syn.file", values=["ram_write_ip.cpp"])

cfg.set_values(key="tb.file", values=["ram_write_test.cpp"])

comp = client.get_component(name="ram_write_ip")
comp.run(operation="C_SIMULATION")

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/axi_lite_test/ram_write_ip/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="example")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa")

status = platform.build()

comp = client.create_app_component(name="ram_write_with_ip",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_ps7_cortexa9_0")

comp = client.get_component(name="ram_write_with_ip")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../ram_write_ip/ram_write_ip/hls/impl/ip/drivers", files=["example_v1_0"], dest_dir_in_cmp = "src", is_skip_copy_sources = False)

comp.set_app_config(key = "USER_COMPILE_SOURCES", values = ["example_v1_0/src/xexample_linux.c", "example_v1_0/src/xexample_sinit.c"])

comp.set_app_config(key = "USER_COMPILE_SOURCES", values = ["example_v1_0/src/xexample_sinit.c"])

comp.set_app_config(key = "USER_COMPILE_SOURCES", values = [""])

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../ram_write_ip/ram_write_ip/hls/impl/ip/drivers/example_v1_0/src", files=["xexample.c", "xexample.h", "xexample_hw.h", "xexample_linux.c", "xexample_sinit.c"], dest_dir_in_cmp = "src", is_skip_copy_sources = False)

status = platform.build()

comp = client.get_component(name="ram_write_with_ip")
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

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

comp = client.get_component(name="ram_write_ip")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp.run(operation="C_SIMULATION")

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../../cora-mcs/counter_wrapper.xsa")

status = platform.build()

status = platform.build()

comp = client.get_component(name="ram_write_with_ip")
comp.build()

status = platform.build()

comp.build()

vitis.dispose()

