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

class Homie {
    private:
        PubSubClient *_mqttClient;
        String deviceID;
        bool _setupCalled;
        String brandname = "Homie";
        String localip;
        String firmware_name = "0.0.0";
        String firmaware_version = "0.0.0";
        const char **nodes;
        size_t nodes_size;
        unsigned long lastReconnectAttempt = 0;
        MQTT_CALLBACK_SIGNATURE;

        String IPAddress2String(IPAddress addr);
        bool connect();
    public:
        void setup(byte *localip, MQTT_CALLBACK_SIGNATURE);
        void setBrand(String name);
        void setFirmware(String name, String version);

        bool loop();
        
        String base_topic();
        bool publish_property(String property, String value);
        bool subscribe_property(String property);

        Homie(PubSubClient &client, String deviceID, const char *nodes[], size_t nodes_size);
};

#endif
