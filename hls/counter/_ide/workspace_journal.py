# 2026-09-11T08:08:39.055390700
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

vitis.dispose()

