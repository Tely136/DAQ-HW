# 2026-09-09T13:41:50.515265200
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

vitis.dispose()

