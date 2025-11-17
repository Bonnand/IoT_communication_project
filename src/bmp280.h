#ifndef BMP280_H
#define BMP280_H

#include <Arduino.h>
#include <SPI.h>

#define READ_TEMPERATURE 0
#define READ_PRESSURE 1

class BMP280 {

private:
    // Constants
    const uint8_t TEMP_BEGIN_REGISTER = 0xFA;
    const uint8_t PRES_BEGIN_REGISTER = 0xF7;
    const uint8_t CRTL_MEAS_REGISTER = 0x74;

    const uint8_t DIG_T1_REGISTER = 0x88;
    const uint8_t DIG_T2_REGISTER = 0x8A;
    const uint8_t DIG_T3_REGISTER = 0x8C;
    const uint8_t DIG_P1_REGISTER = 0x8E;
    const uint8_t DIG_P2_REGISTER = 0x90;
    const uint8_t DIG_P3_REGISTER = 0x92;
    const uint8_t DIG_P4_REGISTER = 0x94;
    const uint8_t DIG_P5_REGISTER = 0x96;
    const uint8_t DIG_P6_REGISTER = 0x98;
    const uint8_t DIG_P7_REGISTER = 0x9A;
    const uint8_t DIG_P8_REGISTER = 0x9C;
    const uint8_t DIG_P9_REGISTER = 0x9E;

    const uint8_t CHIP_SELECT_PIN = 4;

    // Data
    uint32_t temp_raw;
    float temperature;
    uint32_t pres_raw;
    float pressure;

    // Compensation value;
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    int32_t t_fine;

public:
    // Constructor
    BMP280();

    // Getters
    float get_temperature();
    float get_pressure();

    // Functions 
    void setup();
    void read_data(int read_choice);
    void read_compensation_param();
    void convert_pressure();
    void convert_temperature();
    void display_data();
};

#endif
