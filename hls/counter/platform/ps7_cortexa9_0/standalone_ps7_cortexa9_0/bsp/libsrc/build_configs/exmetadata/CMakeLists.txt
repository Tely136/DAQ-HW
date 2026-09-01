
cmake_minimum_required(VERSION 3.15)
project(bsp)

# Use CMAKE_LIBRARY_PATH in link_directories
link_directories(${CMAKE_LIBRARY_PATH})
        
set(DRIVER_TARGETS bram;gpio;coresightps_dcc;devcfg;dmaps;emacps;gpiops;scugic;scutimer;scuwdt;sdps;uartps;usbps;xadcps)
set(DRIVER_LOCATIONS C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/bram_v4_14;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/gpio_v4_13;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/coresightps_dcc_v1_13;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/devcfg_v3_10;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/dmaps_v2_12;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/emacps_v3_24;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/gpiops_v3_15;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/scugic_v5_7;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/scutimer_v2_8;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/scuwdt_v2_7;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/sdps_v4_6;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/uartps_v3_18;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/usbps_v2_11;C:/AMDDesignTools/2026.1/Vitis/data/embeddedsw/XilinxProcessorIPLib/drivers/xadcps_v2_8)

list(LENGTH DRIVER_TARGETS no_of_drivers)
set(index 0)

while(${index} LESS ${no_of_drivers})
    list(GET DRIVER_TARGETS ${index} drv)
    list(GET DRIVER_LOCATIONS ${index} drv_dir)
    set(exp_dir "${drv_dir}/examples")
    add_custom_target(
        ${drv}_example ALL
        COMMAND lopper -f -O C:/DAQ-HW/hls/counter/platform/ps7_cortexa9_0/standalone_ps7_cortexa9_0/bsp/libsrc/${drv} C:/DAQ-HW/hls/counter/platform/ps7_cortexa9_0/standalone_ps7_cortexa9_0/bsp/hw_artifacts/ps7_cortexa9_0_baremetal.dts -- bmcmake_metadata_xlnx ps7_cortexa9_0 ${exp_dir} drvcmake_metadata
        BYPRODUCTS x${drv}_exlist.yaml)
    MATH(EXPR index "${index}+1")
endwhile()
