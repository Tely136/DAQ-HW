# 2026-09-23T16:50:40.484111400
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

client.delete_component(name="hello_world")

client.delete_component(name="componentName")

vitis.dispose()

