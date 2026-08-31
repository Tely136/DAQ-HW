# 2026-08-30T20:45:34.900989800
import vitis

client = vitis.create_client()
client.set_workspace(path="DAQ-HW")

client.sync_git_example_repo(name="vitis_hls_examples")

client.sync_git_example_repo(name="vitis_libraries")

cfg = client.get_config_file(path="C:\Users\tely1\Documents\DAQ-HW\hls\counter\counter_test\hls_config.cfg")

cfg.set_values(key="syn.file", values=["counter_test.cpp"])

cfg = client.get_config_file(path="/c:/Users/tely1/Documents/DAQ-HW/hls/counter/counter_test/hls_config.cfg")

cfg.set_value(section="hls", key="syn.top", value="main")

