#pragma once
/**
 * @file serialCommandsSystem.h
 * @brief A library for creating commands for serial communication using ArduinoIoTCloud.
 * uses the "messaging" widget from ArduinoIoTCloud. and the "String" object from Arduino.h.
*/
#if !defined(SERIALCOMMANDSSYSTEM_H) && (defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)) // STD does not exist on Arduino
#define SERIALCOMMANDSSYSTEM_H
#include <Arduino.h>
#include <Preferences.h>
#include <ArduinoIoTCloud.h>
#include <map>
#include <vector>
#include <queue>


#include "stringTools.h"

class CloudSerialSystem {
    private:
        Preferences preferences;
        String* cloudSerialObject;
        std::map<String, void (*)(CloudSerialSystem*, std::vector<String>* /*Argv*/)> commandsList;
        std::queue<String> printBuffer;
        bool debug;
    public:
    /**
     * @brief Initializes the CloudSerialSystem object.
     * 
     * @param cloudSerialObject Pointer to the CloudSerialSystem object that will be printed to.
    */
    CloudSerialSystem(String* cloudSerialObject);
    
    /**
     * @brief Adds a command to the serial command system.
     * 
     * @param commandName The name of the command.
     * @param function A function pointer to the command's implementation.
     * @see "command" macro in commands.h
    */
    void addCommand(String commandName, void (*function)(CloudSerialSystem*, std::vector<String>*));
    
    /**
     * @brief Checks for commands and performs corresponding actions.
     * 
     * @param command The command to be checked.
    */
    void checkForCommands(String command);
    
    /**
     * @brief Checks for commands from the serial interface.
     * 
     * This function checks for commands from the serial interface and processes them accordingly.
     * 
     * @param serialObject The serial object to check for commands from.
     */
    void checkForCommands() { this->checkForCommands(*this->cloudSerialObject); };
    
    
    /**
     * Prints the specified message to the cloud output.
     *
     * @param message The message to be printed.
    */
    void print(String message);
    
    
    /**
     * Prints a debug message.
     *
     * @param message The message to be printed.
     * @note This function will only print the message if debug mode is enabled.
     * @see setDebug
    */
    void debugPrint(String message);
    
    /**
     * @brief Handle the print queue. This should be called in the cloud loop.
    */
    void handlePrintQueue();
    
    /**
     * @brief Gets the debug mode.
     * 
     * @return true if debug mode is enabled. false if debug mode is disabled.
    */
    bool getDebug();

    /**
     * @brief Sets the debug mode.
     * 
     * @param debug true to enable debug mode. false to disable debug mode.
    */
    void setDebug(bool debug);
};
#endif
