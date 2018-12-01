////
///  AlaramClock.ino
//

static void checkAlarm();

enum AlarmState {
  waiting,
  alarming,
  cancelled
};

int alarmYellowLevel = SLIDER_MAXVALUE/4;
int alramWhiteLevel = SLIDER_MAXVALUE/4;
int riseTime = 15;
int alarmHour = 7;
int alarmMinute = 0;
bool alarmEnabled = false;

time_t startTime;
time_t endTime;
enum AlarmState alarmState = waiting;

BLYNK_WRITE(AlarmYellowSliderPin)
{
  int pinValue = param.asInt();
  alarmYellowLevel = pinValue;
}

BLYNK_WRITE(AlarmWhiteSliderPin)
{
  int pinValue = param.asInt();
  alramWhiteLevel = pinValue;
}

BLYNK_WRITE(RiseTimePin)
{
  int pinValue = param.asInt();
  riseTime = pinValue;
  printf("riseTime = %d\n", riseTime);
}

BLYNK_WRITE(AlaramTimePin)
{
  TimeInputParam t(param);

  if (!t.hasStartTime()) return;
  printf("Alaram time: %d:%d\n",
                  t.getStartHour(),
                  t.getStartMinute());

  alarmHour = t.getStartHour();
  alarmMinute = t.getStartMinute();

  alarmState = waiting;
}

BLYNK_WRITE(AlarmSwitchPin)
{
  bool pinValue = (param.asInt() == 1) ? true : false;
  printf("Alarm Switch: %d\n", pinValue);
  alarmEnabled = pinValue;
  if (!alarmEnabled) {
    cancelAlarm();
  }
}

BLYNK_WRITE(AlarmCurrentButtonPin)
{
  int pinValue = param.asInt();
  // Serial.print("CurrentButton: %d\n", pinValue);
  if (pinValue != BunttonTapState) return;

  alarmYellowLevel = yellowLevel;
  alramWhiteLevel = whiteLevel;
  Blynk.virtualWrite(AlarmYellowSliderPin, alarmYellowLevel);
  Blynk.virtualWrite(AlarmWhiteSliderPin, alramWhiteLevel);
}


void cancelAlarm() {
  if (alarmState == alarming) {
    alarmState = cancelled;
  }
}

static void alarm() {
  // Calculate start/end time;
  startTime = time(NULL);
  endTime = startTime + riseTime * 60;
  printf("Alarm interval %ld - %ld, riseTime %d\n", startTime, endTime, riseTime);

  powerState = On;
  nightLedOn(false);
  setButtonsColor();
}

static void adjustBrightness(time_t now) {

  int timeOffset = now - startTime;
  yellowLevel = timeOffset * alarmYellowLevel / (riseTime * 60);
  whiteLevel = timeOffset * alramWhiteLevel / (riseTime * 60);
  ledsRestore();
  Blynk.virtualWrite(YellowSliderPin, yellowLevel);
  Blynk.virtualWrite(WhiteSliderPin, whiteLevel);
}

static void checkAlarm() {
  time_t now;
  struct tm timeinfo;
  enum AlarmState beginState = alarmState;
  
  now = time(NULL);
  localtime_r(&now, &timeinfo);

  switch(alarmState) {
    case waiting:
      if (!alarmEnabled) return;
      if (timeinfo.tm_hour == alarmHour &&
          timeinfo.tm_min == alarmMinute) {
          // Bingo!
          alarmState = alarming;
          alarm();
      }
      break;
    case alarming:
      if (now > endTime) {
        alarmState = waiting;
      } else {
        adjustBrightness(now);
      }
      break;
    case cancelled:
      if (timeinfo.tm_hour == alarmHour &&
      timeinfo.tm_min == alarmMinute) break;
      alarmState = waiting;
      break;
  }
  if (beginState != alarmState) {
    printf("Time %ld, %d:%d, state %d\n", now, timeinfo.tm_hour, timeinfo.tm_min, alarmState);
  }
}

void initialize_sntp(void)
{
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_init();
  setenv("TZ", "MSK-3", 1);
  tzset();

  timer.setInterval(1000L, checkAlarm);
}
