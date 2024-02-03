file(GLOB SRC_FILES 
  bsp/*.c
  hw/*.c
  hw/driver/*.c
  hw/driver/touch/*.c
  hw/driver/hangul/*.c
  hw/driver/lcd/*.c

)

file(GLOB_RECURSE SRC_FILES_RECURSE
  common/*.c 

  # LVGL
  hw/driver/lvgl/*.c
  lib/lvgl/*.c
)


set(requires 
  "spi_flash" 
  "esp_timer"
  "esp_psram"
  "vfs"
  "nvs_flash"
  "esp_adc"
  "app_update"
  "esp_hid"
  "esp_lcd"
  "esp_mm"
  )


idf_component_register(    
    SRCS ${SRC_FILES}
    SRCS ${SRC_FILES_RECURSE}
    
    INCLUDE_DIRS ""
    INCLUDE_DIRS "bsp"
    INCLUDE_DIRS "common"
    INCLUDE_DIRS "common/core"
    INCLUDE_DIRS "common/hw/include"
    INCLUDE_DIRS "hw"
    INCLUDE_DIRS "lib" 
    # INCLUDE_DIRS "${PROJECT_DIR}/main/hw" 

    PRIV_REQUIRES soc driver freertos
    
    REQUIRES "${requires}"
    )

    target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
  

