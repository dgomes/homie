#include "homie.h"

static char buf[128];

Homie::Homie(PubSubClient &client, String deviceID) {
    this->_mqttClient = &client;
    this->deviceID = deviceID;
}

String IPaddress2String(IPAddress address) {
 return String(address[0]) + "." + 
        String(address[1]) + "." + 
        String(address[2]) + "." + 
        String(address[3]);
}

String byteIP2String(byte *address) {
 return String(address[0]) + "." + 
        String(address[1]) + "." + 
        String(address[2]) + "." + 
        String(address[3]);
}

bool Homie::setup(IPAddress localip, MQTT_CALLBACK_SIGNATURE) {
	String localip_str = IPaddress2String(localip);
	return setup(localip_str, callback);
}

bool Homie::setup(byte *localip, MQTT_CALLBACK_SIGNATURE) {
	String localip_str = byteIP2String(localip);
	return setup(localip_str, callback);
}
bool Homie::setup(String localip, MQTT_CALLBACK_SIGNATURE) {
    this->localip = localip; 
    this->callback = callback;
    this->_mqttClient->setCallback(callback);

    this->_setupCalled = true;
    return this->connect(); 
}

void Homie::setBrand(String name) {
    this->brandname = name;
}

void Homie::setFirmware(String name, String version) {
    this->firmware_name = name;
    this->firmware_version = version;
}

bool Homie::publish_property(String property, String value) {
    sprintf(buf, "%s%s/%s",MQTT_BASE_TOPIC, this->deviceID.c_str(), property.c_str());
    return this->_mqttClient->publish(buf, value.c_str(), true);
}

bool Homie::subscribe_property(String property) {
    sprintf(buf, "%s%s/%s",MQTT_BASE_TOPIC, this->deviceID.c_str(), property.c_str());
    return this->_mqttClient->subscribe(buf);
}

bool Homie::connect() {
    if(!this->_setupCalled)
        return false;

    sprintf(buf, "%s%s/%s",MQTT_BASE_TOPIC, this->deviceID.c_str(), "$online");
    if(!this->_mqttClient->connect(this->deviceID.c_str(), buf, 1, true, "false")) {
        DEBUG_PRINTLN(F("Failed to connect to MQTT"));
        return false;
    }

    publish_property(F("$online"), "true");
    publish_property(F("$name"), this->brandname);
    publish_property(F("$localip"), this->localip); 
    publish_property(F("$fwname"), this->firmware_name); 
    publish_property(F("$fwversion"), this->firmware_version);

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
   
	if ((millis()/UPTIME_REPORT_PERIOD) != time) {
        time = millis()/UPTIME_REPORT_PERIOD;
        publish_property(F("$uptime"), String(millis()/1000));
    }
 
    this->_mqttClient->loop();
    return true;
}
