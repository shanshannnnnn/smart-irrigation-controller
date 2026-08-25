/* ---------------------------------------------------------------------------
 * TEMPLATE FILE — copy this to `credentials.h` and fill in your own values.
 *
 *   cp credentials.example.h credentials.h
 *
 * `credentials.h` is listed in .gitignore and must never be committed.
 * ------------------------------------------------------------------------- */

/* Setup WiFi Connection
   The ESP32 only supports 2.4 GHz networks — a 5 GHz SSID will not connect.
*/
   #define ssid          "YOUR_WIFI_SSID"
   #define password      "YOUR_WIFI_PASSWORD"


/* Setup MQTT Broker Connection

   Default (unencrypted, port 1883). Used when neither HIVEMQS nor MOSQUITTOS
   is defined in profile.h.

   For a public test broker you can leave username/password empty:
     mqtt_server   "test.mosquitto.org"
     mqtt_port     1883
*/
   #define mqtt_server   "YOUR_BROKER_HOSTNAME"
   #define mqtt_port     1883
   #define mqtt_username "YOUR_BROKER_USERNAME"
   #define mqtt_password "YOUR_BROKER_PASSWORD"


/*** For Secure TLS/MQTTS, use with certificates in cert.h ***/
   #ifdef HIVEMQS                          // private broker: <id>.s2.eu.hivemq.cloud
     #define mqtt_server   "YOUR_INSTANCE.s2.eu.hivemq.cloud"
     #define mqtt_port     8883
     #define mqtt_username "YOUR_HIVEMQ_USERNAME"
     #define mqtt_password "YOUR_HIVEMQ_PASSWORD"
   #endif

   #ifdef MOSQUITTOS                       // public broker: test.mosquitto.org
     #define mqtt_server   "test.mosquitto.org"
     #define mqtt_port     8883
     #define mqtt_username ""
     #define mqtt_password ""
   #endif
