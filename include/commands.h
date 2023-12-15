#pragma once
#if !defined(COMMANDS_H) && (defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)) // STD does not exist on Arduino
#define COMMANDS_H
#include "serialCommandsSystem.h"
#include "stringTools.h"
#include "Constants.h"
#ifdef ARDUINO_ARCH_ESP32
#include "WiFi.h"
#elif defined(ARDUINO_ARCH_ESP8266)
#include "ESP8266WiFi.h"
#endif
#include <vector>

#define command(name) void name(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv)

void ping(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv);

void echo(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv);

command(reboot);

command(getIP);

void switchElectronOnStrips(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv);

void blinkStrips(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv);

void setupCommands(CloudSerialSystem* cloudSerialSystem);

#endif
