#include "stripVariables.h"
#include "Constants.h"

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
     * @brief Checks if the electrons should switch between the inner and outer strip.
     * @return True if the electrons should switch, false otherwise.
     */
    bool shouldSwitch();

    /**
     * @brief Gets the timestamp of the last switch.
     * @return The timestamp of the last switch.
     */
    unsigned long getLastSwitchTime();
}
