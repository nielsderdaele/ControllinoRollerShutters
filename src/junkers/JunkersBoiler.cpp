#include "JunkersBoiler.h"

JunkersBoiler::JunkersBoiler(byte inputPin, const MQTTClient& mqttClient){
    this->inputPin = inputPin;
    this->mqttClient = &mqttClient;  
    this->currentValue = -1;
}

double JunkersBoiler::getCurrentThermostatVoltage(){
    if (this->currentValue < 0) {
        return 0;
    } else {
        return this->currentValue * (24.0/1024);
    }
}

void JunkersBoiler::setup() {
    pinMode(this->inputPin, INPUT);
}

void JunkersBoiler::loop() {
    /* Read value each two seconds */
    if ((millis() - this->lastReadMillis) > 2000) {
        this->lastReadMillis = millis();

        /* Update sensor value */
        int sensorValue = analogRead(this->inputPin);
        if (sensorValue != this->currentValue) {
            this->currentValue = sensorValue;
            this->mqttClient->publishMessage("junkers/thermostat/voltage", String(this->getCurrentThermostatVoltage(), 2));
        }
    }
}