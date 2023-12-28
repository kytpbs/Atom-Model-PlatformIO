file_path = ".pio/libdeps/esp32/ArduinoIoTCloud/src/AIoTC_Config.h"
test_script = "sed -i 's/define OTA_ENABLED/\/\/define OTA_ENABLED/g' " + file_path # made by copilot... I wouldn't recommend using it anyways.

def disableOTA():
    print("Disabling OTA...")
    with open(file_path, "r") as f:
        lines = f.readlines()
    with open(file_path, "w") as f:
        for line in lines:
            if "define OTA_ENABLED" in line and not "//" in line:
                line = "//" + line
            f.write(line)
    print("OTA disabled")


def enableOTA():
    """
    This function enables Arduino IoT Cloud OTA.
    Will probably be never used, but it's here just in case.
    """
    print("Enabling OTA...")
    with open(file_path, "rw") as f:
        lines = f.readlines()
        for line in lines:
            if "//define OTA_ENABLED" in line:
                line = line.replace("//", "")
            f.write(line)
    print("OTA enabled")

def before_build(source, target, env):
    disableOTA()

# env.AddPreAction("buildprog", before_build)
# This didn't work, so I will directly call the function, I know it's not the best practice, but it works.
before_build(None, None, None)
