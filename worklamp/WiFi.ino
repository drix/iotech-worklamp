
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

#define WIFI_SSID "casita"
#define WIFI_PASS "mandarima"


ESP8266WebServer server(8080);

bool ota_flag = false;
bool ota_setup = false;

// Wi-Fi Connection
void WiFi_setup() {
  WiFi.mode(WIFI_STA);
  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  // Connected!
  Serial.println();
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

  WiFi_server_setup();
}

void WiFi_server_setup() {

  server.on("/update", []() {
    server.send(200, "text/plain", "read for firmware update");
    if(!ota_setup) {
      WiFi_OTA_setup();
      ota_setup = true;
    }
    delay(1000);
    ota_flag = true;
  });

  server.on("/restart", []() {
    server.send(200, "text/plain", "restarting...");
    delay(1000);
    ESP.restart();
  });
  
  server.on("/ok", []() {
    server.send(200, "text/plain", "worklamp ok...");
  });
  
  server.begin();
}

void WiFi_OTA_setup() {
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFi_loop() {
  if(ota_flag) {
    unsigned long waiting = millis();
    while(millis() - waiting <= 180000){
      ArduinoOTA.handle();
      delay(10);
    }
    ota_flag = false;
  }
  server.handleClient();
}
