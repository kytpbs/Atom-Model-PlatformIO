#pragma once
/**
 * @file serialCommandsSystem.h
 * @brief A library for creating commands for serial communication using ArduinoIoTCloud.
 * uses the "messaging" widget from ArduinoIoTCloud. and the "String" object from Arduino.h.
*/
#if !defined(SERIALCOMMANDSSYSTEM_H) && (defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)) // STD does not exist on Arduino
#define SERIALCOMMANDSSYSTEM_H
#include <Arduino.h>
#include <ArduinoIoTCloud.h>
#include <map>
#include <vector>
#include <queue>


#include "stringTools.h"

class CloudSerialSystem {
    private:
        String* cloudSerialObject;
        std::map<String, void (*)(CloudSerialSystem*, std::vector<String>* /*Argv*/)> commandsList;
        std::queue<String> printBuffer;
        bool debug = true;
    public:
    CloudSerialSystem(String* cloudSerialObject);
    void addCommand(String commandName, void (*function)(CloudSerialSystem*, std::vector<String>*));
    void checkForCommands(String command);
    void checkForCommands() { this->checkForCommands(*this->cloudSerialObject); };
    void print(String message);
    void debugPrint(String message);
    /**
     * @brief Handle the print queue. This should be called in the cloud loop.
    */
    void handlePrintQueue();
    bool getDebug();
    void setDebug(bool debug);
};
#endif
