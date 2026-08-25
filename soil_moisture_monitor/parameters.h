/*   MQTT topic format
 publish : appname/clientid/device
 subscribe : DL/appname/clientid/device

 for use with set_topics()
 for use with client.publish() and client.subscribe()
 see https://pubsubclient.knolleary.net/api
*/
     #define appname "MoistureSensor"    //  name of your iot application
     #define clientid "device01"    //  unique id for this client; 8-12 characters
     #define device "M"             //  "S" sensor ; "M" sensor+actuator


/****** Others *******************/
     #define keepalive 180         // mqtt connection keepalive, in s
     #define QoS 0                 // Quality of Service 0: at most once ; 1: at least once ; 2(not supported): exactly once

     #define mintxinterval 10000   // min interval betw transmit, in ms
     #define Vbatmax 4.2           // battery fully charged voltage
