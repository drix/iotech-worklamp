#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

bool mosquito_shouldlog = false;

void Mosquito_setup() {
  Serial.println("Mosquito_setup()");
  client.setServer("192.168.0.193", 1883);
  mosquito_shouldlog = true;
  Serial.println("Mosquito server setted");
}

void Mosquito_reconnect() {
  Serial.println("Mosquito_reconnect()");
  // Create a random client ID
  String clientId = "ESP8266-worklamp-" + String(random(0xffff), HEX);
  if (client.connect(clientId.c_str())) {
    client.publish("dev/worklamp/log", "connected");
  }
}

int retry = 3;

bool Mosquito_loop() {
  if(!mosquito_shouldlog) return false;
  
  if (!client.connected() && retry > 0) {
    retry--;
    Mosquito_reconnect();
  }
  if (client.connected()) {
    retry = 3;
    client.loop();
    return true;
  }
  return false;
}

void Mosquito_log(String msg) {
  Serial.println(msg);
  
  if (Mosquito_loop()) {
    // Length (with one extra character for the null terminator)
    int str_len = msg.length() + 1; 
    // Prepare the character array (the buffer) 
    char char_array[str_len];
    // Copy it over 
    msg.toCharArray(char_array, str_len);
    client.publish("dev/worklamp/log", char_array);
    Serial.println("message published");
  }
}
