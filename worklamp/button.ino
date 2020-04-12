const int BUTTON_PIN = 2; // D4(gpio2)
bool isButtonPressed = false;
bool buttonLastState = false;
int buttonPressedDebouncer = 0;


bool lastDimming = false;

long requestToSwitchOFFtime = millis();
bool requestToSwitchOFF = false;

bool lightWasOnWhenPressed = false;
long last_pressing = millis();
long last_check = millis();
long delayTime = 1000;

long fistKeyPressTime = millis();
bool fistKeyPress = false;


void Button_setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.println("buttom setted up");
}

void Button_loop() {
// slow the framerate
  long milliseconds = millis();
  if(milliseconds < last_check) return; // avoid flicking from missing capasitor

// get information
  int power = Light_get_power();
  bool buttonHighDetected = digitalRead(BUTTON_PIN); // put your main code here, to run repeatedly:

//  Serial.println("buttom  up: "+ String(buttonHighDetected));
  
  // ensure electric static don't press the button (can be remove if capacitor is added)
  if (buttonHighDetected && buttonPressedDebouncer < 2) {
    buttonPressedDebouncer++;
    return;
  }
  
  buttonPressedDebouncer = 0;
  isButtonPressed = buttonHighDetected;
  
  
// Control buttom press
  if (isButtonPressed && !buttonLastState){
    lightWasOnWhenPressed = Light_is_ON();
    if (!lightWasOnWhenPressed) {
      Light_turn_ON();
    }
    last_pressing = millis();

// Control dimmer
  } else if(isButtonPressed && (milliseconds > (last_pressing + delayTime))){
    if(lastDimming){
      Light_set_power(power + 5);
    } else {
      Light_set_power(power - 5);
    }
  }

// Control buttom keyup
  if (!isButtonPressed && buttonLastState){

    if((milliseconds < (last_pressing + delayTime))) {

// control double click
      if (fistKeyPress) {
        if(power > 89) {
          Light_set_power(0);
          lastDimming = true;
        } else if(power < 11) {
          Light_set_power(100);
          lastDimming = false;
        } else if(lastDimming){
          Light_set_power(100);
          lastDimming = false;
        } else {
          Light_set_power(0);
          lastDimming = true;
        }
        requestToSwitchOFF = false;
        fistKeyPress = false;

// request to turn off
      } else if (lightWasOnWhenPressed){
        requestToSwitchOFF = true;
        requestToSwitchOFFtime = millis();
      }

// start double click checking
      fistKeyPress = true;
      fistKeyPressTime = millis();
      
    } else {
// revert dimmer, so second click&hold will dimmer to the other direction
      lastDimming = !lastDimming;
    }
  }
  
// stop listen for double click 
  if(fistKeyPress){
    if(milliseconds > fistKeyPressTime + 500){
      fistKeyPress = false;
    }  
  }
  
// switch OFF 
  if(requestToSwitchOFF){
    if(milliseconds > requestToSwitchOFFtime + 500){
      requestToSwitchOFF = false;
      Light_turn_OFF();
    }  
  }
  
// prepear for next loop
  buttonLastState = isButtonPressed;
  last_check = milliseconds + 50;
}
