# 2026-09-13T11:50:58.417542
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

vitis.dispose()

