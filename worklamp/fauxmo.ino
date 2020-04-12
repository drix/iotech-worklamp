
#include "fauxmoESP.h"

#define DEVICE_NAME "work lamp"

static fauxmoESP fauxmo;

bool triggerON = false;
bool triggerOFF = false;
int setPowerTo = 0;

void fauxmo_loop(){
  // fauxmoESP uses an async TCP server but a sync UDP server
  // Therefore, we have to manually poll for UDP packets
  fauxmo.handle();

  if(triggerON) {
    triggerON = false;
    Light_turn_ON();
    Light_set_power(setPowerTo);
  }
  
  if(triggerOFF) {
    triggerOFF = false;
    Light_turn_OFF();
  }
}

void fauxmo_setup(){
  // You can use different ways to invoke alexa to modify the devices state:
  // Add virtual devices
  fauxmo.addDevice(DEVICE_NAME);
  fauxmo.setPort(80); // This is required for gen3 devices
  fauxmo.enable(true);
  
  Serial.println("Alexa setted up");
      
    // If your device state is changed by any other means (MQTT, physical button,...)
    // you can instruct the library to report the new state to Alexa on next request:
    // fauxmo.setState(ID_YELLOW, true, 255);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    // Callback when a command from Alexa is received. 
    // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
    // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
    // Just remember not to delay too much here, this is a callback, exit as soon as possible.
    // If you have to do something more involved here set a flag and process it in your main loop.
        
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if ( (strcmp(device_name, DEVICE_NAME) == 0) ) {
      // this just sets a variable that the main loop() does something about
      if (state) {
        triggerON = true;
        setPowerTo = ((value * 100) / 255);
      } else {
        triggerOFF = true;
      }
      Serial.println("Screen switched by Alexa");
    }
  });
}
