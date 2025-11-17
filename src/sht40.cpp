#include "sht40.h"

/*** Constructor ***/
SHT40::SHT40()
    : temp_raw(0), temperature(0.0f), hum_raw(0), humidity(0.0f) {}


/*** Getters ***/    
float SHT40::get_temperature(){
  return this->temperature;
};
    
float SHT40::get_humidity(){
  return this->humidity;
}


/*** Functions ***/    
void SHT40::send_data() {

    // Start the I2C library
    Wire.begin();

    // Write SHT40 address and the address of the temperature/humidity register
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(DATA_REGISTER); 
    Wire.endTransmission();

    delay(10);
}

void SHT40::receive_data() {

    uint8_t raw_data_buffer[numb_of_data_to_rd];
    int index = 0;

    // Retrieve the data sent from the temperature/humidity register
    Wire.requestFrom(SLAVE_ADDR, (uint8_t)numb_of_data_to_rd);

    // Read the data received byte per byte
    while (Wire.available()) {
        raw_data_buffer[index] = (uint8_t)Wire.read();
        index++;
    }

    // Save only the temperature/humidity data (avoids the CRCs)
    temp_raw = ((uint16_t)raw_data_buffer[0] << 8) | raw_data_buffer[1];
    hum_raw  = ((uint16_t)raw_data_buffer[3] << 8) | raw_data_buffer[4];
}

void SHT40::convert_raw_data() {

    // According to the datasheet
    temperature = -45.0f + 175.0f * (temp_raw / 65535.0f); 
    humidity = -6.0f + 125.0f * (hum_raw / 65535.0f);
}

void SHT40::display_data() {
    Serial.print("Raw Temperature = "); 
    Serial.print(temp_raw);
    Serial.print(" ----- Temp = "); Serial.print(temperature); 
    Serial.println(" °C");

    Serial.print("Raw Humidity = "); 
    Serial.print(hum_raw);
    Serial.print(" ----- Hum  = "); Serial.print(humidity); 
    Serial.println(" %RH");
}

    
