

int power = 0;
int currentPower = 0;
bool isLightON = false;
bool shuttedOFF = true;
long light_last_check = millis();

void Light_setup() {
  
  pinMode(15, OUTPUT); // D8
  Serial.println("light setted up");
}

void Light_loop() {
  long milliseconds = millis();
  if(milliseconds < light_last_check) return; // slow framerate
  int targetPower = 0;
  if(isLightON){
    shuttedOFF = false;
    targetPower = power;
  } else if(currentPower == 0 && !shuttedOFF) {
    shuttedOFF = true;
    digitalWrite(15, LOW);
  }

  if(!shuttedOFF && (currentPower != targetPower)) {
     int dirrection = currentPower > targetPower ? -1 : 1;
     long stepPower = 2;//max(abs(currentPower - targetPower) / 3, 2);
     currentPower = currentPower + (stepPower * dirrection);
     currentPower = min(100, max(0, currentPower));
     if (isLightON) {
      int newValue = min(124 + currentPower * 9, 1024);
  
//      Serial.println("newValue: " + String(newValue));
      analogWrite(15, newValue);
     } else {
      analogWrite(15, min(currentPower * 10, 1024));
     }
   }
   
  light_last_check = milliseconds + 25;
}

int Light_get_power() {
  return power;
}

bool Light_is_ON() {
  return isLightON;
}

void Light_set_power(int pwr) {
  pwr = min(100, max(0, pwr));
  if(pwr == power) return;
  
  power = pwr;
  
  Mosquito_log("light poeer setted to: " + String(power));
}

void Light_turn_ON() {

  isLightON = true;

  // set pwor to last saved value
  Light_set_power(max(1,power));
  Mosquito_log("light turned ON");
}


void Light_turn_OFF() {

  isLightON = false;
  Mosquito_log("light turned OFF");
}
