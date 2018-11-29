////
///  Preferences.c
//

#include "FS.h"
#include "SPIFFS.h"

struct Preferences {
    uint32_t magic;
    int yellowLevel;
    int whiteLevel;
    enum LedsPowerState powerState;
};

static struct Preferences currentPrefs;
static struct Preferences savedPrefs;

static void writePrefs() {
    File file = SPIFFS.open(PrefsFilePath, FILE_WRITE);
    if (!file) {
        Serial.println("- failed to open preferences file for writing");
        return;
    }
    
    if ( file.write((uint8_t *)&currentPrefs, sizeof(currentPrefs)) ) {
        Serial.println("Preferences written");
    } else {
        Serial.println("Preferences write failed");
    }
    file.close();
}

void savePrefs() {
    currentPrefs.whiteLevel = whiteLevel;
    currentPrefs.yellowLevel = yellowLevel;
    currentPrefs.powerState = powerState;

    if (memcmp(&currentPrefs, &savedPrefs, sizeof(currentPrefs)) == 0) return;
    writePrefs();
    memcpy(&savedPrefs, &currentPrefs, sizeof(currentPrefs));
    savedPrefs = currentPrefs;
}


void restorePrefs() {

    currentPrefs.magic = PrefsMagic;

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    Serial.print("Reading preferences");

    File file = SPIFFS.open(PrefsFilePath);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open preferences file for reading");
        return;
    }

    if ( file.read((uint8_t *)&savedPrefs, sizeof(savedPrefs)) ) {
        Serial.println("- file read");
    } else {
        Serial.println("- read failed");
    }

    if (savedPrefs.magic != PrefsMagic) {
        powerState = On;
        yellowLevel = SLIDER_MAXVALUE /2;
        whiteLevel = SLIDER_MAXVALUE / 2;
    }

    powerState = savedPrefs.powerState;
    yellowLevel = savedPrefs.yellowLevel;
    whiteLevel = savedPrefs.whiteLevel;

    nightLedOn(false);
    switch(powerState) {
        case On:
            ledsRestore();
            break;
        case Off:
            ledsOff();
            break;
        case Night:
            nightLedOn(true);
            break;
    }
}
