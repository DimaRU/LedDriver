////
///  Preferences.c
//

#include "FS.h"
#include "SPIFFS.h"

struct Preferences {
    uint32_t magic;
    int yellowSlider;
    int whiteSlider;
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
    currentPrefs.whiteSlider = whiteSlider;
    currentPrefs.yellowSlider = yellowSlider;
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
        yellowSlider = LEDC_MAXVALUE;
        whiteSlider = LEDC_MAXVALUE;
    }

    powerState = savedPrefs.powerState;
    yellowSlider = savedPrefs.yellowSlider;
    whiteSlider = savedPrefs.whiteSlider;

    digitalWrite(NightLedPin, LOW);
    switch(powerState) {
        case On:
            ledsRestore();
            break;
        case Off:
            ledsOff();
            break;
        case Night:
            digitalWrite(NightLedPin, HIGH);
            break;
    }
}
