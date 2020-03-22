// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
#include <Servo.h>
#define buz 10
#define buttonPin 8
#define buttonPin2 9
#define buttonPin3 7
#define buttonPin4 6
#define buttonPin5 13

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

Servo myservo;  // create servo object to control a servo
RTC_DS3231 rtc;

char buffer [16];
String daysOfTheWeek[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int alarmPerDay[7][3] = {{19, 1, 1}, {8, 0, 1}, {8, 0, 1}, {8, 0, 1}, {8, 0, 1}, {8, 0, 1}, {15, 47, 1}};
//int inputDay;
int Hor;
int Min;
int Sec;
int Hour = 13;
int Minute = 45;
int potValue;
int buttonState = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;
int ledPin = 13;
int logicVar = 0;
int dayVar = 0;
int val;
unsigned long previousMillis = 0;
const long interval = 40000;

void setup () {

  myservo.write(0);
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Wire.begin();
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  myservo.attach(buz);

  pinMode(buz, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin5, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(ledPin, OUTPUT);

  delay(100); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2020, 2, 1, 14, 44, 0));
  }


  //val = 0;
  //myservo.write(val);
}

void loop () {


  DateTime now = rtc.now();
  Hor = now.hour();
  Min = now.minute();
  Sec = now.second();
  //Serial.println(Hor);
  //Serial.println(Min);
  //Serial.println(Sec);
  //kaffi();
  unsigned long currentMillis = millis();
  ChangeIF();

  sjema();

}

/////////////////////////////////////////////////////////////////
void ChangeIF() {
  DateTime now = rtc.now();
  int nextDay = now.dayOfTheWeek() + 1;
  if (nextDay == 7) {
    nextDay = 0;
  }
  delay(1);
  buttonState2 = digitalRead(buttonPin2);
  boolean buttonPressed2 = false;
  while (buttonState2 == HIGH) {
    buttonPressed2 = true;
    buttonState2 = digitalRead(buttonPin2);
  }
  //logicVar = 0;
  while (buttonPressed2 == true) {
    logicVar++;
    buttonPressed2 = false;
  }
  if (logicVar == 0) {
    lcd.clear();
    WriteScreenRealTime();
    alarmEveryDay();
    alarmOnOff(nextDay);
  }

  if (logicVar == 1) {
    lcd.clear();
    DayToDaySetTime();
    alarmEveryDay();
  }

  if (logicVar == 2) {

    lcd.clear();
    logicVar = 0;
  }
  delay(5);
}


void DayToDaySetTime() {
  buttonState3 = digitalRead(buttonPin3);
  boolean buttonPressed3 = false;
  while (buttonState3 == HIGH) {
    buttonPressed3 = true;
    buttonState3 = digitalRead(buttonPin3);
  }
  while (buttonPressed3 == true) {
    dayVar++;
    buttonPressed3 = false;
    //Serial.print(dayVar);
  }
  if (dayVar >= 7) {
    dayVar = 0;
  }
  kortNedKodeScreen(dayVar);
  alarmOnOff(dayVar);


}

void kortNedKodeScreen(int VarDay) {



  int dummyHor = alarmPerDay[VarDay][0];
  int dummyHor2 = HourChangeGenreal(dummyHor);
  alarmPerDay[VarDay][0] = dummyHor2;

  int dummyMin = alarmPerDay[VarDay][1];
  int dummyMin2 = MinuteChangeGeneral(dummyMin);
  alarmPerDay[VarDay][1] = dummyMin2;

  //alarmOnOff();

  //int AlarmTodayOnOff = now.dayOfTheWeek();

  String OnOff;

  if (alarmPerDay[VarDay][2] == 1) {
    OnOff = "ON";
  }
  if (alarmPerDay[VarDay][2] == 0) {
    OnOff = "OFF";
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[VarDay]);
  lcd.setCursor(0, 1);
  lcd.print("Set alarm:");
  lcd.setCursor(14, 1);
  lcd.print(alarmPerDay[VarDay][1]);
  lcd.setCursor(13, 1);
  lcd.print(":");
  lcd.setCursor(11, 1);
  lcd.print(alarmPerDay[VarDay][0]);
  lcd.setCursor(13, 0);
  lcd.print(OnOff);

  delay(5);

}

void alarmOnOff(int AlarmONOFF) {

  int AlarmONOFFDummy = alarmPerDay[AlarmONOFF][2];

  buttonState4 = digitalRead(buttonPin4);
  boolean buttonPressed4 = false;
  while (buttonState4 == HIGH) {
    buttonPressed4 = true;
    buttonState4 = digitalRead(buttonPin4);
  }
  if (buttonPressed4 == true) {
    AlarmONOFFDummy++;

  }
  if (alarmPerDay[AlarmONOFF][2] == 2) {
    AlarmONOFFDummy = 0;
  }
  alarmPerDay[AlarmONOFF][2] = AlarmONOFFDummy;

}

void WriteScreenRealTime() {
  DateTime now = rtc.now();
  //alarmOnOff();
  int AlarmTomorrow = now.dayOfTheWeek() + 1;
  if (AlarmTomorrow == 7) {
    AlarmTomorrow = 0;
  }

  String OnOff;
  //alarmOnOff();
  if (alarmPerDay[AlarmTomorrow][2] == 1) {
    OnOff = "ON";
  }
  if (alarmPerDay[AlarmTomorrow][2] == 0) {
    OnOff = "OFF";
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.setCursor(8, 0);
  lcd.print(Hor);
  lcd.setCursor(10, 0);
  lcd.print(":");
  lcd.setCursor(11, 0);
  lcd.print(Min);
  lcd.setCursor(13, 0);
  lcd.print(":");
  lcd.setCursor(14, 0);
  lcd.print(Sec);
  lcd.setCursor(0, 1);
  lcd.print(OnOff);
  lcd.setCursor(11, 1);
  lcd.print(alarmPerDay[AlarmTomorrow][0]);
  lcd.setCursor(13, 1);
  lcd.print(":");
  lcd.setCursor(14, 1);
  lcd.print(alarmPerDay[AlarmTomorrow][1]);
  delay(2);

}



int HourChangeGenreal(int inputHorDay) {
  buttonState = digitalRead(buttonPin);
  boolean buttonPressed = false;
  while (buttonState == HIGH) {
    buttonPressed = true;
    buttonState = digitalRead(buttonPin);
  }
  if (buttonPressed == true) {
    inputHorDay++;

  }
  if (inputHorDay == 24) {
    inputHorDay = 0;
  }

  return inputHorDay;
}


int MinuteChangeGeneral(int inputMinDay) {

  buttonState5 = digitalRead(buttonPin5);
  boolean buttonPressed5 = false;
  while (buttonState5 == HIGH) {
    buttonPressed5 = true;
    buttonState5 = digitalRead(buttonPin5);
  }
  if (buttonPressed5 == true) {
    inputMinDay = inputMinDay + 5;

  }
  if (inputMinDay >= 60) {
    inputMinDay = 0;
  }


  return inputMinDay;
  Serial.print("3");
}



void alarmEveryDay() {
  DateTime now = rtc.now();
  int VarAlarm = now.dayOfTheWeek();

  if ( alarmPerDay[VarAlarm][2] == 1) {
    if ( Hor == alarmPerDay[VarAlarm][0] &&  (Min == alarmPerDay[VarAlarm][1]) &&  (Sec == 0)) {
    val = 40;
  }

   if ( Hor == alarmPerDay[VarAlarm][0] &&  (Min == alarmPerDay[VarAlarm][1]) &&  (Sec == 2)) {
    val = 0;
  }
  myservo.write(val);
}
}




//void kaffi() {
//  val = 0;
//  myservo.write(val);
//  if (currentMillis - previousMillis >= interval) {
//    previousMillis = currentMillis;
//    val = 40;
//    myservo.write(val);
//  }
//  if (currentMillis - previousMillis >= 2000) {
//    previousMillis = currentMillis;
//    val = 0;
//    myservo.write(val);
//  }
//
//}


void sjema() {
  //int twat = 6
  for (int twat = 0; twat < 7; twat++) {
    Serial.print(daysOfTheWeek[twat]);
    Serial.print("  ");
    Serial.print(alarmPerDay[twat][0]);
    Serial.print(":");
    Serial.print(alarmPerDay[twat][1]);
    Serial.print("  ");
    Serial.println(alarmPerDay[twat][2]);
    Serial.println("---------------------------------");
  }

}
