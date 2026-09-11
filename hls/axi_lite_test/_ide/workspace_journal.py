# 2026-09-10T09:46:45.261604
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

vitis.dispose()

