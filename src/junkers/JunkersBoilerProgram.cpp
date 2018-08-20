#include "JunkersBoilerProgram.h"
#include <Controllino.h>

JunkersBoilerProgram::JunkersBoilerProgram(const MQTTClient& mqttClient) {
    this->junkersBoiler = new JunkersBoiler(CONTROLLINO_A15, mqttClient);
}

void JunkersBoilerProgram::setup() {
    this->junkersBoiler->setup();    
}

void JunkersBoilerProgram::loop() {
    this->junkersBoiler->loop();
}