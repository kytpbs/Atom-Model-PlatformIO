#include "stripCommands.h"

unsigned long lastSwitchTime = 0; // Create a variable to store the last switch time

namespace stripCommands {
    void setupStrips() {
        // Setup inner strips
        Serial.println("Setting up inner strip1...");
        innerStrip.setup(BRIGHTNESS); // This initializes the strip
        innerStrip.setColors(innerStrip.Color(innerRED, innerGREEN, innerBLUE), innerStrip.Color(innerBACKGROUNDRED, innerBACKGROUNDGREEN, innerBACKGROUNDBLUE)); // Set the colors of the strip
        innerStrip.setElectronAmount(INPIXELAMOUNT); // Set the amount of electrons in the strip

        Serial.println("Setting up inner strip2...");
        innerStrip2.setup(BRIGHTNESS);
        innerStrip2.setColors(innerStrip2.Color(innerRED, innerGREEN, innerBLUE), innerStrip2.Color(innerBACKGROUNDRED, innerBACKGROUNDGREEN, innerBACKGROUNDBLUE)); // Set the colors of the strip
        innerStrip2.setElectronAmount(INPIXELAMOUNT); // Set the amount of electrons in the strip

        // Setup outer strip
        Serial.println("Setting up outer strip 1...");
        outerStrip.setup(BRIGHTNESS); // This initializes the strip
        outerStrip.setColors(outerStrip.Color(outerRED, outerGREEN, outerBLUE), outerStrip.Color(outerBACKGROUNDRED, outerBACKGROUNDGREEN, outerBACKGROUNDBLUE)); // Set the colors of the strip
        outerStrip.setElectronAmount(OUTPIXELAMOUNT); // Set the amount of electrons in the strip

        Serial.println("Setting up outer strip 2...");
        outerStrip2.setup(BRIGHTNESS);
        outerStrip2.setColors(outerStrip2.Color(outerRED, outerGREEN, outerBLUE), outerStrip2.Color(outerBACKGROUNDRED, outerBACKGROUNDGREEN, outerBACKGROUNDBLUE)); // Set the colors of the strip
        outerStrip2.setElectronAmount(OUTPIXELAMOUNT); // Set the amount of electrons in the strip

        Serial.println("Setting up outer strip 3...");
        outerStrip3.setup(BRIGHTNESS);
        outerStrip3.setColors(outerStrip3.Color(outerRED, outerGREEN, outerBLUE), outerStrip3.Color(outerBACKGROUNDRED, outerBACKGROUNDGREEN, outerBACKGROUNDBLUE)); // Set the colors of the strip
        outerStrip3.setElectronAmount(OUTPIXELAMOUNT); // Set the amount of electrons in the strip
        // Setup small strip
        Serial.println("Setting up small strip...");
        smallStrip.setup(BRIGHTNESS); // This initializes the strip
        smallStrip.setColors(smallStrip.Color(smallRED, smallGREEN, smallBLUE), smallStrip.Color(smallBACKGROUNDRED, smallBACKGROUNDGREEN, smallBACKGROUNDBLUE)); // Set the colors of the strip
        smallStrip.setElectronAmount(SMALLPIXELAMOUNT); // Set the amount of electrons in the strip
        Serial.println("Done setting up strips!");
    }

    void runSwitch() {
        Serial.println("Blinking! as " + String(SWITCHTIME) + " Seconds have passed."); // Print that we are blinking
        lastSwitchTime = millis(); // Set the last switch time to the current time
        blinkAll(BLINKAMOUNT); // Blink the pixels
        switchPixel(); // Switch the pixels
        return;
    }

    void moveElectronForward() {
        innerStrip.moveColorForwardOnce();
        innerStrip2.moveColorForwardOnce();
        outerStrip.moveColorForwardOnce();
        outerStrip2.moveColorForwardOnce();
        outerStrip3.moveColorForwardOnce();
        smallStrip.moveColorForwardOnce();
    }

    void updateBlinks() {
        innerStrip.updateBlink();
        innerStrip2.updateBlink();
        outerStrip.updateBlink();
        outerStrip2.updateBlink();
        outerStrip3.updateBlink();
        smallStrip.updateBlink();
    }

    void blinkAll(int times) {
        innerStrip.startBlink(times);
        outerStrip.startBlink(times);
    }

    void switchPixel() {
    if (innerStrip.electronAmount <= outerStrip.electronAmount) {
        innerStrip.increaseElectronAmount();
        outerStrip.decreaseElectronAmount();
    }
    else {
        innerStrip.decreaseElectronAmount();
        outerStrip.increaseElectronAmount();
    }
    }

    bool shouldSwitch() {
        return millis() - lastSwitchTime >= SWITCHTIME * 1000;
    }

    void setBrightness(int brightness) {
        smallStrip.setBrightness(brightness);
        
        innerStrip.setBrightness(brightness);
        innerStrip2.setBrightness(brightness);
        
        outerStrip.setBrightness(brightness);
        outerStrip2.setBrightness(brightness);
        outerStrip3.setBrightness(brightness);
    }
}
