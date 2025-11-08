# ESP32 connected to BMP280 (SPI), SHT40 (I²C), and Light Sensor (CAN) and send data to Blynk 

## Overview
This project uses an **ESP32** to measure environmental data, send values to the **Blynk** platform, **CAN**, **I²C** and **SPI** bus are used.

It includes:
- **BMP280** (SPI): temperature and pressure  
- **SHT40** (I²C): temperature and humidity  
- **Light sensor**: brightness measurement

---

## Hardware Required
- ESP32 board (FireBeetle, DevKit, etc.)  
- BMP280 (SPI)  
- SHT40 (I²C)  
- Light sensor (analog or digital)  
- CAN transceiver (SN65HVD230)  

---

## ESP32 Wiring

### BMP280 – SPI
| Signal | ESP32 Pin |
|---------|------------|
| CS      | 4          |
| SCK     | 18         |
| MISO    | 19         |
| MOSI    | 23         |
| VCC     | 3.3V       |
| GND     | GND        |

### SHT40 – I²C
| Signal | ESP32 Pin |
|---------|------------|
| SDA     | 21         |
| SCL     | 22         |
| VCC     | 3.3V       |
| GND     | GND        |

### CAN Bus
| Signal | ESP32 Pin |
|----------|-----------|
| TX (CTX) | 14        |
| RX (CRX) | 13        |
| CANH / CANL | Connect to CAN transceiver |

---

## Blynk Virtual Pins
| Function | Virtual Pin |
|-----------|--------------|
| SHT40 Temperature | V1 |
| SHT40 Humidity | V2 |
| Communication protocol V3 |
| BMP280 Pressure | V4 |
| BMP280 Temperature | V5 |
| C0 Sensor | V6 |

---
