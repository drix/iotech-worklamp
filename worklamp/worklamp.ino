
#define SERIAL_BAUDRATE 115200

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  
  Button_setup();
  
  Light_setup();

  Sonar_setup();

  WiFi_setup();
  
  Mosquito_setup();

  fauxmo_setup();
  
  Mosquito_log("System nominal");
}

void loop() {
  
  Button_loop();
  
  Sonar_loop();
  
  Light_loop();

  WiFi_loop();

  fauxmo_loop();
  
  yield();             // Feed the WDT (again)
}
