#include "can.h"

/*** Constructor ***/
CAN::CAN()
    : can_data(0), CO(0), adc_data_mV(0) {}


/*** Getters ***/   
uint16_t CAN::get_adc_raw_data(){
  return this->adc_raw_data;
};

uint16_t CAN::get_can_data(){
  return this->can_data;
};

twai_message_t CAN::get_msg(){
  return this->msg;
};
    
uint16_t CAN::get_data_mV(){
  return this->adc_data_mV;
}

uint16_t CAN::get_CO(){
  return this->CO;
}



/*** Functions ***/

void CAN::init() {
    
    // Select GPIO14 for TX, GPIO13 for RX and set normal TWAI mode to send/receive/acknowledge messages
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_14, GPIO_NUM_13, TWAI_MODE_NORMAL);

    // Set the communication speed
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();

    // Accept all CAN message IDs (no filtering)
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
 
    // Installs the TWAI driver with previous configuration
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        Serial.println("Driver CAN installed");
    } 
    else {
        Serial.println("Error during CAN driver installation");
        return;
    }
    
    // Starts the TWAI driver, putting the TWAI driver into the running state
    if (twai_start() == ESP_OK) {
        Serial.println("CAN running");
    } 
    else {
        Serial.println("Error during CAN start");
    }
    
    // ADC configuration -- ADC value on 12 bits (0–4095)
    esp_adc_cal_characterize(ADC_UNIT_1,ADC_ATTEN_DB_11,ADC_WIDTH_BIT_12,0,&(this->adc1_chars));
}

void CAN::retrieve_data_from_lum_sensor(){
    // Read the voltage of the luminosity sensor
    adc_raw_data = analogRead(LUMINOSITY_PIN);
}

void CAN::send_data() {

  // CAN message creation | Config : CAN ID - RTR(0) => sending message - Send two bytes
  twai_message_t msg;
  msg.identifier = CAN_ID;        
  msg.rtr = 0;                   
  msg.data_length_code = 2;        

  // Put the luminosity value on two bytes (MSB first)
  msg.data[0] = (adc_raw_data >> 8) & 0xFF; 
  msg.data[1] = adc_raw_data & 0xFF;         

  // Transmit CAN message
  if (twai_transmit(&msg, pdMS_TO_TICKS(1000)) == ESP_OK) {
    Serial.println("CAN message sent");
  } else {
    Serial.println("Error during sending CAN message");
  }
}


void CAN::receive_data() {

    // Display and parse data when a CAN message is received
    if (twai_receive( (&this->msg), pdMS_TO_TICKS(1000)) == ESP_OK) { 
        Serial.print("Reçu ID: 0x");
        Serial.println(msg.identifier, HEX); // Shows the ID of the message sender
    
        Serial.print("Data: ");
        for (int i = 0; i < msg.data_length_code; i++) {
            Serial.printf("0x%02X ", msg.data[i]);
        }       
        can_data = (msg.data[0] << 8) | msg.data[1];
    }
}


void CAN::convert_raw_data() {

    // Conversion brute -> mV
    adc_data_mV = (uint16_t) esp_adc_cal_raw_to_voltage(can_data, &adc1_chars);

    // Conversion according to the datasheet
    RS_gas = (5 - ((float)adc_data_mV/1000.0)) / ((float)adc_data_mV/1000.0);
    ratio = RS_gas / R0;

    // Retrieve CO value with concentration curve in the datasheet
    CO = pow(10, ( log10(ratio) - B_CO ) / A_CO );
}

void CAN::display_received_can_data() {
    Serial.print("Voltage = ");
    Serial.print(adc_data_mV);
    Serial.print(" mV | Monoxyde de carbone = ");
    Serial.print(CO);
    Serial.println(" ppm");
}

void CAN::display_lum_sensor_data(){
    Serial.print("Luminosity raw data : ");
    Serial.println(adc_raw_data);
}
    
