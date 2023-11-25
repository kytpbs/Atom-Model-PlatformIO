#include <Adafruit_NeoPixel.h>
#include "Constants.h"

#ifndef STRIPS_H
#define STRIPS_H


/**
 * Sets the pixel at the given index to the given color
 * Uses circular indexing, so if the index is greater than the number of pixels, it will wrap around
 * Will not show the pixel unlike other functions. You must call strip.show() after calling this function
*/
void setPixel(Adafruit_NeoPixel *strip, int index, uint32_t color);

/**
 * Sets all the pixels to the given color
 * It will show the pixels unlike the setPixel function
 * @param STRIP: The strip to change the pixels of
 * @param COLOR: The color to set the pixels to
*/
void flushColor(Adafruit_NeoPixel *strip, uint32_t color);

/**
 * Moves the red color pixel by pixel from the back to the front removing the red color from the back
 * @param INDEX: The index of the pixel to light up
 * @param NUMPIXELS: The number of pixels in the strip will be used to wrap around the index, will leave an empty between them (might change later, should I?)
*/
void moveColorFowardOnceLib(Adafruit_NeoPixel *strip, uint32_t color, uint32_t backgroundColor, int index, int numPixels, int pixelSpace);

class NeoElectrons: public Adafruit_NeoPixel{
    private:
        unsigned long blinkStartTime = 0;
        unsigned int blinkTimes = 0;
        bool blinkState = false;
    public:
        uint32_t electronColor;
        uint32_t backgroundColor;
        uint32_t blinkColor = Color(BLINKRED, BLINKGREEN, BLINKBLUE);
        int blinkWaitTime = BLINKDELAY;
        int pixelSpace;
        int electronAmount;
        int electronIndex = 0;
        NeoElectrons();
        NeoElectrons(uint16_t n, int16_t p, neoPixelType t, int pixelSpace);
        NeoElectrons(uint16_t n, int16_t p, neoPixelType t);
        NeoElectrons(uint16_t n, int16_t p, int pixelSpace);
        NeoElectrons(uint16_t n, int16_t p);
        void setup();
        void setup(int brightness);
        /**
         * Sets the pixel at the given index to the given color
         * Uses circular indexing, so if the index is greater than the number of pixels, it will wrap around
         * Will not show the pixel unlike other functions. You must call strip.show() after calling this function
        */
        void setPixel(int index, uint32_t color);
        /**
         * Sets all the pixels to the given color
         * It will show the pixels unlike the setPixel function
         * @param STRIP: The strip to change the pixels of
         * @param COLOR: The color to set the pixels to
        */
        void flushColor(uint32_t color);
        /**
         * Sets the colors of the electron and the background
        */
        void setColors(uint32_t electronColor, uint32_t backgroundColor);
        /**
         * Moves the electron foward once one pixel from the index
         * Call this function periodically to move the electron foward
         * @return The Index of the first electron
        */
        int moveColorFowardOnce(uint32_t color, uint32_t backgroundColor, int pixelAmount);
        /**
         * Moves the electron foward once one pixel from the index
         * Call this function periodically to move the electron foward
         * @return The Index of the first electron
        */
        int moveColorFowardOnce(uint32_t color, uint32_t backgroundColor);
        /**
         * Moves the electron foward once one pixel from the index
         * Call this function periodically to move the electron foward
         * @return The Index of the first electron 
        */
        int moveColorFowardOnce();

        /**
         * Gets the index of the electron
         * @return The index of the first electron
        */
        int getElectronIndex() {return electronIndex;}

        /* BLINK FUNCTIONS */
        
        /**
         * Starts the blink.
        */
        void startBlink(int times = 1) {blinkTimes = times; blinkStartTime = millis();}

        /**
         * Checks if it should be blinking and blinks if it should
         * Should be called in the loop function for the blink to work
        */
        void updateBlink(unsigned int delay);

        /**
         * Checks if it should be blinking and blinks if it should
         * Should be called in the loop function for the blink to work
        */
        void updateBlink() {updateBlink(blinkWaitTime);}

        /**
         * Actually runs the blink command,
         * Will be called by the updateBlink function 
         * so it is recoomended to use that instead
        */
        void runBlink();
        
        /**
         * Uses the blinkTimes variable to check if it is currently blinking
         * @return If it is currently blinking, returns true, otherwise returns false
        */
        bool isBlinking() {return blinkTimes > 0;}
        
        /**
         * Sets the index of the electron
         * @param electronIndex: The index of the first electron
        */
        void setElectronIndex(int electronIndex) {this->electronIndex = electronIndex;}

        /**
         * Sets the color of the Electron
         * @param color: The color of the electron using the Color function
        */
        void setElectronColor(uint32_t color) {this->electronColor = color;}

        /**
         * Sets the background color of the Strip
         * @param color: The color of the background using the Color function
        */
        void setBackgroundColor(uint32_t color) {this->backgroundColor = color;}

        /**
         * Sets the amount of electrons in the strip
         * @param electronAmount: The amount of electrons in the strip
        */
        void setElectronAmont(int electronAmount) {this->electronAmount = electronAmount;}
        
        /**
         * Increases the amount of electrons in the strip by one
        */
        void increaseElectronAmount() {this->electronAmount++;}

        /**
         * Decreases the amount of electrons in the strip by one
        */
        void decreaseElectronAmount() {this->electronAmount--;}
};

#endif
