#pragma once
#include "stripVariables.h"
#include "Constants.h"

#ifdef ARDUINO_ARCH_ESP32
namespace ESP32_utils {
    void smallStripLoop(void* pvParameters);
    void innerStrip1Loop(void* pvParameters);
    void innerStrip2Loop(void* pvParameters);
    void outerStrip1Loop(void* pvParameters);
    void outerStrip2Loop(void* pvParameters);
    void outerStrip3Loop(void* pvParameters);

    void smallStripRun(void* pvParameters);
    void innerStrip1Run(void* pvParameters);
    void innerStrip2Run(void* pvParameters);
    void outerStrip1Run(void* pvParameters);
    void outerStrip2Run(void* pvParameters);
    void outerStrip3Run(void* pvParameters);
};
#endif

namespace stripCommands {

    /**
     * @brief Sets up the LED strips.
     */
    void setupStrips();

    /**
     * @brief Runs the switch function handling timers.
     * @note This function should be called instead of switchPixel() because it updates the timers and checks.
     */
    void runSwitch();

    /**
     * @brief Moves the electron forward on all the strips.
     */
    void moveElectronForward();

    /**
     * @brief Updates the blinking on all the strips.
     * @warning This function should be called in the loop function for the blinking to work.
     */
    void updateBlinks();

    /**
     * @brief Blinks all the LED strips a specified number of times.
     * @param times The number of times to blink the LED strips.
     */
    void blinkAll(int times);

    /**
     * @brief Switches the electrons between the inner and outer strip.
     */
    void switchPixel();

    /**
     * @brief Sets the brightness of the LED strips.
     * @param brightness The brightness to set the LED strips to.
    */
    void setBrightness(int brightness);

    /**
     * @brief Checks if the electrons should switch between the inner and outer strip.
     * @return True if the electrons should switch, false otherwise.
     */
    bool shouldSwitch();

#ifdef ARDUINO_ARCH_ESP32
    /**
     * @brief Create tasks for each strip
     * @param isInfinite Whether the tasks should be infinite or not, defaults to true
     * If infinite, the tasks will loop forever, if not, they will run once and then delete themselves
     * The recommended value is true, as it will run the tasks forever, and will not delete them
     * @param stackSize The stack size of the tasks
     * @param priority The priority of the tasks
     * @note This function should only be called once, if isInfinite is true, 
     * or else you will get a memory leak and a crash (if you call it more than once).
     * @details This function will pin the tasks to core 1.
    */
    void createTasksForEach(bool isInfinite = true, int stackSize = 2048, int priority = 2);

    /**
     * @brief Creates tasks for each strip that are not infinite and self-killing.
     * @param stackSize The stack size of the tasks.
     * @param priority The priority of the tasks.
     * @note This function will automatically kill the tasks after they are done.
     * @details This function will pin the tasks to core 1.
    */
    void _createTasks(int stackSize, int priority);

    /**
     * @brief Creates tasks for each strip that are infinite long.
     * @param stackSize The stack size of the tasks.
     * @param priority The priority of the tasks.
     * @note This function will not kill the tasks after they are done, as they are infinite and will never be done.
     * @warning This function should only be called once, or else you will get a memory leak and a crash
     * because the same tasks will be created multiple times.
     * @details This function will pin the tasks to core 1.
    */
    void _createTasksInfinite(int stackSize, int priority); //! Will pin to core 1! (Core 0 is used for WiFi, and other stuff, DON'T PIN TO CORE 0!)
#endif
}
