#include "homie.h"

Homie::Homie(PubSubClient &client, String deviceID, const char *nodes[], size_t nodes_size) {
    this->_mqttClient = &client;
    this->deviceID = deviceID;
    this->nodes = nodes;
    this->nodes_size = nodes_size;
}

String IPaddress2String(byte *address) {
 return String(address[0]) + "." + 
        String(address[1]) + "." + 
        String(address[2]) + "." + 
        String(address[3]);
}

void Homie::setup(byte *localip, MQTT_CALLBACK_SIGNATURE) {
    this->localip = IPaddress2String(localip); 
    this->callback = callback;
    this->_mqttClient->setCallback(callback);

    this->_setupCalled = true;
    this->connect(); 
}

void Homie::setBrand(String name) {
    this->brandname = name;
}

void Homie::setFirmware(String name, String version) {
    this->firmware_name = name;
    this->firmaware_version = version;
}

bool Homie::publish_property(String property, String value) {
    String prop = String(MQTT_BASE_TOPIC) + deviceID + String("/") + property;
    return this->_mqttClient->publish(prop.c_str(), value.c_str(), true);
}

bool Homie::subscribe_property(String property) {
    String prop = String(MQTT_BASE_TOPIC) + deviceID + String("/") + property;
    return this->_mqttClient->subscribe(prop.c_str()); 
}

String Homie::base_topic() {
    return String(MQTT_BASE_TOPIC) + deviceID + String("/"); 
}
bool Homie::connect() {
    if(!this->_setupCalled)
        return false;

    String online = String(MQTT_BASE_TOPIC) + deviceID + String("/$online");
    if(!this->_mqttClient->connect(this->deviceID.c_str(), NULL, NULL, online.c_str(), 0, true, "false"))
        return false;

    publish_property(String("$online"), "true");
    publish_property(String("$name"), this->brandname);
    publish_property(String("$localip"), this->localip); 
    publish_property(String("$fwname"), this->firmware_name); 
    publish_property(String("$fwversion"), this->firmaware_version);

    unsigned i = 0;
    String nodes_str;
    for (; i< this->nodes_size-1; i++) {
        nodes_str += String(nodes[i])+ String(",");
        subscribe_property(String(nodes[i]) + "/set"); 
    }
    nodes_str += String(this->nodes[i]);
    subscribe_property(String(nodes[i]) + "/set"); 
    if(!publish_property(String("$nodes"), nodes_str)) {
        publish_property(String("$nodes"), "error: too many");
    }

    return true;
}

bool Homie::loop() {

    if(!this->_mqttClient->connected()) {
        long now = millis();
        if (now - this->lastReconnectAttempt > RECONNECT_DELAY) {
            this->lastReconnectAttempt = now;
            // Attempt to reconnect
            if (this->connect()) {
                this->lastReconnectAttempt = 0;
                return false;
            }
        }

    }
    
    this->_mqttClient->loop();
    return true;
}
