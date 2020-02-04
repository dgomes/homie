/*
 Homie.h - A simple client for MQTT using Homie convention.
  Diogo Gomes
  http://diogogomes.com
*/

#ifndef Homie_h
#define Homie_h

#ifdef DEBUG
#define DEBUG_PRINT(x)     Serial.print (x)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x) 
#endif

#include <PubSubClient.h>

#define MQTT_BASE_TOPIC "devices/"
#define RECONNECT_DELAY 5000
#define UPTIME_REPORT_PERIOD 60000

class Homie {
    private:
        PubSubClient *_mqttClient;
        String deviceID;
        bool _setupCalled;
        String brandname = "Homie";
        String localip;
        String firmware_name = "0.0.0";
        String firmware_version = "0.0.0";
        unsigned long time; // var created to show uptime more close to zero milliseconds as possible
        unsigned long lastReconnectAttempt = 0;
        MQTT_CALLBACK_SIGNATURE;

        String IPAddress2String(IPAddress addr);
        String byteIP2String(byte *addr);

        bool connect();

    public:
        bool setup(byte *localip, MQTT_CALLBACK_SIGNATURE);
        bool setup(String localip, MQTT_CALLBACK_SIGNATURE);
        bool setup(IPAddress localip, MQTT_CALLBACK_SIGNATURE);
        void setBrand(String name);
        void setFirmware(String name, String version);

        bool loop();
        
        bool publish_property(String property, String value);
        bool subscribe_property(String property);

        Homie(PubSubClient &client, String deviceID);
        Homie(PubSubClient &client, String deviceID, const char *nodes[], size_t nodes_size);
};

#endif
