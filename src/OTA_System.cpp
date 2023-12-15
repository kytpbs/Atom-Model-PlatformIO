#include "OTA_System.h"
#ifdef OTA_SYSTEM_H
#ifdef ARDUINO_ARCH_ESP8266
    ESP8266WebServer server(80);
#else // We are on an ESP32
    WebServer server(80);
#endif
CloudSerialSystem* cloudCommandLine;

void setupOTA(CloudSerialSystem* cloudCLISystem) {
    cloudCommandLine = cloudCLISystem;
    
    Serial.println("Setting up Server...");
    cloudCommandLine->debugPrint("Connected to WiFi: \"" + WiFi.SSID() + "\"");
    cloudCommandLine->print("CURRENT IP: " + WiFi.localIP().toString());
    Serial.println("CURRENT IP: " + WiFi.localIP().toString());
    
    server.on("/", []() {
        server.sendHeader("Location", "/update");
        server.send(302);
    });
    
    Serial.println("Done setting up Server!");
    
    ElegantOTA.setAuth(USERNAME, PASSWORD);

    ElegantOTA.begin(&server);    // Start ElegantOTA

    server.begin();
    cloudCommandLine->debugPrint("OTA/HTTP server started");
    ElegantOTA.onStart(onOTAStart);
    ElegantOTA.onEnd(onOTAEnd);
}

void handleOTA() {
    server.handleClient();
    ElegantOTA.loop();
    delay(50); // add a delay so that we don't get a WDT reset, was a pain to debug... DO NOT REMOVE!
}

void onOTAStart() {
    cloudCommandLine->print("OTA UPLOAD STARTED");
}

void onOTAEnd(bool success) {
    if (success) {
        cloudCommandLine->print("OTA UPLOAD COMPLETE! REBOOTING...");
    }
    else {
        cloudCommandLine->print("OTA Failed PLEASE TRY AGAIN");
    }
}

#endif
