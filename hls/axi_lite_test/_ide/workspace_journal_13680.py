# 2026-09-09T13:45:01.554009400
import vitis

client = vitis.create_client()
client.set_workspace(path="axi_lite_test")

vitis.dispose()

