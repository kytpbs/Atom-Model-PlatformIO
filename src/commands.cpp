#include "commands.h"
#ifdef COMMANDS_H // only include this file if commands.h is included

void ping(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv) {
    cloudSerialSystem->print("pong");
}

void echo(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv) {
    if (argv->size() == 0) {
        cloudSerialSystem->print("No arguments provided!");
        return;
    }
    cloudSerialSystem->print(joinString(argv, " "));
}

void switchElectronOnStrips(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv) {
    stripCommands::runSwitch();
    cloudSerialSystem->print("Running Switch!");
}

void blinkStrips(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv) {
    int blinkAmount = BLINKAMOUNT;
    if (argv->size() > 0) {
        blinkAmount = argv->at(0).toInt();
        if (blinkAmount == 0) {
            cloudSerialSystem->print("Invalid argument! blinking " + String(BLINKAMOUNT) + " times instead.");
            blinkAmount = BLINKAMOUNT;
        }
    }
    stripCommands::blinkAll(blinkAmount);
    cloudSerialSystem->print("Blinking " + String(blinkAmount) + " times.");
}

void setDebugMode(CloudSerialSystem* cloudSerialSystem, std::vector<String>* argv) {
    if (argv->size() == 0) {
        cloudSerialSystem->print("No arguments provided! Current debug mode: " + String(cloudSerialSystem->getDebug() ? "true" : "false"));
        return;
    }
    String argument = argv->at(0);
    if (argument == "true" || argument == "on") {
        cloudSerialSystem->setDebug(true);
        cloudSerialSystem->print("Debug mode set to true");
        return;
    }
    if (argument == "false" || argument == "off") {
        cloudSerialSystem->setDebug(false);
        cloudSerialSystem->print("Debug mode set to false");
        return;
    }
    cloudSerialSystem->print("Invalid argument! Not changing debug mode. Current debug mode: " + String(cloudSerialSystem->getDebug() ? "true" : "false"));
}

command(reboot) {
    cloudSerialSystem->print("Rebooting...");
    ESP.restart();
}

command(getIP) {
    cloudSerialSystem->print(WiFi.localIP().toString());
}

void setupCommands(CloudSerialSystem* cloudSerialSystem) {
    cloudSerialSystem->addCommand("ping", ping);
    cloudSerialSystem->addCommand("echo", echo);
    cloudSerialSystem->addCommand("switch", switchElectronOnStrips);
    cloudSerialSystem->addCommand("blink", blinkStrips);
    cloudSerialSystem->addCommand("debug", setDebugMode);
    cloudSerialSystem->addCommand("reboot", reboot);
    cloudSerialSystem->addCommand("ip", getIP);
}

#endif
