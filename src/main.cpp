// Blynk information connection
#define BLYNK_TEMPLATE_ID "TMPL5VXIZVkuu"
#define BLYNK_TEMPLATE_NAME "Projet IoT"
#define BLYNK_AUTH_TOKEN "G_r3L2JDRDnHhL6zJg4IICEBU_DkuBD6"

#include <Arduino.h>
#include <BlynkSimpleEsp32.h>
#include "sht40.h"
#include "bmp280.h"
#include "can.h"

// Internet credentials
char ssid[] = "iPhone";
char pass[] = "nouv1234";

#define I2C_PROTOCOL 1
#define SPI_PROTOCOL 2
#define CAN_PROTOCOL 3

// Global variables
int ledPin = 2;
int protocol_selected=3; // 1 - I2C | 2 - SPI | 3 - CAN
SHT40 sht40;
BMP280 bmp280;
CAN can;


void retrieve_data_from_sht40(){
  
  // Write SHT40 address and the address of the temperature/humidity register
  sht40.send_data();

  // Retrieve the received data
  sht40.receive_data();

  // Convert data according to the datasheet
  sht40.convert_raw_data();

  // Display it on the console 
  sht40.display_data();

  // Send data to Blynk 
  Blynk.virtualWrite(V1, sht40.get_temperature());
  Blynk.virtualWrite(V2, sht40.get_humidity());
}


void init_bmp280(){

  // Initialize BMP280
  bmp280.setup();

  // Read compensation parameters which permits to convert the temperature and pressure
  bmp280.read_compensation_param();
}


void retrieve_data_from_bmp280(){
  
  // Read temperature and pressure data
  bmp280.read_data(READ_TEMPERATURE);
  bmp280.read_data(READ_PRESSURE);

  // Convert data according to the datasheet
  bmp280.convert_temperature();
  bmp280.convert_pressure();
  
  // Display it on the console 
  bmp280.display_data();

  // Send data to Blynk 
  Blynk.virtualWrite(V4, bmp280.get_temperature());
  Blynk.virtualWrite(V5, bmp280.get_pressure());
}

void init_can(){

  // Initialize TWAI and ADC calibration
  can.init();
}

void receive_data_with_can(){

  // Read data received with can communication
  can.receive_data();

  // Convert the received data into CO levels
  can.convert_raw_data();

  // Display can data 
  can.display_received_can_data();

  // Send data to Blynk 
  Blynk.virtualWrite(V6, can.get_CO());
}

void send_lum_value_with_can(){
  
  // Retrieve the voltage of the luminosity sensor
  can.retrieve_data_from_lum_sensor();

  // Display luminosity value 
  can.display_lum_sensor_data();

  // Send the luminosity value on the CAN bus
  can.send_data();
}


// Blynk functions
BLYNK_WRITE(V0)  
{
  int value = param.asInt();  
  Serial.print("LED value received from Blynk : ");
  Serial.println(value);
  digitalWrite(ledPin, value);   
}

BLYNK_WRITE(V3)  
{
  // Get the value from the Blynk app 
  protocol_selected = param.asInt();     
  Serial.print("Value received from Blynk : ");
  Serial.println(protocol_selected);

  if (protocol_selected == 1) {
    Blynk.setProperty(V1, "isHidden", false);
    Blynk.setProperty(V2, "isHidden", false);
    Blynk.setProperty(V4, "isHidden", true);
    Blynk.setProperty(V5, "isHidden", true);
    Blynk.setProperty(V6, "isHidden", true);
  }
  else if (protocol_selected == 2) {
    Blynk.setProperty(V1, "isHidden", true);
    Blynk.setProperty(V2, "isHidden", true);
    Blynk.setProperty(V4, "isHidden", false);
    Blynk.setProperty(V5, "isHidden", false);
    Blynk.setProperty(V6, "isHidden", true);
  }
  else if(protocol_selected==3){
    Blynk.setProperty(V1, "isHidden", true);
    Blynk.setProperty(V2, "isHidden", true);
    Blynk.setProperty(V4, "isHidden", true);
    Blynk.setProperty(V5, "isHidden", true);
    Blynk.setProperty(V6, "isHidden", false);
  }
  else {
    Blynk.setProperty(V1, "isHidden", true);
    Blynk.setProperty(V2, "isHidden", true);
    Blynk.setProperty(V4, "isHidden", true);
    Blynk.setProperty(V5, "isHidden", true);
    Blynk.setProperty(V6, "isHidden", true);
  }
}


void setup() {

    // Initialise the serial connection
    Serial.begin(115200);

    // Initialise LED and ensure it is OFF initially
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);     

    // Initialise the Blynk connection
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    // Initialize BMP280
    init_bmp280();

    // Initialize CAN
    init_can();
}

void loop(){

  // Start Blynk application
  Blynk.run();
  
  // Retrieve data either to sht40 (I2C) or to bmp280 (SPI)
  if(protocol_selected == I2C_PROTOCOL){
    retrieve_data_from_sht40();
  }
  else if(protocol_selected == SPI_PROTOCOL){
    retrieve_data_from_bmp280();
  }
  else if(protocol_selected == CAN_PROTOCOL){
    receive_data_with_can();
    send_lum_value_with_can();
  }

  // Wait 1 second before retrieving more recent data and send data more recent data (CAN case)
  delay(1000);
}