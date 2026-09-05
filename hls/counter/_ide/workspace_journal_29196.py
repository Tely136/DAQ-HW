# 2026-09-03T16:48:33.498303300
import vitis

client = vitis.create_client()
client.set_workspace(path="counter")

cfg = client.get_config_file(path="C:\DAQ-HW\hls\counter\counter\hls_config.cfg")

cfg.set_values(key="syn.file", values=["counter.cpp", "counter.h", "counter_test.cpp"])

cfg = client.get_config_file(path="/c:/DAQ-HW/hls/counter/counter/hls_config.cfg")

cfg.set_value(section="hls", key="syn.cflags", value="")

cfg.set_value(section="hls", key="syn.csimflags", value="")

cfg.set_values(key="syn.file", values=["counter.cpp", "counter.h"])

cfg.set_values(key="syn.file_cflags", values=[])

cfg.set_values(key="syn.file_csimflags", values=[])

cfg.set_value(section="hls", key="tb.cflags", value="")

cfg.set_value(section="hls", key="syn.csimflags", value="")

cfg.set_values(key="tb.file_cflags", values=[])

cfg.set_values(key="tb.file", values=["./counter_test.cpp"])

vitis.dispose()

