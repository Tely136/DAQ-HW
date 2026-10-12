# 2026-10-11T10:15:45.715731
import vitis

client = vitis.create_client()
client.set_workspace(path="ws_test")

comp = client.get_component(name="stream_generator")
comp.run(operation="CO_SIMULATION")

comp = client.create_hls_component(name = "stream_reader",platform = "$COMPONENT_LOCATION/../../../hardware/counter_wrapper.xsa",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="stream_reader")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

vitis.dispose()

