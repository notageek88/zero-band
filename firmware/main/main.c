/**
 * ZERO Band — Firmware Skeleton v0.1
 * Target: LILYGO T-Beam Supreme (ESP32-S3)
 * 
 * Modules:
 *   - LoRa ping (SX1262 via SPI)
 *   - GPS NMEA parse (u-blox NEO-M9N via UART)
 *   - BLE advertising (ESP-IDF BT stack)
 *   - OLED display (SSD1306 via I2C)
 * 
 * Build: ESP-IDF v5.x
 *   idf.py build && idf.py flash monitor
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "driver/spi_master.h"
#include "driver/i2c.h"

// ============================================================
// PIN DEFINITIONS — T-Beam Supreme
// ============================================================
// LoRa SX1262 (SPI)
#define LORA_MOSI   10
#define LORA_MISO   11
#define LORA_SCK    12
#define LORA_CS     13
#define LORA_RST    17
#define LORA_DIO1   14
#define LORA_BUSY   18

// GPS u-blox NEO-M9N (UART1)
#define GPS_TX      34
#define GPS_RX      33
#define GPS_UART    UART_NUM_1
#define GPS_BAUD    115200

// OLED SSD1306 (I2C)
#define I2C_SDA     21
#define I2C_SCL     22
#define OLED_ADDR   0x3C

// IMU ICM-42688-P (SPI or I2C — same SPI bus as LoRa)
#define IMU_CS      4

// Status LED
#define LED_PIN     37

static const char *TAG = "ZERO";

// ============================================================
// LoRa — SX1262 Basic Driver
// ============================================================

typedef struct {
    spi_device_handle_t spi;
} lora_t;

static lora_t lora_dev;

static esp_err_t lora_cmd(spi_device_handle_t spi, uint8_t cmd, uint8_t *data, size_t len) {
    spi_transaction_t t = {
        .flags = 0,
        .cmd = cmd,
        .length = len * 8,
        .tx_buffer = data,
    };
    return spi_device_transmit(spi, &t);
}

static esp_err_t lora_init(lora_t *dev) {
    // SPI bus config
    spi_bus_config_t buscfg = {
        .miso_io_num = LORA_MISO,
        .mosi_io_num = LORA_MOSI,
        .sclk_io_num = LORA_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 256,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 8 * 1000 * 1000,  // 8 MHz
        .mode = 0,
        .spics_io_num = LORA_CS,
        .queue_size = 4,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &dev->spi));

    // Reset SX1262
    gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
    gpio_set_level(LORA_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(LORA_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(10));

    ESP_LOGI(TAG, "LoRa SX1262 initialized");
    return ESP_OK;
}

static void lora_send_ping(lora_t *dev) {
    // Простой ping пакет — "ZERO:PING"
    uint8_t payload[] = "ZERO:PING";
    // TODO: настроить частоту 868.0 MHz, SF7, BW125, CR4/5
    // TODO: вызвать SetTx через SPI
    ESP_LOGI(TAG, "LoRa TX: %s", payload);
}

// ============================================================
// GPS — NMEA Parser (GPRMC)
// ============================================================

typedef struct {
    float lat;
    float lon;
    float speed_knots;
    char time[10];
    bool valid;
} gps_data_t;

static gps_data_t gps_data = {0};

static void gps_parse_rmc(const char *line) {
    // $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
    char buf[128];
    strncpy(buf, line, sizeof(buf) - 1);

    char *token = strtok(buf, ",");
    if (!token) return;

    int field = 0;
    float raw_lat = 0, raw_lon = 0;
    char lat_dir = 'N', lon_dir = 'E';
    char status = 'V';

    while (token) {
        switch (field) {
            case 1: strncpy(gps_data.time, token, 9); break;  // time
            case 2: status = token[0]; break;                  // A=valid, V=invalid
            case 3: raw_lat = atof(token); break;              // lat DDMM.MMMM
            case 4: lat_dir = token[0]; break;
            case 5: raw_lon = atof(token); break;              // lon DDDMM.MMMM
            case 6: lon_dir = token[0]; break;
            case 7: gps_data.speed_knots = atof(token); break;
        }
        token = strtok(NULL, ",");
        field++;
    }

    if (status == 'A') {
        // Convert DDMM.MMMM -> decimal degrees
        int lat_deg = (int)(raw_lat / 100);
        gps_data.lat = lat_deg + (raw_lat - lat_deg * 100) / 60.0f;
        if (lat_dir == 'S') gps_data.lat = -gps_data.lat;

        int lon_deg = (int)(raw_lon / 100);
        gps_data.lon = lon_deg + (raw_lon - lon_deg * 100) / 60.0f;
        if (lon_dir == 'W') gps_data.lon = -gps_data.lon;

        gps_data.valid = true;
        ESP_LOGI(TAG, "GPS: %.6f, %.6f @ %.1f knots", gps_data.lat, gps_data.lon, gps_data.speed_knots);
    } else {
        gps_data.valid = false;
    }
}

static void gps_task(void *arg) {
    uart_config_t uart_cfg = {
        .baud_rate = GPS_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_driver_install(GPS_UART, 1024, 0, 0, NULL, 0);
    uart_param_config(GPS_UART, &uart_cfg);
    uart_set_pin(GPS_UART, GPS_TX, GPS_RX, -1, -1);

    char line[128];
    int pos = 0;
    uint8_t ch;

    ESP_LOGI(TAG, "GPS task started");

    while (1) {
        if (uart_read_bytes(GPS_UART, &ch, 1, pdMS_TO_TICKS(100)) > 0) {
            if (ch == '\n') {
                line[pos] = '\0';
                if (strncmp(line, "$GPRMC", 6) == 0 || strncmp(line, "$GNRMC", 6) == 0) {
                    gps_parse_rmc(line);
                }
                pos = 0;
            } else if (ch != '\r' && pos < 127) {
                line[pos++] = ch;
            }
        }
    }
}

// ============================================================
// BLE — Advertising (ESP-IDF Bluedroid)
// ============================================================

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_main.h"

static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20,   // 20ms
    .adv_int_max        = 0x40,   // 40ms
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    if (event == ESP_GAP_BLE_ADV_START_COMPLETE_EVT) {
        ESP_LOGI(TAG, "BLE advertising started");
    }
}

static esp_err_t ble_init(void) {
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());
    ESP_ERROR_CHECK(esp_ble_gap_register_callback(gap_event_handler));

    // Set device name
    ESP_ERROR_CHECK(esp_ble_gap_set_device_name("ZERO-BAND"));

    // Advertising data
    uint8_t adv_data[] = {
        0x02, 0x01, 0x06,         // Flags: LE General Discoverable
        0x0B, 0x09, 'Z','E','R','O','-','B','A','N','D','\0',  // Complete Local Name
    };
    ESP_ERROR_CHECK(esp_ble_gap_config_adv_data_raw(adv_data, sizeof(adv_data)));
    ESP_ERROR_CHECK(esp_ble_gap_start_advertising(&adv_params));

    return ESP_OK;
}

// ============================================================
// OLED — SSD1306 I2C Basic
// ============================================================

static esp_err_t oled_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA,
        .scl_io_num = I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0));

    // SSD1306 init sequence
    uint8_t init_cmds[] = {
        0xAE,        // Display OFF
        0xD5, 0x80,  // Clock divide
        0xA8, 0x3F,  // Multiplex ratio (64)
        0xD3, 0x00,  // Display offset
        0x40,        // Start line 0
        0x8D, 0x14,  // Charge pump ON
        0x20, 0x00,  // Memory mode horizontal
        0xA1,        // Segment remap
        0xC8,        // COM scan direction
        0xDA, 0x12,  // COM pins
        0x81, 0xCF,  // Contrast
        0xD9, 0xF1,  // Pre-charge
        0xDB, 0x40,  // VCOMH
        0xA4,        // Display RAM
        0xA6,        // Normal display
        0xAF,        // Display ON
    };

    for (int i = 0; i < sizeof(init_cmds); i++) {
        uint8_t buf[2] = {0x00, init_cmds[i]};  // 0x00 = command
        i2c_master_write_to_device(I2C_NUM_0, OLED_ADDR, buf, 2, pdMS_TO_TICKS(100));
    }

    ESP_LOGI(TAG, "OLED SSD1306 initialized");
    return ESP_OK;
}

static void oled_show_status(void) {
    // TODO: render GPS coords + LoRa status на дисплей
    // Использовать u8g2 библиотеку для нормального рендера текста
    ESP_LOGI(TAG, "OLED: showing status");
}

// ============================================================
// LoRa Ping Task
// ============================================================

static void lora_task(void *arg) {
    lora_t *dev = (lora_t *)arg;
    ESP_LOGI(TAG, "LoRa task started");

    while (1) {
        lora_send_ping(dev);
        oled_show_status();
        vTaskDelay(pdMS_TO_TICKS(5000));  // Ping every 5 seconds
    }
}

// ============================================================
// Main
// ============================================================

void app_main(void) {
    ESP_LOGI(TAG, "ZERO Band v0.1 starting...");

    // NVS init (required for BLE)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    // Init peripherals
    lora_init(&lora_dev);
    oled_init();
    ble_init();

    // Start tasks
    xTaskCreate(gps_task,  "gps",  4096, NULL,         5, NULL);
    xTaskCreate(lora_task, "lora", 4096, &lora_dev,    4, NULL);

    // Main loop — heartbeat LED
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(900));

        if (gps_data.valid) {
            ESP_LOGI(TAG, "Status: GPS OK (%.4f, %.4f) | LoRa TX | BLE ADV",
                     gps_data.lat, gps_data.lon);
        } else {
            ESP_LOGI(TAG, "Status: GPS searching... | LoRa TX | BLE ADV");
        }
    }
}
