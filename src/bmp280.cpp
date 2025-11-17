#include "bmp280.h"

/*** Constructor ***/
BMP280::BMP280() 
    : temp_raw(0), temperature(0.0f), pres_raw(0), pressure(0.0f) {}
    


/*** Getters ***/    
float BMP280::get_temperature(){
  return this->temperature;
};
    
float BMP280::get_pressure(){
  return this->pressure;
}


/*** Functions ***/

void BMP280::setup(){

    // Initialize the chip select 
    pinMode(CHIP_SELECT_PIN, OUTPUT);
    
    // Start the SPI library
    SPI.begin();

    // Start BMP280 in normal mode (cyclic standby and measurement periods) and configure oversampling to x1 (temperature and pressure)    
    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(CRTL_MEAS_REGISTER);
    SPI.transfer(0x27);
    digitalWrite(CHIP_SELECT_PIN, HIGH);
}

void BMP280::read_data(int read_choice){

    uint8_t msb,lsb,xlsb;

    // Enable reading data
    digitalWrite(CHIP_SELECT_PIN, LOW);

    if(read_choice == READ_TEMPERATURE){

        // Send the beginning address of temperature register
        SPI.transfer(TEMP_BEGIN_REGISTER);

        // Read the three temperature registers (register increments automatically)
        msb = SPI.transfer(0x00);
        lsb = SPI.transfer(0x00);
        xlsb = SPI.transfer(0x00);
        temp_raw = ((uint32_t)msb << 12) | ((uint32_t)lsb << 4) | ((uint32_t)xlsb >> 4);
    }
    else if(read_choice == READ_PRESSURE){

        // Send the beginning address of pressure register 
        SPI.transfer(PRES_BEGIN_REGISTER);

        // Read the three pressure registers (register increments automatically)
        msb = SPI.transfer(0x00);
        lsb = SPI.transfer(0x00);
        xlsb = SPI.transfer(0x00);
        pres_raw = ((uint32_t)msb << 12) | ((uint32_t)lsb << 4) | ((uint32_t)xlsb >> 4);
    }

    // Disable reading data
    digitalWrite(CHIP_SELECT_PIN, HIGH);
}


void BMP280::read_compensation_param() {

    // Retrieve compensation parameters (LSB then MSB)
    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_T1_REGISTER);
    dig_T1 = SPI.transfer(0x00);
    dig_T1 |= ((uint16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_T2_REGISTER);
    dig_T2 = SPI.transfer(0x00);
    dig_T2 |= ((int16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_T3_REGISTER);
    dig_T3 = SPI.transfer(0x00);
    dig_T3 |= ((int16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_P1_REGISTER);
    dig_P1 = SPI.transfer(0x00);
    dig_P1 |= ((uint16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_P2_REGISTER);
    dig_P2 = SPI.transfer(0x00);
    dig_P2 |= ((int16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_P3_REGISTER);
    dig_P3 = SPI.transfer(0x00);
    dig_P3 |= ((int16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_P4_REGISTER);
    dig_P4 = SPI.transfer(0x00);
    dig_P4 |= ((int16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_P5_REGISTER);
    dig_P5 = SPI.transfer(0x00);
    dig_P5 |= ((int16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_P6_REGISTER);
    dig_P6 = SPI.transfer(0x00);
    dig_P6 |= ((int16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_P7_REGISTER);
    dig_P7 = SPI.transfer(0x00);
    dig_P7 |= ((int16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_P8_REGISTER);
    dig_P8 = SPI.transfer(0x00);
    dig_P8 |= ((int16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);

    digitalWrite(CHIP_SELECT_PIN, LOW);
    SPI.transfer(DIG_P9_REGISTER);
    dig_P9 = SPI.transfer(0x00);
    dig_P9 |= ((int16_t)SPI.transfer(0x00) << 8);
    digitalWrite(CHIP_SELECT_PIN, HIGH);
}



void BMP280::convert_temperature() {
    double var1, var2, T;
    
    // Convertion indicated in the datasheet
    var1 = (((double)temp_raw) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
    var2 = ((((double)temp_raw) / 131072.0 - ((double)dig_T1) / 8192.0) *
            (((double)temp_raw) / 131072.0 - ((double)dig_T1) / 8192.0)) *
            ((double)dig_T3);
    
    t_fine = (int32_t)(var1 + var2);
    T = (var1 + var2) / 5120.0;
    
    temperature = T;
}


void BMP280::convert_pressure() {
    double var1, var2, p;
    
    // Convertion indicated in the datasheet
    var1 = ((double)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((double)dig_P4) * 65536.0);
    var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)dig_P1);
    
    p = 1048576.0 - (double)pres_raw;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
    
    pressure = p;
}

void BMP280::display_data() {
    Serial.print("Raw Temperature = "); 
    Serial.print(temp_raw);
    Serial.print(" ----- Temp = "); Serial.print(temperature); 
    Serial.println(" °C");

    Serial.print("Raw Pressure = "); 
    Serial.print(pres_raw);
    Serial.print(" ----- Pressure  = "); Serial.print(pressure); 
    Serial.println(" Pa");
}

    
