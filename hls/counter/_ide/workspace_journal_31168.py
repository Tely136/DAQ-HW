# 2026-09-08T16:38:28.150304800
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

vitis.dispose()

