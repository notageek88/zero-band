# ZERO Band — Firmware

Скелет прошивки для LILYGO T-Beam Supreme (ESP32-S3).

## Что реализовано в v0.1

| Модуль | Статус | Описание |
|--------|--------|----------|
| LoRa SX1262 | skeleton | SPI init + ping TX каждые 5 сек |
| GPS NEO-M9N | working | UART parser NMEA GPRMC/GNRMC |
| BLE | working | Advertising как "ZERO-BAND" |
| OLED SSD1306 | skeleton | I2C init, рендер — TODO |

## Сборка

### Требования
- ESP-IDF v5.x (`idf.py --version`)
- USB-C кабель к T-Beam Supreme

### Установка ESP-IDF
```bash
git clone --recursive https://github.com/espressif/esp-idf.git ~/esp-idf
cd ~/esp-idf && git checkout v5.2
./install.sh esp32s3
source export.sh
```

### Сборка и прошивка
```bash
cd firmware/
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## Распиновка T-Beam Supreme

| Функция | GPIO |
|---------|------|
| LoRa MOSI | 10 |
| LoRa MISO | 11 |
| LoRa SCK | 12 |
| LoRa CS | 13 |
| LoRa RST | 17 |
| LoRa DIO1 | 14 |
| GPS TX → MCU RX | 33 |
| GPS RX → MCU TX | 34 |
| OLED SDA | 21 |
| OLED SCL | 22 |
| LED | 37 |

## Roadmap прошивки

- [ ] v0.1 — LoRa ping + GPS parse + BLE adv (текущий)
- [ ] v0.2 — Mesh protocol (Meshtastic-compatible packet format)
- [ ] v0.3 — BLE GATT сервис (данные на телефон)
- [ ] v0.4 — E-ink дисплей + u8g2
- [ ] v0.5 — Датчик пульса MAX30102
- [ ] v1.0 — OTA обновления + deep sleep
