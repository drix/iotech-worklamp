// defines pins numbers
int TRIGER_PIN = 5; //D1
int ECHO_PIN   = 4; //D2

// defines variables
long duration;
int  distance;

static long last_detection = millis();
  
void Sonar_setup() {
  pinMode(TRIGER_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input
  Serial.println("sonar setted up");
}

void Sonar_loop() {
  long milliseconds = millis();
  if(milliseconds < last_detection) return;

// Clears the trigPin
  digitalWrite(TRIGER_PIN, LOW);
  delayMicroseconds(2);
  
// Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGER_PIN, LOW);
  
// Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  
// Calculating the distance
  distance = duration*0.034/2;
  
  if(distance > 0 && distance < 30) {
// turn on if off
    if(!Light_is_ON()){
      Light_turn_ON();
    }

// set power   
    int power;
    if(distance < 6){
      power = 100;
    } else if(distance > 21) {
      power = 0;
    } else {
      power = 100 - ((distance - 5) * 6);
    }
    Light_set_power(power);
  } 
  last_detection = milliseconds + 100;
}
