# 2026-09-25T20:33:09.784780200
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="stream_gen_read_test")
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

status = platform.build()

comp.build()

status = platform.build()

comp.build()

