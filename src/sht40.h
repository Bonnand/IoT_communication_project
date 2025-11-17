#ifndef SHT40_H
#define SHT40_H

#include <Arduino.h>
#include <Wire.h>

class SHT40 {

private:
    // Constants
    const uint8_t SLAVE_ADDR = 0x44; 
    const uint8_t DATA_REGISTER = 0xFD; 
    const int numb_of_data_to_rd = 6;

    // Data
    uint16_t temp_raw;
    float temperature;
    uint16_t hum_raw;
    float humidity;

public:
    // Constructor
    SHT40();

    // Getters
    float get_temperature();
    float get_humidity();

    // Functions
    void send_data();
    void receive_data();
    void convert_raw_data();
    void display_data();
};

#endif
