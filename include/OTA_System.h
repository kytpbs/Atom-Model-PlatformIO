#pragma once
#if !defined(OTA_SYSTEM_H) && (defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266))
#define OTA_SYSTEM_H
#include "serialCommandsSystem.h"

#if __has_include("arduino_secrets.h")
    #include "arduino_secrets.h"
#else
    #define USERNAME "admin"
    #define PASSWORD "admin"
#endif

#include "arduino_secrets.h"
#ifdef ARDUINO_ARCH_ESP8266
    #include <ESP8266WebServer.h>
    #include <ESP8266WiFi.h>
#else // We are on an ESP32
    #include <WiFi.h>
    #include <WebServer.h>
#endif

#include <ElegantOTA.h>

/**
 * @brief Setup the OTA update system
 * @param cloudCLI The cloudCLI object that will be used to print to the cloud
*/
void setupOTA(CloudSerialSystem* cloudCLI); // request the cloudCLI object so we can print to it

/**
 * @brief Handle the OTA update system. This should be called in the cloud loop.
*/
void handleOTA();

/**
 * @brief Called when the OTA update starts
 * @note You shouldn't need to call this function yourself
*/
void onOTAStart();

/**
 * @brief Called when the OTA update ends
 * @note You shouldn't need to call this function yourself
*/
void onOTAEnd(bool success);

#endif
