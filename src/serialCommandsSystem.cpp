#include "serialCommandsSystem.h"
#ifdef SERIALCOMMANDSSYSTEM_H // only include this file if serialCommandsSystem.h is included

bool firstRun = true; // will be used to setup "debug" preference, and initialize the preferences object

CloudSerialSystem::CloudSerialSystem(String* cloudSerialObject) {
    this->cloudSerialObject = cloudSerialObject;
}

void CloudSerialSystem::addCommand(String commandName, void (*function)(CloudSerialSystem*, std::vector<String>*)) {
    this->commandsList[commandName] = function;
}

void CloudSerialSystem::print(String message) {
    this->printBuffer.push(message);
}

void CloudSerialSystem::checkForCommands(String command) {
    int spaceIndex = command.indexOf(" ");
    bool hasArgs = spaceIndex != -1;
    
    if (!hasArgs) {
        spaceIndex = command.length();
    }

    String commandName = command.substring(0, spaceIndex);
    if (this->commandsList.find(commandName) != this->commandsList.end()) {
        std::vector<String> argv;
        if (hasArgs) { // Split the string into arguments, if it has any
            splitString(command.substring(spaceIndex), &argv);
        }
        Serial.println("Running command + " + commandName + " with " + String(argv.size()) + " arguments");
        this->commandsList[commandName](this, &argv);
    }
    else {
        Serial.println(commandName + " named Command not found");
        this->print("Command not found");
    }
}

void CloudSerialSystem::debugPrint(String message) {
    if (firstRun) {
        this->preferences.begin("cloudSerial", false);
        this->debug = this->preferences.getBool("debug", true);
        firstRun = false;
        this->debugPrint("Debug mode: " + String(this->debug ? "true" : "false"));
        this->preferences.end();
    }
    
    if (this->debug) {
        this->print("DEBUG - " + message); // only print to cloudSerial if debug is enabled
    }
    Serial.println("DEBUG - " + message); // Always print to serial
}

void CloudSerialSystem::setDebug(bool debug) {
    this->debug = debug;
    this->preferences.begin("cloudSerial", false);
    this->preferences.putBool("debug", debug);
    this->preferences.end();
}

bool CloudSerialSystem::getDebug() {
    return this->debug;
} 

void CloudSerialSystem::handlePrintQueue() {
    if (this->printBuffer.size() > 0) {
        String message = this->printBuffer.front();
        this->printBuffer.pop();
        this->cloudSerialObject->operator= (message);
    }
}

#endif
