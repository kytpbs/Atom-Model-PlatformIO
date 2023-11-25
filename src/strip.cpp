#include "strip.h"

void setPixel(Adafruit_NeoPixel *strip, int index, uint32_t color) {
    strip->setPixelColor(index % strip->numPixels(), color);
}

void flushColor(Adafruit_NeoPixel *strip, uint32_t color) {
    strip->fill(color, 0, strip->numPixels());
    strip->show();
}

void moveColorFowardOnceLib(Adafruit_NeoPixel *strip, uint32_t color, uint32_t backgroundColor, int index, int numPixels, int pixelSpace) {
    strip->clear(); // Clear the strip
    strip->fill(backgroundColor, 0); // Set the background color
    for (int i = 0; i < numPixels; i++) {
        setPixel(strip, i * pixelSpace + index, color);
    }
    strip->show();
}


/* ELECTRONS CLASS */
NeoElectrons::NeoElectrons(): Adafruit_NeoPixel() {
    this->pixelSpace = DEFAULTPIXELSPACE;
}

NeoElectrons::NeoElectrons(uint16_t n, int16_t p, neoPixelType t): Adafruit_NeoPixel(n, p, t) {
    this->pixelSpace = DEFAULTPIXELSPACE;
}

NeoElectrons::NeoElectrons(uint16_t n, int16_t p): Adafruit_NeoPixel(n, p) {
    this->pixelSpace = DEFAULTPIXELSPACE;
}

NeoElectrons::NeoElectrons(uint16_t n, int16_t p, neoPixelType t, int pixelSpace): Adafruit_NeoPixel(n, p, t) {
    this->pixelSpace = pixelSpace;
}

NeoElectrons::NeoElectrons(uint16_t n, int16_t p, int pixelSpace): Adafruit_NeoPixel(n, p) {
    this->pixelSpace = pixelSpace;
}

void NeoElectrons::setup(int brightness) {
    this->begin(); // This initializes the strip
    this->setBrightness(brightness); // Set the brightness of the strip
    this->clear(); // Set all pixel colors to 'off'
    this->show(); // Initialize all pixels to 'off'
}

void NeoElectrons::setup() {
    setup(BRIGHTNESS);
}

void NeoElectrons::setPixel(int index, uint32_t color) {
    setPixelColor(index % numPixels(), color);
}

void NeoElectrons::flushColor(uint32_t color) {
    fill(color);
    show();
}

int NeoElectrons::moveColorFowardOnce(uint32_t color, uint32_t backgroundColor, int activePixelAmount) {
    if (isBlinking()) {
        return electronIndex; // If the strip is blinking, don't move the electrons
    }

    if (pixelSpace == DEFAULTPIXELSPACE) {
        pixelSpace = numPixels() / activePixelAmount;
    }

    // Move the electrons foward
    moveColorFowardOnceLib(this, color, backgroundColor, electronIndex, activePixelAmount, pixelSpace);
    
    // Increase the electron index
    this->electronIndex = (electronIndex + 1) % numPixels();
    return electronIndex;
}

int NeoElectrons::moveColorFowardOnce(uint32_t color, uint32_t backgroundColor) {
    return moveColorFowardOnce(color, backgroundColor, electronAmount);
}

int NeoElectrons::moveColorFowardOnce() {
    return moveColorFowardOnce(electronColor, backgroundColor);
}

void NeoElectrons::setColors(uint32_t electronColor, uint32_t backgroundColor) {
    this->electronColor = electronColor;
    this->backgroundColor = backgroundColor;
}

void NeoElectrons::updateBlink(unsigned int delay) {
    if (isBlinking()) {
        // If the blink has finished
        if (millis() - blinkStartTime >= delay) {
            blinkStartTime = millis();
            runBlink();
            blinkTimes--;
        }
    }
}

void NeoElectrons::runBlink() {
    if (isBlinking()) {
        if (blinkState) {
            flushColor(blinkColor);
        }
        else {
            clear();
            show();
        }
        blinkState = !blinkState;
        show();
    }
}
