#include <ESP8266WiFi.h>         //provides ESP8266 specific Wi-Fi routines we are calling to connect to network.
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>  // OTA library

#ifndef STASSID
#define STASSID "jsk"
#define STAPSK  "rajbhaimehta"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

char recStr[10] = "";
#include<SoftwareSerial.h>
SoftwareSerial SUART(4, 5);  //D2, D1 = SRX, STX
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
char auth[] = "S5nLbY5leUqm9DseIdTvk_soPTEY9_sM"; // the auth code that you got on your gmail and Blynk app
const int blink_led = D0;  // LED pin on NodeMCU ESP8266

WidgetTerminal terminal(V0);

int data, f = 0;
int c1 = 0, c2 = 0, c3 = 0, v1 = 0, v2 = 0, v3 = 0;

#define irSensorPin D3       // Flame sensor on this pin

void setup()
{
  pinMode(irSensorPin, INPUT_PULLUP);
  pinMode(blink_led, OUTPUT);
  Serial.println("Booting");
  // Step to connect ESP with the Wi-Fi
  WiFi.mode(WIFI_STA);               //Set ESP as station mode
  WiFi.begin(ssid, password);      //Wi-Fi Credentials

  while (WiFi.waitForConnectResult() != WL_CONNECTED)      //Connecting  ESP to wi-fi takes some time, so wait till it gets connected
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(100);
    ESP.restart();
  }
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
  ArduinoOTA.begin();          //OTA initialization
  Blynk.begin(auth, ssid, password);



  Serial.begin(115200);
  SUART.begin(115200);
}

void loop()
{
  ArduinoOTA.handle();
  //Blynk.notify("SENSOR DATA");
  Blynk.run();
  SUART.print(f); //sending 1234
  delay(100);
  //  SUART.readBytes(recStr, 3);
  //  Serial.println(recStr);
  //  delay(10);

  switch (f)
  {
    case 0:
      SUART.readBytes(recStr, 1);
      f++;
      //if(recStr>200){Blynk.notify(recStr);}
      terminal.print("c1 : ");
      terminal.print(recStr);
      terminal.print("  ");
      if(recStr>2){Blynk.notify("c1 over current");}
      delay(500);
      break;
    case 1:
      SUART.readBytes(recStr, 1);
      f++;
      terminal.print("c2 : ");
      terminal.print(recStr);
      terminal.print("  ");
      if(recStr>2){Blynk.notify("c2 over current");}
      //delay(500);
      break;
    case 2:
      SUART.readBytes(recStr, 1);
      f++;
      terminal.print("c3 : ");
      terminal.print(recStr);
      terminal.println("");
      if(recStr>2){Blynk.notify("c3 over current");}
      //delay(500);
      break;
    case 3:
      SUART.readBytes(recStr, 1);
      f++;
      terminal.print("v1 : ");
      terminal.print(recStr);
      terminal.print("  ");
      if(recStr>5){Blynk.notify("v1 over Voltage");}
      //delay(500);
      break;
    case 4:
      SUART.readBytes(recStr, 1);
      f++;
      terminal.print("v2 : ");
      terminal.print(recStr);
      terminal.print("  ");
      if(recStr>5){Blynk.notify("v2 over Voltage");}
      //delay(500);
      break;
    case 5:
      SUART.readBytes(recStr, 1);
      f = 0;
      terminal.print("v3 : ");
      terminal.print(recStr);
      terminal.println("*********");
      if(recStr>5){Blynk.notify("v3 over Voltage");}
      //delay(500);
      terminal.flush();
      break;
  }
  //Serial.println(digitalRead(irSensorPin)); //display the results
  if(digitalRead(irSensorPin)!=1){Blynk.notify("~~~~~FIRE~~~~~");}
} 
