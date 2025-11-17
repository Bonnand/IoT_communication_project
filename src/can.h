#ifndef CAN_H
#define CAN_H

#include <Arduino.h>
#include <driver/twai.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>

class CAN {

private:
    // Constants
    const uint8_t LUMINOSITY_PIN = 36; 
    const uint16_t CAN_ID = 0x322; 
    const float R0 = 0.91; 
    const float A_CO = -0.488;                  // Parameter of concentration curve
    const float B_CO = 3.0;                     // Parameter of concentration curve

    // Data
    twai_message_t msg;
    uint16_t can_data;
    uint16_t adc_raw_data;
    uint16_t adc_data_mV;
    float RS_gas; 
    float ratio; 
    double CO;  
    esp_adc_cal_characteristics_t adc1_chars;

public:
    // Constructor
    CAN();

    // Getters
    twai_message_t get_msg();
    uint16_t get_can_data();
    uint16_t get_adc_raw_data();
    uint16_t get_data_mV();
    uint16_t get_CO();

    // Functions
    void init();
    void send_data();
    void receive_data();
    void retrieve_data_from_lum_sensor();
    void convert_raw_data();
    void display_received_can_data();
    void display_lum_sensor_data();
};

#endif
