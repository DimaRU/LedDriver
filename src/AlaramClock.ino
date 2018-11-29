////
///  AlaramClock.ino
//

int alarmYellowLevel = SLIDER_MAXVALUE/4;
int alramWhiteLevel = SLIDER_MAXVALUE/4;
int riseTime = 15;
bool isAlarmOn = false;

void setAlarmInterval();

BLYNK_WRITE(AlarmYellowSliderPin)
{
  int pinValue = param.asInt();
  alarmYellowLevel = pinValue;
  Serial.printf("Yellow alarm: %d\n", alarmYellowLevel);
}

BLYNK_WRITE(AlarmWhiteSliderPin)
{
  int pinValue = param.asInt();
  alramWhiteLevel = pinValue;
  Serial.printf("White alarm: %d\n", alramWhiteLevel);
}

BLYNK_WRITE(RiseTimePin)
{
  int pinValue = param.asInt();
  riseTime = pinValue;
  Serial.printf("RiseTime: %d min\n", riseTime);

  setAlarmInterval();
}

BLYNK_WRITE(AlaramTimePin)
{
  TimeInputParam t(param);

  Serial.println("Alaram time");
  if (t.hasStartTime()) {
    Serial.println(String("Start: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
  }
  setAlarmInterval();
}

BLYNK_WRITE(AlarmSwitchPin)
{
  int pinValue = param.asInt();
  Serial.printf("Alarm Switch: %d\n", pinValue);

  isAlarmOn = pinValue == 1 ? true : false; 
}


void setAlarmInterval() {
    
}