////
/// LedDriver.h - common defs for pproject 
//

// Screen controls definitions
#define OffButtonPin V0
#define OnButtonPin V1
#define NightButtonPin V2
#define YellowSliderPin V3
#define WhiteSliderPin V4

// Alarm clock
#define AlarmSwitchPin V5
#define AlaramTimePin V6
#define RiseTimePin V7
#define AlarmYellowSliderPin V8
#define AlarmWhiteSliderPin V9
#define AlarmCurrentButtonPin V10

#define SLIDER_MAXVALUE 1023
#define SLIDER_MINVALUE 0

#define YellowChannel 0
#define WhiteChannel 1

#define PrefsMagic 0x55aa22b2L
#define PrefsFilePath "/preferences.bin"
#define PrefsSaveTimeout 10000

#define BunttonTapState 0

enum LedsPowerState {
  On, Off, Night
};


void ledsOff();
void ledsRestore();
void setButtonsColor();

void savePrefs();
void restorePrefs();

void ledSetup();
void setBrightness(int bright, int channel);
void nightLedOn(bool on);

void cancelAlarm();
