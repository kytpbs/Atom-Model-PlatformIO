#include <Arduino.h>

#include "Constants.h"
#include "stripCommands.h"

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) // only include if we are on an ESP
#include "thingProperties.h"
#include "serialCommandsSystem.h"
#include "commands.h"
#include "OTA_System.h"
#ifdef ARDUINO_ARCH_ESP32
#include "WiFi.h"
#elif defined(ARDUINO_ARCH_ESP8266)
#include "ESP8266WiFi.h"
#endif
CloudSerialSystem cloudCLI(&cloudSerial); // Create the cloud serial system object that will handle the cloud serial commands
bool connectedToCloud = false;
#define debugPrint(...) cloudCLI.debugPrint(__VA_ARGS__)
#define debugPrintln(...) debugPrint(__VA_ARGS__)
#else
#define debugPrintln(x) Serial.println(x)
#endif

#ifdef ARDUINO_ARCH_ESP8266
unsigned long lastCloudUpdate = 0;
#endif

NeoElectrons smallStrip (SMALLNUMPIXELS, SMALLDATA, NEO_GRB + NEO_KHZ800); // Create the small strip object
NeoElectrons innerStrip (INNUMPIXELS, INDATA, NEO_GRB + NEO_KHZ800);       // Create the inner strip object
NeoElectrons innerStrip2(INNUMPIXELS, INDATA2, NEO_GRB + NEO_KHZ800);      // Create the second inner strip object
NeoElectrons outerStrip (OUTNUMPIXELS, OUTDATA, NEO_GRB + NEO_KHZ800);     // Create the outer strip object
NeoElectrons outerStrip2(OUTNUMPIXELS, OUTDATA2, NEO_GRB + NEO_KHZ800);    // Create the second outer strip object
NeoElectrons outerStrip3(OUTNUMPIXELS, OUTDATA3, NEO_GRB + NEO_KHZ800);    // Create the third outer strip object

bool builtinLedState = false; // The state of the builtin led

/* START OF FUNCTIONS DEFINITONS */
#ifdef ARDUINO_ARCH_ESP8266
void cloudLoop();
void OTALoop(); //! not implemented yet
#elif defined(ARDUINO_ARCH_ESP32)
void cloudLoop(void *pvParameters);
void OTALoop(void *pvParameters);
#else
void blinkbuiltinled();
#endif


#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) // only include if we are on an ESP
void cloudSetup();
void onCloudSync();
void onCloudDisconnect();
void onInnerStripColorChange();
void onOuterStripColorChange();
void onSmallStripColorChange();
void onLedCountsChange();
void onCloudSerialChange();
#else
void blinkBuiltInLed();
#endif

void setup() {
  Serial.begin(115200); // Start the serial connection
  while (!Serial) { // Wait for the serial connection to be established.
    if (millis() > 3000) {
      break; // If we don't get a serial connection in 3 seconds, stop waiting.
    }
    delay(100);
  }
  Serial.println("Serial connection established.");
  Serial.println("Setting up strips...");
  stripCommands::setupStrips();

#ifdef ARDUINO_ARCH_ESP32
  Serial.println("On ESP, ENABLING CLOUD");
  cloudSetup();
#elif defined(ARDUINO_ARCH_ESP8266)
  Serial.println("On ESP8266, ENABLING CLOUD (in main loop)");
  cloudSetup();
#else
  Serial.println("On Unknown, NOT ENABLING CLOUD");
#endif
}


/* CLOUD SETUPS */
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) // only include if we are on an ESP

void onCloudSync() {
  Serial.println("Syncing with cloud Completed!");
  connectedToCloud = true;
}

void onCloudDisconnect() {
  Serial.println("Disconnected from cloud!");
  connectedToCloud = false;
}

void cloudSetup() {
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, onCloudSync);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, onCloudDisconnect);

  // Set the colors of the strips
  innerStripColor.getCloudValue().setColorRGB(innerRED, innerGREEN, innerBLUE);
  innerStripColor.updateLocalTimestamp();
  outerStripColor.getCloudValue().setColorRGB(outerRED, outerGREEN, outerBLUE);
  outerStripColor.updateLocalTimestamp();
  smallStripColor.getCloudValue().setColorRGB(smallRED, smallGREEN, smallBLUE);
  smallStripColor.updateLocalTimestamp();

  // setup all CLI commands
  setupCommands(&cloudCLI);

#ifdef ARDUINO_ARCH_ESP32
  // Multi-threading setup
  Serial.print("Setup currently running on core: ");
  Serial.println(xPortGetCoreID());
  delay(500);
  xTaskCreatePinnedToCore(cloudLoop, "CloudLoop", 20000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(OTALoop, "OTALoop", 20000, NULL, 2, NULL, 0); // OTA loop has a higher priority than the cloud loop, so it will run first
  delay(1000);
#elif defined(ARDUINO_ARCH_ESP8266)
  delay(500);
  // without using FreeRTOS, we can't do multi-threading, so we just run the cloud loop in the main loop, every minute or so
  Serial.println("Running cloud loop in main loop every minute");
#endif
}
#endif

/* DONE SETUP */

void loop() {
  if (stripCommands::shouldSwitch()) { // If it has been SWITCHTIME seconds since the last switch
    stripCommands::runSwitch(); // Run the switch
  }
#ifdef ARDUINO_ARCH_ESP8266
  cloudLoop();
  OTALoop();
#elif defined(ARDUINO_ARCH_ESP32)
  // nothing to do here, cloud loop is running on a separate thread
#else
  blinkBuiltInLed(); // Blink the builtin led to show that the program is running, but not on an ESP as the pin changes from board to board, might change in the future
#endif

  stripCommands::moveElectronForward(); // Move The Electrons Forward
  stripCommands::updateBlinks(); // Run the updateBlink function for all the strips
}

/*   CLOUD FUNCTIONS   */
#if !defined(ARDUINO_ARCH_ESP8266) && !defined(ARDUINO_ARCH_ESP32)
void blinkBuiltInLed() {
  builtinLedState = !builtinLedState;
  digitalWrite(LED_BUILTIN, builtinLedState);
}
#endif

#ifdef ARDUINO_ARCH_ESP8266
void cloudLoop() {
  if (millis() - lastCloudUpdate >= 60000) { // If it has been 60 seconds since the last cloud update
    Serial.println("Updating cloud...");
    ArduinoCloud.update();
    if (connectedToCloud) { // handle Serial Output
      cloudCLI.handlePrintQueue();
    }
    lastCloudUpdate = millis();
  }
}

void OTALoop() {
  handleOTA();
}
#endif

#if defined(ARDUINO_ARCH_ESP32) // only include if we are on an ESP
void OTASetup() {
  Serial.println("Setting up OTA...");
  setupOTA(&cloudCLI);
  Serial.println("Done setting up OTA!");
}

void OTALoop(void *pvParameters) {
  bool firstRun = true;
  Serial.println("OTA Loop currently running on core: " + String(xPortGetCoreID()));
  for (;;) { // infinite loop
    if (firstRun && WiFi.status() == WL_CONNECTED) {
      firstRun = false;
      debugPrintln("Connected to WiFi: \"" + WiFi.SSID() + "\"");
      cloudCLI.print("CURRENT IP: " + WiFi.localIP().toString());
      Serial.println("CURRENT IP: " + WiFi.localIP().toString());
      OTASetup();
    }
    handleOTA(); // this function already calls delay() so we don't need to do it ourselves
  }
}

void cloudLoop(void *pvParameters) {
  Serial.println("Cloud Loop currently running on core: " + String(xPortGetCoreID()));
  for (;;) { // infinite loop
    ArduinoCloud.update();
    if (connectedToCloud) { // handle Serial Output
      cloudCLI.handlePrintQueue();
    }
    // The delay function already calls yield() so we don't need to do it ourselves
    delay(500); // wait half a second, so we don't use too much CPU waiting for the cloud to update. *might increase in the future
  }
}
#endif


#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) // only include if we are on an ESP
void onInnerStripColorChange() {
  Color color = innerStripColor.getValue();
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  color.getRGB(red, green, blue);
  debugPrintln("Changing inner strip color to " + String(red) + ", " + String(green) + ", " + String(blue));
  innerStrip.setElectronColor(innerStrip.Color(red, green, blue));
  innerStrip2.setElectronColor(innerStrip2.Color(red, green, blue));
}

void onOuterStripColorChange() {
  Color color = outerStripColor.getValue();  
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  color.getRGB(red, green, blue);
  debugPrintln("Changing inner strip color to " + String(red) + ", " + String(green) + ", " + String(blue));
  outerStrip.setElectronColor(outerStrip.Color(red, green, blue));
  outerStrip2.setElectronColor(outerStrip2.Color(red, green, blue));
  outerStrip3.setElectronColor(outerStrip3.Color(red, green, blue));
}

void onSmallStripColorChange() {
  Color color = smallStripColor.getValue();  
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  color.getRGB(red, green, blue);
  debugPrintln("Changing inner strip color to " + String(red) + ", " + String(green) + ", " + String(blue));
  smallStrip.setElectronColor(smallStrip.Color(red, green, blue));
}

void onLedCountsChange() {
  String tmpCounts = ledCounts;
  tmpCounts.replace(" ", "");

  int counts[6];
  for(int i = 0; i < 6; i++) {
    int index = tmpCounts.indexOf(",");
    if (index == -1) {
      if (i == 5) {
        index = tmpCounts.length();
      }
      else {
        debugPrintln("Error parsing led counts, not enough commas");
        return;
      }
    }
    counts[i] = tmpCounts.substring(0, index).toInt();
    tmpCounts = tmpCounts.substring(index + 1);
  }

  debugPrintln("Changing led counts to " + String(counts[0]) + ", " + String(counts[1])
   + ", " + String(counts[2]) + ", " + String(counts[3]) + ", " + String(counts[4]) + ", " + String(counts[5]));
  
  smallStrip.updateLength(counts[0]);

  innerStrip.updateLength(counts[1]);
  innerStrip2.updateLength(counts[2]);

  outerStrip.updateLength(counts[3]);
  outerStrip2.updateLength(counts[4]);
  outerStrip3.updateLength(counts[5]);
}

void onCloudSerialChange() { // Will only give the newest message, NICE!
  Serial.println("New cloudSerial Command: " + cloudSerial);
  String tmpSerial = cloudSerial;
  
  if (tmpSerial.isEmpty() || tmpSerial == "" || tmpSerial == " ") {
    return;
  }

  cloudCLI.checkForCommands(tmpSerial);
}
#endif
