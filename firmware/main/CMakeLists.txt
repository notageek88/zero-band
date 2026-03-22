idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES 
        driver
        esp_system
        nvs_flash
        bt
        freertos
        log
        spi_flash
)
