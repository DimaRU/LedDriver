////
/// LedDriver.h - common defs for pproject 
//

// Screen controls definitions
#define OffButtonPin V0
#define OnButtonPin V1
#define NightButtonPin V2
#define YellowSlider V3
#define WhiteSlider V4
#define BrightSlider V5

#define SLIDER_MAXVALUE 1023
#define SLIDER_MINVALUE 0

#define YellowChannel 0
#define WhiteChannel 1

#define PrefsMagic 0x55aa22bbL
#define PrefsFilePath "/preferences.bin"
#define PrefsSaveTimeout 10000

enum LedsPowerState {
  On, Off, Night
};

//extern enum LedsPowerState powerState;
//extern int yellowSlider;
//extern int whiteSlider;

void ledsOff();
void ledsRestore();
void setButtonsColor();

void savePrefs();
void restorePrefs();

void ledSetup();
void setBrightness(int bright, int channel);
void nightLedOn(bool on);
