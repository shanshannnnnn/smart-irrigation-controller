/* 44IoT Activities development code */
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "credentials.h"
#include "parameters.h"
#include "profile.h"
#include "cert.h"

/*** Define Board IO Pins ***/
#define LD1 12
#define LD2 15
#define LD3 13
#define LDR 39     
#define PB1 0
#define PB2 27
#define A36 36
#define A13 13
#define SPK 2
#define mysensor 35

/*** Duo LED (ACTIVE LOW) ***/
#define DUO_GREEN 19     // D19
#define DUO_RED   4      // A04 / GPIO4

/*** Other components ***/
#define BUZZER 5
#define GREENLED 18      // renamed from REDLED
#define BUZZER_SW 34

/*** Expansion IO ***/
#define RELAY 23

/*** Global variables ***/
float sensor;
long timer;                        
bool currPB2Val, oldPB2Val;
bool buzzerMuted = false;
char publish_topic[100];
char subscribe_topic[100];

/*** WIFI and MQTT Client setup ***/
#if defined(HIVEMQS) || defined(MOSQUITTOS)
 WiFiClientSecure espClient;
#else
 WiFiClient espClient;
#endif
PubSubClient client(espClient);

void setup() {
  pinMode(LD1, OUTPUT); 
  pinMode(LD2, OUTPUT);
  pinMode(LD3, OUTPUT); 
  pinMode(A36, INPUT);  
  pinMode(LDR, INPUT);
  pinMode(PB2, INPUT);
  pinMode(RELAY, OUTPUT);  
  pinMode(mysensor, INPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(DUO_GREEN, OUTPUT);
  pinMode(DUO_RED, OUTPUT);
  pinMode(BUZZER_SW, INPUT);

  oldPB2Val = digitalRead(PB2);

  digitalWrite(RELAY, HIGH);
  digitalWrite(LD1, HIGH);
  digitalWrite(LD2, HIGH);
  digitalWrite(LD3, HIGH);

  digitalWrite(BUZZER, LOW);

  // ACTIVE LOW → OFF = HIGH
  digitalWrite(DUO_GREEN, HIGH);
  digitalWrite(DUO_RED, HIGH);
  digitalWrite(GREENLED, LOW);

  timer = millis();
  set_topics();

  Serial.begin(115200);
  while (!Serial) delay(1);

  #if defined(Publish) || defined(Subscribe)                              
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    client.setKeepAlive(keepalive);
    mqttconnect();
  #endif
}

void loop() {
  if (millis() > timer) {

    sensor = analogRead(mysensor);
    sensor = (sensor + 100) * 2048 / 1988;
    sensor = sensor * 0.0008;
    sensor = (sensor - 3) / (-0.02);

    Serial.print("Soil Moisture: ");
    Serial.println(sensor);

    /*** Reset outputs ***/
    digitalWrite(DUO_GREEN, HIGH);
    digitalWrite(DUO_RED, HIGH);
    digitalWrite(GREENLED, LOW);
    digitalWrite(BUZZER, LOW);

    if (sensor >= 100) {
      digitalWrite(DUO_GREEN, LOW);
      buzzerMuted = false;
    }
    else if (sensor < 100 && sensor >= 80) {
      digitalWrite(DUO_GREEN, LOW);
      digitalWrite(DUO_RED, LOW);
    }
    else if (sensor < 80 && sensor >= 40) {
      digitalWrite(DUO_RED, LOW);
    }
    else {   // sensor < 40 : dry, sound the alarm
      digitalWrite(DUO_RED, LOW);
      digitalWrite(GREENLED, HIGH);

      if (!buzzerMuted) {
        digitalWrite(BUZZER, HIGH);
      }
    }

    #if defined(Publish)
      DynamicJsonDocument doc(256);
      doc["v"] = (short)(sensor * 10);
      doc["b"] = (short)batterymonitor();
      doc["r"] = (short)!digitalRead(RELAY);

      char mqtt_message[256];
      serializeJson(doc, mqtt_message);
      client.publish(publish_topic, mqtt_message, false);
    #endif

    timer = millis() + max(mintxinterval, 10000);
  }

  if (digitalRead(BUZZER_SW) == LOW && digitalRead(BUZZER) == HIGH) {
    buzzerMuted = true;
    digitalWrite(BUZZER, LOW);
  }

  #if defined(Publish) || defined(Subscribe)
    if (!client.loop()) mqttconnect();
  #endif

  if (pb2PressedChk()) {
    if (sensor < 100) digitalWrite(LD1, !digitalRead(LD1));
  }
}

/******************** Functions **********************/

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  if (msg.equalsIgnoreCase("HI")) digitalWrite(RELAY, LOW);
  else digitalWrite(RELAY, HIGH);
}

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void mqttconnect() {
  while (!client.connected()) {
    if (client.connect(clientid, mqtt_username, mqtt_password)) {
      #if defined(Subscribe)
        client.subscribe(subscribe_topic, QoS);
      #endif
    } else delay(5000);
  }
}

void set_topics() {
  sprintf(publish_topic, "%s/%s/%s", appname, clientid, device);
  sprintf(subscribe_topic, "DL/%s/%s/%s", appname, clientid, device);
}

bool pb2PressedChk() {
  currPB2Val = digitalRead(PB2);
  if (currPB2Val != oldPB2Val) {
    oldPB2Val = currPB2Val;
    if (currPB2Val == LOW) return true;
  }
  return false;
}

float batterymonitor() {
  digitalWrite(A13, HIGH);
  delay(1);
  float r = analogRead(A36);
  r = (r + 100) * 2048 / 1988;
  r = r * 0.0008;
  r = (2 * r / Vbatmax) * 100;
  return r;
}
