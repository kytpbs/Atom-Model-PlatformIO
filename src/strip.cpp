#include "strip.h"

int moduloEuclidean(int a, int b) {
  int m = a % b;
  if (m < 0) {
    // m += (b < 0) ? -b : b; // avoid this form: it is UB when b == INT_MIN
    m = (b < 0) ? m - b : m + b;
  }
  return m;
}

void setPixel(Adafruit_NeoPixel *strip, int index, uint32_t color) {
    strip->setPixelColor(moduloEuclidean(index, strip->numPixels()), color);
}

void flushColor(Adafruit_NeoPixel *strip, uint32_t color) {
    strip->fill(color, 0, strip->numPixels());
    strip->show();
}

uint32_t mixColors(uint32_t Color1, uint32_t Color2, float mixLevel) {
    uint8_t r1 = (Color1 >> 16) & 0xFF;
    uint8_t g1 = (Color1 >> 8) & 0xFF;
    uint8_t b1 = Color1 & 0xFF;
    uint8_t r2 = (Color2 >> 16) & 0xFF;
    uint8_t g2 = (Color2 >> 8) & 0xFF;
    uint8_t b2 = Color2 & 0xFF;
    uint8_t r = (uint8_t)(r1 * mixLevel + r2 * (1 - mixLevel));
    uint8_t g = (uint8_t)(g1 * mixLevel + g2 * (1 - mixLevel));
    uint8_t b = (uint8_t)(b1 * mixLevel + b2 * (1 - mixLevel));
    return Adafruit_NeoPixel::Color(r, g, b);
}

void moveColorForwardOnceLib(Adafruit_NeoPixel *strip, uint32_t color, uint32_t backgroundColor, int index, int numPixels, int pixelSpace) {
    strip->clear(); // Clear the strip
    strip->fill(backgroundColor, 0); // Set the background color
    // mix background color with color
    for (int i = 0; i < numPixels; i++) {
        setPixel(strip, i * pixelSpace + index, color);
        // setPixel(strip, i * pixelSpace + index - 2, mixColors(color, backgroundColor, 0.3));
        // setPixel(strip, i * pixelSpace + index - 1, mixColors(color, backgroundColor, 0.6));
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

int NeoElectrons::moveColorForwardOnce(uint32_t color, uint32_t backgroundColor, int activePixelAmount) {
    if (isBlinking()) {
        return electronIndex; // If the strip is blinking, don't move the electrons
    }
    int currentPixelSpace;
    if (this->pixelSpace == DEFAULTPIXELSPACE) {
        currentPixelSpace = numPixels() / activePixelAmount;
    }
    else {
        currentPixelSpace = this->pixelSpace;
    }
    // Move the electrons forward
    moveColorForwardOnceLib(this, color, backgroundColor, electronIndex, activePixelAmount, currentPixelSpace);
    
    // Increase the electron index
    this->electronIndex = (electronIndex + 1) % numPixels();
    return electronIndex;
}

int NeoElectrons::moveColorForwardOnce(uint32_t color, uint32_t backgroundColor) {
    return moveColorForwardOnce(color, backgroundColor, electronAmount);
}

int NeoElectrons::moveColorForwardOnce() {
    return moveColorForwardOnce(electronColor, backgroundColor);
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
        }
        blinkState = !blinkState;
        show();
    }
}
