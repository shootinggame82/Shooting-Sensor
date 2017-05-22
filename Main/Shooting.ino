/*
   Arduino Shooting Game
   v3.12 coded by Andreas Olsson

   This is the main system that holds the hole game. It can use wireless or
   wired game type. We also have terminal printer, and sd card to store highscore.
   RTC Module is to keep time and date.

   ID numbers for targets, don't use same numbers for all targets.

   Recive ID: 16 > Target 1, 26 > Target 2, 36 > Target 3, 46 > Target 4
   Send ID: 17 > Target 1, 27 > Target 2, 37 > Target 3, 47 > Target 4
   Test ID: 91 > Target 1, 92 > Target 2, 93 > Target 3, 94 > Target 4
   OK Test ID: 81 > Target 1, 82 > Target 2, 83 > Target 3, 84 > Target 4
   AckPayLoad: 61,62,51 > Target 1, 63,64,52 > Target 2, 65,66,53 > Target 3, 67,68,54 > Target 4

   We use SdFat library so we can use the sd card reader with SoftSerial.
 */
#include "Adafruit_Thermal.h"
#include "gun2.h"
#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include "SdFat.h"
#include "nRF24L01.h"
#include "RF24.h"

const uint8_t SOFT_MISO_PIN = 40;
const uint8_t SOFT_MOSI_PIN = 41;
const uint8_t SOFT_SCK_PIN  = 42;
const uint8_t SD_CHIP_SELECT_PIN = 43;

SdFatSoftSpi<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> SD;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
RTC_DS1307 RTC;
Adafruit_Thermal printer(&Serial3);
File hquickFil;

int senderId;
int transmitterId = 91;

RF24 radio(53, 49);

const boolean inputMode = HIGH;
const int numPorts=4;
int led[numPorts]={22,23,24,25};
int ins[numPorts]={A0,A1,A2,A3};
int goButton=A4;
int transmittLed=32;
int transmittErrorLed=31;
int rotarySwitch[3]={2,3,4};

int s1State = 1;
int s2State = 1;
int s3State = 1;
int s4State = 1;

int testShotGame = 0;

int theDelay=1;

unsigned long time1;
unsigned long time2;
float interval1;

int startTimerTimes = 15;
int startAlarmTimer = 0;
int timerRunTimes = 0;
long LastSpeakerToggleTime = 0;
bool IsSpeakerOn = false;
const int SpeakerTogglePeriod = 5000;
int changeInfo = 0;
int counttime = 0;

String stringOne, stringTwo;
//For menu
int selectButton = 5;
int selectState = 0;
int goState = 0;
int okButton = 6;
int okState = 0;
int playMode = 0;
int players = 1;
//Highscore Timed
int qplayer;
int qshot;
int qdisplay;
String qdatum;
//20 sek
int qplayer2;
int qshot2;
int qdisplay2;
String qdatum2;
//30 sek
int qplayer3;
int qshot3;
int qdisplay3;
String qdatum3;
//Highscore Rapid Fire
int rplayer;
String rtime;
String rdatum;
int rdisplay;
int rPlayer_1;
String rtime_1;
String rdatum_1;
int rdisplay_1;
int rPlayer_2;
String rtime_2;
String rdatum_2;
int rdisplay_2;
int rPlayer_3;
String rtime_3;
String rdatum_3;
int rdisplay_3;
//Highscore Quick Draw
int qqplayer;
String qtime1;
float qftime1;
String qtime2;
float qftime2;
String qtime3;
float qftime3;
String qtime4;
float qftime4;
String qtime5;
float qftime5;
String qtime6;
float qftime6;
String qtime7;
float qftime7;
String qtime8;
float qftime8;
String qqdatum;
int qqdisplay;
int rshooting = 0;

int printhigh = 0;
int steps = 1;
String timeNow;
int mainscreen = 1;
int menuStatus = 0;
int mainmenu = 0;
int menumode = 0;

//Result Timed
int tPlayer1;
int tPlayer2;
int tPlayer3;
int tPlayer4;
int tPrintScore = 0;
String tDatum1;
String tDatum2;
String tDatum3;
String tDatum4;
//Resultat Rapid
int rPlayer1;
float rsPlayer1;
char rsqPlayer1[15];
int rPlayer2;
float rsPlayer2;
char rsqPlayer2[15];
int rPlayer3;
float rsPlayer3;
char rsqPlayer3[15];
int rPlayer4;
float rsPlayer4;
char rsqPlayer4[15];
int rPrintScore = 0;
String rDatum1;
String rDatum2;
String rDatum3;
String rDatum4;
//Resultat Quickdraw
int qP11;
float qfP11;
char qsP11[15];
int qP12;
float qfP12;
char qsP12[15];
int qP13;
float qfP13;
char qsP13[15];
int qP14;
float qfP14;
char qsP14[15];
int qP15;
float qfP15;
char qsP15[15];
int qP16;
float qfP16;
char qsP16[15];
int qP17;
float qfP17;
char qsP17[15];
int qP18;
float qfP18;
char qsP18[15];

int qP21;
float qfP21;
char qsP21[15];
int qP22;
float qfP22;
char qsP22[15];
int qP23;
float qfP23;
char qsP23[15];
int qP24;
float qfP24;
char qsP24[15];
int qP25;
float qfP25;
char qsP25[15];
int qP26;
float qfP26;
char qsP26[15];
int qP27;
float qfP27;
char qsP27[15];
int qP28;
float qfP28;
char qsP28[15];

int qP31;
float qfP31;
char qsP31[15];
int qP32;
float qfP32;
char qsP32[15];
int qP33;
float qfP33;
char qsP33[15];
int qP34;
float qfP34;
char qsP34[15];
int qP35;
float qfP35;
char qsP35[15];
int qP36;
float qfP36;
char qsP36[15];
int qP37;
float qfP37;
char qsP37[15];
int qP38;
float qfP38;
char qsP38[15];

int qP41;
float qfP41;
char qsP41[15];
int qP42;
float qfP42;
char qsP42[15];
int qP43;
float qfP43;
char qsP43[15];
int qP44;
float qfP44;
char qsP44[15];
int qP45;
float qfP45;
char qsP45[15];
int qP46;
float qfP46;
char qsP46[15];
int qP47;
float qfP47;
char qsP47[15];
int qP48;
float qfP48;
char qsP48[15];

int qPrintScore = 0;
String qDatum1;
String qDatum2;
String qDatum3;
String qDatum4;

int NewHigh = 0;

int resetHigh = 0;

int selectTimed = 0;
float timedRun = 10000;
int timeSel = 1;

int resultsTimed = 0;
int resultsRapid = 0;
int resultsQuick = 0;

int GameModel = 0; //0 is wireless and 1 is for wired.
int lyssna = false;
int lyssna1 = false;
int lyssna2 = false;
int lyssna3 = false;
int checka = false;
int checka0 = false;
int checka1 = false;
int checka2 = false;
int skicka1 = false;
int skicka2 = false;

int ackMessg;
byte ackMessgLen = 2;
int ackData = 61;
int ackData2 = 62;
int ackData3 = 63;
int ackData4 = 64;
int ackData5 = 65;
int ackData6 = 66;
int ackData7 = 67;
int ackData8 = 68;
int nockData = 51;
int nockData2 = 52;
int nockData3 = 53;
int nockData4 = 54;
//Incase it stucks inside a loop, count max times and leave the loop
int checkloop = 0;
int checkloop2 = 0;
int checkloop3 = 0;
int checkloop4 = 0;

const uint64_t pipes[9] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL, 0xF0F0F0F0C3LL, 0xF0F0F0F0C1LL, 0xF0F0F0F0E3LL, 0xABCDABCD71LL, 0xF0F0F0F096LL, 0x3A3A3A3AD2LL, 0x3A3A3A3AC3LL};

void setup() {
        // Firstime we startup we need to setup some settings.
        Serial3.begin(19200);
        Serial.begin(9600);
        printer.begin();
        lcd.begin(20, 4);

        if (!SD.begin(SD_CHIP_SELECT_PIN)) {
                Serial.println("initialization failed!");
                return;
        }
        Serial.println("initialization done.");

        //We setup the wireless options.
        transmitterId = 16;
        radio.begin();
        radio.setPALevel(RF24_PA_LOW);
        radio.setDataRate(RF24_1MBPS);
        radio.setRetries(15,15);
        radio.setPayloadSize(8);
        radio.setChannel(120);
        radio.enableAckPayload();

        radio.openWritingPipe(pipes[0]);
        radio.openReadingPipe(1,pipes[5]);
        radio.startListening();

        //Settings for the RTC clock
        Wire.begin();
        RTC.begin();
        //Remove the comment below to set the rtc with the computer time.
        //RTC.adjust(DateTime(__DATE__, __TIME__));
        pinMode(selectButton,INPUT);
        pinMode(okButton,INPUT);
        pinMode(goButton,INPUT);
        digitalWrite(goButton,HIGH);
        pinMode(transmittLed, OUTPUT);
        digitalWrite(transmittLed,HIGH);
        pinMode(transmittErrorLed, OUTPUT);
        digitalWrite(transmittErrorLed,HIGH);

        for (int i=0; i<numPorts; i++)
        {
                pinMode(led[i], OUTPUT);
                pinMode(ins[i],INPUT);
        }

        for (int i=0; i<3; i++)
        {
                pinMode(rotarySwitch[i],INPUT);
                digitalWrite(rotarySwitch[i],HIGH);
        }
        createSweCaracters(); //This is to make swedish ÅÄÖ on lcd.
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        delay(300);
        lcd.setCursor(0,1);
        lcd.print("        v3.12");
        delay(300);
        lcd.setCursor(0,2);
        lcd.print("Made by:");
        delay(300);
        lcd.setCursor(0,3);
        lcd.print("Andreas Olsson");
        delay(3000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,2);
        lcd.print("Please wait");

        //We now read the settings for rapid fire.
        if (SD.exists("rsset.txt")) {
                hquickFil = SD.open("rsset.txt");
                if (hquickFil) {
                        String rsSetting;
                        Serial.println("rsset.txt:");

                        while (hquickFil.available()) {
                                rsSetting = hquickFil.readStringUntil('\n');
                                rshooting = rsSetting.toInt();

                        }

                        hquickFil.close();

                } else {
                        // if the file didn't open, print an error:
                        Serial.println("error open rsset.txt");
                }

        } else {
                //We now create the file.
                SD.remove("rsset.txt");
                hquickFil = SD.open("rsset.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add some info.
                        hquickFil.println("1");
                        hquickFil.close();
                }
        }

        //Read highscore for Timed Shooting on SD card.
        if (SD.exists("htime.txt")) {
                int ind1;
                int ind2;
                int ind3;
                int shotint;

                String pquick;
                String shot;
                String datum;

                hquickFil = SD.open("htime.txt");
                if (hquickFil) {
                        Serial.println("htime.txt:");
                        String l_line;
                        while (hquickFil.available()) {
                                l_line = hquickFil.readStringUntil('\n');
                                l_line.trim();
                                if (l_line != "") {
                                        int l_start_posn = 0;
                                        ind1 = l_line.indexOf(',');
                                        pquick = l_line.substring(0, ind1);
                                        ind2 = l_line.indexOf(',', ind1+1 );
                                        shot = l_line.substring(ind1+1, ind2);
                                        ind3 = l_line.indexOf(',', ind2+1 );
                                        datum = l_line.substring(ind2+1, ind3);
                                        qplayer = pquick.toInt();
                                        qshot = shot.toInt();
                                        shotint = shot.toInt();
                                        qdatum = datum;
                                        if (qshot == 0) {
                                                qdisplay = 0;
                                        } else {
                                                qdisplay = 1;
                                        }

                                }
                        }
                        hquickFil.close();
                }

        } else {
                //The file didn't exist so we create it.
                hquickFil = SD.open("htime.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add information.
                        hquickFil.println("0,0,2016-05-05 10:00");
                        hquickFil.close();
                }
        }

        if (SD.exists("htime2.txt")) {
                int ind1;
                int ind2;
                int ind3;
                int shotint;

                String pquick;
                String shot;
                String datum;

                hquickFil = SD.open("htime2.txt");
                if (hquickFil) {
                        Serial.println("htime2.txt:");
                        String l_line;
                        while (hquickFil.available()) {
                                l_line = hquickFil.readStringUntil('\n');
                                l_line.trim();
                                if (l_line != "") {
                                        int l_start_posn = 0;
                                        ind1 = l_line.indexOf(',');
                                        pquick = l_line.substring(0, ind1);
                                        ind2 = l_line.indexOf(',', ind1+1 );
                                        shot = l_line.substring(ind1+1, ind2);
                                        ind3 = l_line.indexOf(',', ind2+1 );
                                        datum = l_line.substring(ind2+1, ind3);
                                        qplayer2 = pquick.toInt();
                                        qshot2 = shot.toInt();
                                        shotint = shot.toInt();
                                        qdatum2 = datum;
                                        if (qshot2 == 0) {
                                                qdisplay2 = 0;
                                        } else {
                                                qdisplay2 = 1;
                                        }

                                }
                        }
                        hquickFil.close();
                }

        } else {
                //Create the file if it not exists
                hquickFil = SD.open("htime2.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add the information
                        hquickFil.println("0,0,2016-05-05 10:00");
                        hquickFil.close();
                }
        }

        if (SD.exists("htime3.txt")) {
                int ind1;
                int ind2;
                int ind3;
                int shotint;

                String pquick;
                String shot;
                String datum;

                hquickFil = SD.open("htime3.txt");
                if (hquickFil) {
                        Serial.println("htime3.txt:");
                        String l_line;
                        while (hquickFil.available()) {
                                l_line = hquickFil.readStringUntil('\n');
                                l_line.trim();
                                if (l_line != "") {
                                        int l_start_posn = 0;
                                        ind1 = l_line.indexOf(',');
                                        pquick = l_line.substring(0, ind1);
                                        ind2 = l_line.indexOf(',', ind1+1 );
                                        shot = l_line.substring(ind1+1, ind2);
                                        ind3 = l_line.indexOf(',', ind2+1 );
                                        datum = l_line.substring(ind2+1, ind3);
                                        qplayer3 = pquick.toInt();
                                        qshot3 = shot.toInt();
                                        shotint = shot.toInt();
                                        qdatum3 = datum;
                                        if (qshot3 == 0) {
                                                qdisplay3 = 0;
                                        } else {
                                                qdisplay3 = 1;
                                        }

                                }
                        }
                        hquickFil.close();
                }

        } else {
                //The file didn't exist so we create it.
                hquickFil = SD.open("htime3.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add the information
                        hquickFil.println("0,0,2016-05-05 10:00");
                        hquickFil.close();
                }
        }
        //We now read the highscore for Quickdraw
        if (SD.exists("hqu.txt")) {
                int ind1;
                int ind2;
                int ind3;
                int ind4;
                int ind5;
                int ind6;
                int ind7;
                int ind8;
                int ind9;
                int ind10;
                int shotint;

                String pquick; doPost();
                String tid1;
                String tid2;
                String tid3;
                String tid4;
                String tid5;
                String tid6;
                String tid7;
                String tid8;
                String shot;
                String datum;

                hquickFil = SD.open("hqu.txt");
                if (hquickFil) {
                        Serial.println("hqu.txt:");
                        String l_line;
                        while (hquickFil.available()) {
                                l_line = hquickFil.readStringUntil('\n');
                                l_line.trim();
                                if (l_line != "") {
                                        int l_start_posn = 0;
                                        ind1 = l_line.indexOf(',');
                                        pquick = l_line.substring(0, ind1);
                                        ind2 = l_line.indexOf(',', ind1+1 );
                                        tid1 = l_line.substring(ind1+1, ind2);
                                        ind3 = l_line.indexOf(',', ind2+1 );
                                        tid2 = l_line.substring(ind2+1, ind3);
                                        ind4 = l_line.indexOf(',', ind3+1 );
                                        tid3 = l_line.substring(ind3+1, ind4);
                                        ind5 = l_line.indexOf(',', ind4+1 );
                                        tid4 = l_line.substring(ind4+1, ind5);
                                        ind6 = l_line.indexOf(',', ind5+1 );
                                        tid5 = l_line.substring(ind5+1, ind6);
                                        ind7 = l_line.indexOf(',', ind6+1 );
                                        tid6 = l_line.substring(ind6+1, ind7);
                                        ind8 = l_line.indexOf(',', ind7+1 );
                                        tid7 = l_line.substring(ind7+1, ind8);
                                        ind9 = l_line.indexOf(',', ind8+1 );
                                        tid8 = l_line.substring(ind8+1, ind9);
                                        ind10 = l_line.indexOf(',', ind9+1 );
                                        datum = l_line.substring(ind9+1, ind10);
                                        qqplayer = pquick.toInt();
                                        qtime1 = tid1;
                                        qftime1 = tid1.toFloat();
                                        qtime2 = tid2;
                                        qftime2 = tid2.toFloat();
                                        qtime3 = tid3;
                                        qftime3 = tid3.toFloat();
                                        qtime4 = tid4;
                                        qftime4 = tid4.toFloat();
                                        qtime5 = tid5;
                                        qftime5 = tid5.toFloat();
                                        qtime6 = tid6;
                                        qftime6 = tid6.toFloat();
                                        qtime7 = tid7;
                                        qftime7 = tid7.toFloat();
                                        qtime8 = tid8;
                                        qftime8 = tid8.toFloat();
                                        qqdatum = datum;
                                        if (qqplayer == 0) {
                                                qqdisplay = 0;
                                        } else {
                                                qqdisplay = 1;
                                        }

                                }
                        }
                        hquickFil.close();
                }


        } else {
                //We don't have the file, so we create it.
                hquickFil = SD.open("hqu.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add some information.
                        hquickFil.println("0,0,0,0,0,0,0,0,0,2016-05-05 10:00");
                        hquickFil.close();
                }
        }
        //We add highscores for Rapid Fire (6 shot)
        if (SD.exists("hrap.txt")) {
                int ind1;
                int ind2;
                int ind3;
                int shotint;

                String pquick;
                String shot;
                String datum;
                hquickFil = SD.open("hrap.txt");
                if (hquickFil) {
                        Serial.println("hrap.txt:");
                        String l_line;
                        while (hquickFil.available()) {
                                l_line = hquickFil.readStringUntil('\n');
                                l_line.trim();
                                if (l_line != "") {
                                        int l_start_posn = 0;
                                        ind1 = l_line.indexOf(',');
                                        pquick = l_line.substring(0, ind1);
                                        ind2 = l_line.indexOf(',', ind1+1 );
                                        shot = l_line.substring(ind1+1, ind2);
                                        ind3 = l_line.indexOf(',', ind2+1 );
                                        datum = l_line.substring(ind2+1, ind3);
                                        rplayer = pquick.toInt();
                                        rtime = shot;
                                        rdatum = datum;
                                        if (rplayer == 0) {
                                                rdisplay = 0;
                                        } else {
                                                rdisplay = 1;
                                        }

                                }
                        }
                        hquickFil.close();
                }

        } else {
                //The file don't exist so we create it.
                hquickFil = SD.open("hrap.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add some information.
                        hquickFil.println("0,0,2016-05-05 10:00");
                        hquickFil.close();
                }
        }
        //We add highscores for Rapid Fire (10 shot)
        if (SD.exists("hrap1.txt")) {
                int ind1;
                int ind2;
                int ind3;
                int shotint;

                String pquick;
                String shot;
                String datum;
                hquickFil = SD.open("hrap1.txt");
                if (hquickFil) {
                        Serial.println("hrap1.txt:");
                        String l_line;
                        while (hquickFil.available()) {
                                l_line = hquickFil.readStringUntil('\n');
                                l_line.trim();
                                if (l_line != "") {
                                        int l_start_posn = 0;
                                        ind1 = l_line.indexOf(',');
                                        pquick = l_line.substring(0, ind1);
                                        ind2 = l_line.indexOf(',', ind1+1 );
                                        shot = l_line.substring(ind1+1, ind2);
                                        ind3 = l_line.indexOf(',', ind2+1 );
                                        datum = l_line.substring(ind2+1, ind3);
                                        rPlayer_1 = pquick.toInt();
                                        rtime_1 = shot;
                                        rdatum_1 = datum;
                                        if (rPlayer_1 == 0) {
                                                rdisplay_1 = 0;
                                        } else {
                                                rdisplay_1 = 1;
                                        }

                                }
                        }
                        hquickFil.close();
                }

        } else {
                //The file don't exist so we create it.
                hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add some information.
                        hquickFil.println("0,0,2016-05-05 10:00");
                        hquickFil.close();
                }
        }
        //We add highscores for Rapid Fire (20 shot)
        if (SD.exists("hrap2.txt")) {
                int ind1;
                int ind2;
                int ind3;
                int shotint;

                String pquick;
                String shot;
                String datum;
                hquickFil = SD.open("hrap2.txt");
                if (hquickFil) {
                        Serial.println("hrap2.txt:");
                        String l_line;
                        while (hquickFil.available()) {
                                l_line = hquickFil.readStringUntil('\n');
                                l_line.trim();
                                if (l_line != "") {
                                        int l_start_posn = 0;
                                        ind1 = l_line.indexOf(',');
                                        pquick = l_line.substring(0, ind1);
                                        ind2 = l_line.indexOf(',', ind1+1 );
                                        shot = l_line.substring(ind1+1, ind2);
                                        ind3 = l_line.indexOf(',', ind2+1 );
                                        datum = l_line.substring(ind2+1, ind3);
                                        rPlayer_2 = pquick.toInt();
                                        rtime_2 = shot;
                                        rdatum_2 = datum;
                                        if (rPlayer_2 == 0) {
                                                rdisplay_2 = 0;
                                        } else {
                                                rdisplay_2 = 1;
                                        }

                                }
                        }
                        hquickFil.close();
                }

        } else {
                //The file don't exist so we create it.
                hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add some information.
                        hquickFil.println("0,0,2016-05-05 10:00");
                        hquickFil.close();
                }
        }
        //We add highscores for Rapid Fire (32 shot)
        if (SD.exists("hrap3.txt")) {
                int ind1;
                int ind2;
                int ind3;
                int shotint;

                String pquick;
                String shot;
                String datum;
                hquickFil = SD.open("hrap3.txt");
                if (hquickFil) {
                        Serial.println("hrap3.txt:");
                        String l_line;
                        while (hquickFil.available()) {
                                l_line = hquickFil.readStringUntil('\n');
                                l_line.trim();
                                if (l_line != "") {
                                        int l_start_posn = 0;
                                        ind1 = l_line.indexOf(',');
                                        pquick = l_line.substring(0, ind1);
                                        ind2 = l_line.indexOf(',', ind1+1 );
                                        shot = l_line.substring(ind1+1, ind2);
                                        ind3 = l_line.indexOf(',', ind2+1 );
                                        datum = l_line.substring(ind2+1, ind3);
                                        rPlayer_3 = pquick.toInt();
                                        rtime_3 = shot;
                                        rdatum_3 = datum;
                                        if (rPlayer_3 == 0) {
                                                rdisplay_3 = 0;
                                        } else {
                                                rdisplay_3 = 1;
                                        }

                                }
                        }
                        hquickFil.close();
                }

        } else {
                //The file don't exist so we create it.
                hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add some information.
                        hquickFil.println("0,0,2016-05-05 10:00");
                        hquickFil.close();
                }
        }
        //We add information for players.
        if (SD.exists("s1.txt")) {


        } else {
                //We now create the file
                hquickFil = SD.open("s1.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add info.
                        hquickFil.println("0");
                        hquickFil.close();
                }
        }
        if (SD.exists("s2.txt")) {


        } else {
                hquickFil = SD.open("s2.txt", FILE_WRITE);
                if (hquickFil) {
                        hquickFil.println("0");
                        hquickFil.close();
                }
        }
        if (SD.exists("s3.txt")) {


        } else {
                hquickFil = SD.open("s3.txt", FILE_WRITE);
                if (hquickFil) {
                        hquickFil.println("0");
                        hquickFil.close();
                }
        }
        if (SD.exists("s4.txt")) {


        } else {
                hquickFil = SD.open("s4.txt", FILE_WRITE);
                if (hquickFil) {
                        hquickFil.println("0");
                        hquickFil.close();
                }
        }
        //We now read the file for how long the timed mode will be.
        if (SD.exists("tset.txt")) {
                hquickFil = SD.open("tset.txt");
                if (hquickFil) {
                        String selTime;
                        Serial.println("tset.txt:");

                        while (hquickFil.available()) {
                                selTime = hquickFil.readStringUntil('\n');
                                timeSel = selTime.toInt();

                        }

                        hquickFil.close();
                        if (timeSel == 1) {
                                timedRun = 10000;
                        } else if (timeSel == 2) {
                                timedRun = 20000;
                        } else if (timeSel == 3) {
                                timedRun = 30000;
                        }
                } else {
                        // if the file didn't open, print an error:
                        Serial.println("error open tset.txt");
                }

        } else {
                //We now create the file.
                SD.remove("tset.txt");
                hquickFil = SD.open("tset.txt", FILE_WRITE);
                if (hquickFil) {
                        //And add some info.
                        hquickFil.println("1");
                        hquickFil.close();
                }
        }

        delay(1000);
        setGameType();//This goes to option to choose to use wireless or wired.
        if (GameModel == 1) {
                doPost(); //For wired game flash the leds.
        } else {
                delay(500);
                testWireless(); //For wireless send test to the sensors.
        }
        lcd.clear();
}

void testWireless() {
//This is where we test the wireless sensor with a test signal.

        radio.stopListening();
        checka = true;

        while (checka == true)

        {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Sensor 1");
                lcd.setCursor(0,2);
                lcd.print("Please Wait");
                transmitterId = 91;
                radio.openWritingPipe(pipes[0]);
                radio.openReadingPipe(1,pipes[5]);
                Serial.println("Start Sending");
                delay(60);
                if(radio.write(&transmitterId, sizeof(transmitterId))) {
                        digitalWrite(transmittLed, HIGH);
                        digitalWrite(transmittErrorLed, LOW);

                        if ( radio.isAckPayloadAvailable() ) {
                                radio.read(&ackMessg,sizeof(ackMessg));
                                if (ackMessg == ackData) {
                                        Serial.println("Sending OK");
                                        Serial.print("Acknowledge received: ");
                                        Serial.println(ackMessg);
                                        lyssna = true;
                                        Serial.println(transmitterId);
                                        radio.startListening();
                                }
                        }
                }
                else {
                        digitalWrite(transmittLed, LOW);
                        digitalWrite(transmittErrorLed, HIGH);
                }
                while (lyssna == true) {
                        delay(10);
                        radio.writeAckPayload(1, &ackData2, sizeof(ackData2));
                        if (radio.available(pipes[5])) {
                                digitalWrite(transmittLed, HIGH);
                                digitalWrite(transmittErrorLed, LOW);
                                while (radio.available(pipes[5])) {
                                        radio.read(&senderId, sizeof(senderId));
                                        Serial.print("Recived: ");
                                        Serial.println(senderId);
                                }

                        } else {
                                digitalWrite(transmittLed, LOW);
                                digitalWrite(transmittErrorLed, HIGH);
                        }

                        if (senderId == 81) //Test ID
                        {
                                radio.stopListening();
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("Sensor 1 OK");
                                lcd.setCursor(0,2);
                                lcd.print("Please wait");
                                delay(1000);
                                lyssna = false;
                                checka0 = true;
                                checka = false;
                        } //Test Stop


                        if (digitalRead(okButton)==HIGH) {
                                lyssna = false;
                                checka = false;
                                checka0 = true;
                                delay(500);
                        }
                }

                if (digitalRead(okButton)==HIGH) {
                        lyssna = false;
                        checka = false;
                        checka0 = true;
                        delay(500);
                }
        }


        while (checka0 == true)
        {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Sensor 2");
                lcd.setCursor(0,2);
                lcd.print("Please wait");
                transmitterId = 92;
                radio.stopListening();
                radio.openWritingPipe(pipes[2]);
                radio.openReadingPipe(1,pipes[6]);
                Serial.println("Start Sending");
                delay(60);
                if(radio.write(&transmitterId, sizeof(transmitterId)))
                {
                        digitalWrite(transmittLed, HIGH);
                        digitalWrite(transmittErrorLed, LOW);
                        if ( radio.isAckPayloadAvailable() ) {
                                radio.read(&ackMessg,sizeof(ackMessg));
                                if (ackMessg == ackData3) {
                                        Serial.println("Sending OK");
                                        Serial.print("Acknowledge received: ");
                                        Serial.println(ackMessg);
                                        lyssna1 = true;
                                        Serial.println(transmitterId);
                                        radio.startListening();
                                }
                        }
                }
                else {

                        digitalWrite(transmittLed, LOW);
                        digitalWrite(transmittErrorLed, HIGH);
                }
                while (lyssna1 == true) {
                        delay(10);
                        radio.writeAckPayload(1, &ackData4, sizeof(ackData4));
                        if (radio.available(pipes[6])) {
                                digitalWrite(transmittLed, HIGH);
                                digitalWrite(transmittErrorLed, LOW);
                                while (radio.available(pipes[6])) {
                                        radio.read(&senderId, sizeof(senderId));
                                        Serial.print("Recived: ");
                                        Serial.println(senderId);
                                }

                        }  else {
                                digitalWrite(transmittLed, LOW);
                                digitalWrite(transmittErrorLed, HIGH);

                        }

                        if (senderId == 82) //Test ID
                        {
                                radio.stopListening();

                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("Sensor 2 OK");
                                lcd.setCursor(0,2);
                                lcd.print("Please wait");
                                delay(1000);
                                lyssna1 = false;
                                checka0 = false;
                                checka1 = true;
                        } //Test Stop

                        if (digitalRead(okButton)==HIGH) {
                                lyssna1 = false;
                                checka0 = false;
                                checka1 = true;
                                delay(500);
                        }
                }
                if (digitalRead(okButton)==HIGH) {
                        lyssna1 = false;
                        checka0 = false;
                        checka1 = true;
                        delay(500);
                }
        }



        while (checka1 == true)
        {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Sensor 3");
                lcd.setCursor(0,2);
                lcd.print("Please wait");
                transmitterId = 93;
                radio.stopListening();
                radio.openWritingPipe(pipes[3]);
                radio.openReadingPipe(1,pipes[7]);
                Serial.println("Start Sending");
                delay(60);
                if(radio.write(&transmitterId, sizeof(transmitterId)))
                {
                        digitalWrite(transmittLed, HIGH);
                        digitalWrite(transmittErrorLed, LOW);
                        if ( radio.isAckPayloadAvailable() ) {
                                radio.read(&ackMessg,sizeof(ackMessg));
                                if (ackMessg == ackData5) {

                                        Serial.println("Sending OK");
                                        Serial.print("Acknowledge received: ");
                                        Serial.println(ackMessg);
                                        lyssna2 = true;
                                        Serial.println(transmitterId);
                                        radio.startListening();
                                }
                        }
                }
                else {
                        digitalWrite(transmittLed, LOW);
                        digitalWrite(transmittErrorLed, HIGH);
                }

                while (lyssna2 == true) {
                        delay(10);
                        radio.writeAckPayload(1, &ackData6, sizeof(ackData6));
                        if (radio.available(pipes[7])) {
                                digitalWrite(transmittLed, HIGH);
                                digitalWrite(transmittErrorLed, LOW);
                                while (radio.available(pipes[7])) {
                                        radio.read(&senderId, sizeof(senderId));
                                        Serial.print("Recived: ");
                                        Serial.println(senderId);
                                }

                        }  else {
                                digitalWrite(transmittLed, LOW);
                                digitalWrite(transmittErrorLed, HIGH);
                        }

                        if (senderId == 83) //Test ID
                        {
                                radio.stopListening();

                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("Sensor 3 OK");
                                lcd.setCursor(0,2);
                                lcd.print("Please wait");
                                delay(1000);
                                lyssna2 = false;
                                checka1 = false;
                                checka2 = true;


                        } //Test Stop

                        if (digitalRead(okButton)==HIGH) {
                                lyssna2 = false;
                                checka1 = false;
                                checka2 = true;
                                delay(500);
                        }
                }
                if (digitalRead(okButton)==HIGH) {
                        lyssna2 = false;
                        checka1 = false;
                        checka2 = true;
                        delay(500);
                }
        }


        while (checka2 == true)
        {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Sensor 4");
                lcd.setCursor(0,2);
                lcd.print("Please wait");
                transmitterId = 94;
                radio.stopListening();
                radio.openWritingPipe(pipes[4]);
                radio.openReadingPipe(1,pipes[8]);
                Serial.println("Start Sending");
                delay(60);
                if(radio.write(&transmitterId, sizeof(transmitterId)))
                {
                        digitalWrite(transmittLed, HIGH);
                        digitalWrite(transmittErrorLed, LOW);
                        if ( radio.isAckPayloadAvailable() ) {
                                radio.read(&ackMessg,sizeof(ackMessg));
                                if (ackMessg == ackData7) {

                                        Serial.println("Sending OK");
                                        Serial.print("Acknowledge received: ");
                                        Serial.println(ackMessg);
                                        lyssna3 = true;
                                        Serial.println(transmitterId);
                                        radio.startListening();
                                }
                        }
                }
                else {
                        digitalWrite(transmittLed, LOW);
                        digitalWrite(transmittErrorLed, HIGH);
                }

                while (lyssna3 == true) {
                        delay(10);
                        radio.writeAckPayload(1, &ackData8, sizeof(ackData8));
                        if (radio.available(pipes[8])) {
                                digitalWrite(transmittLed, HIGH);
                                digitalWrite(transmittErrorLed, LOW);
                                while (radio.available(pipes[8])) {

// We recive the signal to enable this target
                                        radio.read(&senderId, sizeof(senderId));
                                        Serial.print("Recived: ");
                                        Serial.println(senderId);
                                }

                        }  else {
                                digitalWrite(transmittLed, LOW);
                                digitalWrite(transmittErrorLed, HIGH);

                        }

                        if (senderId == 84) //Test ID
                        {

                                radio.stopListening();
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("Sensor 4 OK");
                                lcd.setCursor(0,2);
                                lcd.print("Please wait");
                                delay(1000);
                                lyssna3 = false;
                                checka2 = false;


                        } //Test Stop

                        if (digitalRead(okButton)==HIGH) {
                                lyssna3 = false;
                                checka2 = false;
                                delay(500);
                        }

                }

                if (digitalRead(okButton)==HIGH) {
                        lyssna3 = false;
                        checka2 = false;
                        delay(500);
                }
        }


}

void loop() {
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        DateTime now = RTC.now();
        timeNow = String(now.year()) + "-";
        if (now.month() < 10) {
                timeNow = timeNow + "0" + String(now.month());
        } else {
                timeNow = timeNow + String(now.month());
        }
        if (now.day() < 10) {
                timeNow = timeNow + "- 0" + String(now.day());
        } else {
                timeNow = timeNow + "-" + String(now.day());
        }
        if (now.hour() < 10) {
                timeNow = timeNow + " 0" + String(now.hour());
        } else {
                timeNow = timeNow + " " + String(now.hour());
        }
        if (now.minute() < 10) {
                timeNow = timeNow + ":0" + String(now.minute());
        } else {
                timeNow = timeNow + ":" + String(now.minute());
        }
        //DATE
        lcd.setCursor(0,1);
        lcd.print(now.year(), DEC);
        lcd.print('-');
        if (now.month() < 10) lcd.print("0");
        lcd.print(now.month(), DEC);
        lcd.print('-');
        if (now.day() < 10) lcd.print("0");
        lcd.print(now.day(), DEC);
        lcd.print(' ');

        //TIME
        lcd.setCursor(13,1);
        if (now.hour() < 10) lcd.print("0");
        lcd.print(now.hour(), DEC);
        lcd.print(':');
        lcd.setCursor(16,1);
        if (now.minute() < 10) lcd.print("0");
        lcd.print(now.minute(), DEC);
        if (playMode == 0) {
                lcd.setCursor(0,2);
                lcd.print("Select players:");
                lcd.setCursor(0,3);
                lcd.print("1");
                while (playMode == 0) {
                        selectState = digitalRead(selectButton);

                        //Let them select the players first!
                        if (digitalRead(selectButton)==HIGH) {
                                if (steps == 0) {
                                        lcd.setCursor(0,3);
                                        lcd.print("1");
                                        players = 1;
                                        steps = 1;
                                        delay(500);
                                } else if (steps == 1) {
                                        lcd.setCursor(0,3);
                                        lcd.print("2");
                                        players = 2;
                                        steps = 2;
                                        delay(500);
                                } else if (steps == 2) {
                                        lcd.setCursor(0,3);
                                        lcd.print("3");
                                        players = 3;
                                        steps = 3;
                                        delay(500);
                                } else if (steps == 3) {
                                        lcd.setCursor(0,3);
                                        lcd.print("4");
                                        players = 4;
                                        steps = 0;
                                        delay(500);
                                }
                        }
                        okState = digitalRead(okButton);
                        if (digitalRead(okButton)==HIGH) {
                                playMode = 1;
                                break;
                        }

                }
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Players selected:");
                lcd.setCursor(0,3);
                lcd.print(players);
                delay(3000);
                playMode = 1;
                displayCodeEntryScreen();
                changeInfo = 1;
        }

        okState = digitalRead(okButton);
        if (okState == HIGH && mainmenu == 0) {
                changeInfo = 0;
                menu();
        }

        goState = digitalRead(goButton);
        if (goState == LOW) {
                changeInfo = 0;
                if (GameModel == 1) {
                        if (digitalRead(rotarySwitch[0])==LOW) doQuickDraw();
                        else if (digitalRead(rotarySwitch[1])==LOW) doTimedMode();
                        else if (digitalRead(rotarySwitch[2])==LOW) doRapidFire();
                } else {
                        if (digitalRead(rotarySwitch[0])==LOW) doQuickDrawW();
                        else if (digitalRead(rotarySwitch[1])==LOW) doTimedModeW();
                        else if (digitalRead(rotarySwitch[2])==LOW) doRapidFireW();

                }
        }

        //This is just for display players and selected gamemode on display.
        if ((millis() - LastSpeakerToggleTime) > SpeakerTogglePeriod)
        {
                LastSpeakerToggleTime = millis();
                if (changeInfo == 1)
                {
                        if (IsSpeakerOn)
                        {
                                IsSpeakerOn = false;
                                if (digitalRead(rotarySwitch[0])==LOW) {
                                        lcd.setCursor(0,3);
                                        lcd.print("                   ");
                                        lcd.setCursor(0,3);
                                        lcd.print("Game: Quickdraw");
                                } else if (digitalRead(rotarySwitch[1])==LOW) {
                                        lcd.setCursor(0,3);
                                        lcd.print("                   ");
                                        lcd.setCursor(0,3);
                                        lcd.print("Game: Time mode");
                                } else if (digitalRead(rotarySwitch[2])==LOW) {
                                        lcd.setCursor(0,3);
                                        lcd.print("                   ");
                                        lcd.setCursor(0,3);
                                        lcd.print("Game: Rapid Fire");
                                }

                        }
                        else
                        {
                                IsSpeakerOn = true;
                                lcd.setCursor(0,3);
                                lcd.print("                   ");
                                lcd.setCursor(0,3);
                                lcd.print("Players: ");
                                lcd.print(players);

                        }
                }
        }
}
//Timed Mode Wired
void doTimedMode()
{
        for (int i=1; i<=players; i++)
        {
                if (i == 1) {
                        delay(100);
                } else {
                        lcd.clear();

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Time for");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(i);
                        lcd.setCursor(0,3);
                        lcd.print("In 30 sec");
                        delay(30000);
                }
                randomSeed(millis());

                int currentPort=random(4);
                int newPort=random(4);
                int maxRounds=32;
                int hitCounter=0;
                time1=millis();
                interval1=0;

                Serial.write(0x0C); delay(5);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Time mode");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(i);
                delay(3000);
                delay(5);
                lcd.setCursor(0,3);
                lcd.print("GET READY!!");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Time mode");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(i);
                while (interval1 < timedRun) // Time is set in the game menu. (10, 20 or 30 sec)
                {
                        digitalWrite(led[currentPort],HIGH);
                        switch (currentPort)
                        {
                        case 0:
                                while (digitalRead(ins[0])==LOW) ;
                                break;

                        case 1:
                                while (digitalRead(ins[1])==LOW) ;
                                break;

                        case 2:
                                while (digitalRead(ins[2])==LOW) ;
                                break;

                        case 3:
                                while (digitalRead(ins[3])==LOW) ;
                                break;

                        }
                        hitCounter++;
                        time2=millis();
                        interval1=(time2-time1);
                        digitalWrite(led[currentPort],LOW);
                        delay(5);
                        lcd.setCursor(0,3);
                        lcd.print("Hit: ");
                        lcd.print(hitCounter);
                        newPort=random(4);
                        while (newPort==currentPort) newPort=random(4);
                        currentPort=newPort;
                }
                delay(5);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Time mode");
                lcd.setCursor(0,2);
                lcd.print(hitCounter);
                lcd.print(" Hit");
                Serial.write(0x0D);
                lcd.setCursor(0,3);
                if (timeSel == 1) {
                        lcd.print("In 10 sec");
                } else if (timeSel == 2) {
                        lcd.print("In 20 sec");
                } else if (timeSel == 3) {
                        lcd.print("In 30 sec");
                }
                DateTime now = RTC.now();
                timeNow = String(now.year()) + "-";
                if (now.month() < 10) {
                        timeNow = timeNow + "0" + String(now.month());
                } else {
                        timeNow = timeNow + String(now.month());
                }
                if (now.day() < 10) {
                        timeNow = timeNow + "-0" + String(now.day());
                } else {
                        timeNow = timeNow + "-" + String(now.day());
                }
                if (now.hour() < 10) {
                        timeNow = timeNow + " 0" + String(now.hour());
                } else {
                        timeNow = timeNow + " " + String(now.hour());
                }
                if (now.minute() < 10) {
                        timeNow = timeNow + ":0" + String(now.minute());
                } else {
                        timeNow = timeNow + ":" + String(now.minute());
                }
                //We check and add if there is an highscore to SD.
                if (i == 1) {
                        tPlayer1 = hitCounter;
                        tDatum1 = timeNow;
                        if (timeSel == 1) {
                                if (tPlayer1 > qshot) {
                                        qshot = tPlayer1;
                                        qplayer = 1;
                                        qdatum = tDatum1;
                                        NewHigh = 1;
                                        SD.remove("htime.txt");
                                        hquickFil = SD.open("htime.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 2) {
                                if (tPlayer1 > qshot2) {
                                        qshot2 = tPlayer1;
                                        qplayer2 = 1;
                                        qdatum2 = tDatum1;
                                        NewHigh = 1;
                                        SD.remove("htime2.txt");
                                        hquickFil = SD.open("htime2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer2);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot2);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum2);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 3) {
                                if (tPlayer1 > qshot3) {
                                        qshot3 = tPlayer1;
                                        qplayer3 = 1;
                                        qdatum3 = tDatum1;
                                        NewHigh = 1;
                                        SD.remove("htime3.txt");
                                        hquickFil = SD.open("htime3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer3);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot3);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum3);
                                                hquickFil.close();
                                        }
                                }
                        }
                } else
                if (i == 2) {
                        tPlayer2 = hitCounter;
                        tDatum2 = timeNow;
                        if (timeSel == 1) {
                                if (tPlayer2 > qshot) {
                                        qshot = tPlayer2;
                                        qplayer = 2;
                                        qdatum = tDatum2;
                                        NewHigh = 1;
                                        SD.remove("htime.txt");
                                        hquickFil = SD.open("htime.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 2) {
                                if (tPlayer2 > qshot2) {
                                        qshot2 = tPlayer2;
                                        qplayer2 = 2;
                                        qdatum2 = tDatum2;
                                        NewHigh = 1;
                                        SD.remove("htime2.txt");
                                        hquickFil = SD.open("htime2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer2);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot2);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum2);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 3) {
                                if (tPlayer2 > qshot3) {
                                        qshot3 = tPlayer2;
                                        qplayer3 = 2;
                                        qdatum3 = tDatum2;
                                        NewHigh = 1;
                                        SD.remove("htime3.txt");
                                        hquickFil = SD.open("htime3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer3);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot3);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum3);
                                                hquickFil.close();
                                        }
                                }
                        }
                } else
                if (i == 3) {
                        tPlayer3 = hitCounter;
                        tDatum3 = timeNow;
                        if (timeSel == 1) {
                                if (tPlayer3 > qshot) {
                                        qshot = tPlayer3;
                                        qplayer = 3;
                                        qdatum = tDatum3;
                                        NewHigh = 1;
                                        SD.remove("htime.txt");
                                        hquickFil = SD.open("htime.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 2) {
                                if (tPlayer3 > qshot2) {
                                        qshot2 = tPlayer3;
                                        qplayer2 = 3;
                                        qdatum2 = tDatum3;
                                        NewHigh = 1;
                                        SD.remove("htime2.txt");
                                        hquickFil = SD.open("htime2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer2);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot2);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum2);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 3) {
                                if (tPlayer3 > qshot3) {
                                        qshot3 = tPlayer3;
                                        qplayer3 = 3;
                                        qdatum3 = tDatum3;
                                        NewHigh = 1;
                                        SD.remove("htime3.txt");
                                        hquickFil = SD.open("htime3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer3);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot3);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum3);
                                                hquickFil.close();
                                        }
                                }
                        }

                } else
                if (i == 4) {
                        tPlayer4 = hitCounter;
                        tDatum4 = timeNow;
                        if (timeSel == 1) {
                                if (tPlayer4 > qshot) {
                                        qshot = tPlayer4;
                                        qplayer = 4;
                                        qdatum = tDatum4;
                                        NewHigh = 1;
                                        SD.remove("htime.txt");
                                        hquickFil = SD.open("htime.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 2) {
                                if (tPlayer4 > qshot2) {
                                        qshot2 = tPlayer4;
                                        qplayer2 = 4;
                                        qdatum2 = tDatum4;
                                        NewHigh = 1;
                                        SD.remove("htime2.txt");
                                        hquickFil = SD.open("htime2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer2);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot2);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum2);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 3) {
                                if (tPlayer4 > qshot3) {
                                        qshot3 = tPlayer4;
                                        qplayer3 = 4;
                                        qdatum3 = tDatum4;
                                        NewHigh = 1;
                                        SD.remove("htime3.txt");
                                        hquickFil = SD.open("htime3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer3);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot3);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum3);
                                                hquickFil.close();
                                        }
                                }
                        }
                }
                tPrintScore = 1;
                delay(5000);
        }
        //We show on LCD if new highscore.
        if (NewHigh == 1) {
                if (timeSel == 1) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(qplayer);
                        lcd.setCursor(0,3);
                        lcd.print("Hits: ");
                        lcd.print(qshot);
                        NewHigh = 0;
                        delay(5000);
                } else if (timeSel == 2) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(qplayer2);
                        lcd.setCursor(0,3);
                        lcd.print("Hits: ");
                        lcd.print(qshot2);
                        NewHigh = 0;
                        delay(5000);
                } else if (timeSel == 3) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(qplayer3);
                        lcd.setCursor(0,3);
                        lcd.print("Hits: ");
                        lcd.print(qshot3);
                        NewHigh = 0;
                        delay(5000);
                }
        }

        if (tPrintScore == 1) {
                //Give them option to print out the highscore.
                lcd.clear();
                printhigh = 1;
                while (printhigh = 1) {

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Push on select");
                        lcd.setCursor(0,2);
                        lcd.print("to print results");
                        lcd.setCursor(0,3);
                        lcd.print("or else press OK");
                        if (digitalRead(okButton)==HIGH) {
                                printhigh = 0;
                                qqdisplay = 0;
                                mainscreen = 0;
                                tPrintScore = 0;
                                displayCodeEntryScreen();
                                break;
                        }
                        if (digitalRead(selectButton)==HIGH) {
                                printhigh = 0;
                                tPrintScore = 0;
                                printTimed();
                                break;
                        }
                }
        }
        changeInfo = 1;
        resultsTimed = 1;
}

//Timed mode wireless.
void doTimedModeW()
{
        for (int i=1; i<=players; i++)
        {
                if (i == 1) {
                        delay(100);
                } else {
                        lcd.clear();

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Time for");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(i);
                        lcd.setCursor(0,3);
                        lcd.print("In 30 sec");
                        delay(30000);
                }
                randomSeed(millis());

                int currentPort=random(4);
                int newPort=random(4);
                int maxRounds=32;
                int hitCounter=0;
                time1=millis();
                interval1=0;

                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Time mode");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(i);
                delay(3000);

                lcd.setCursor(0,3);
                lcd.print("GET READY!!");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Time mode");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(i);
                while (interval1 < timedRun) // 10 seconds
                {
                        bool skicka3 = false;
                        bool skicka4 = false;
                        bool skicka5 = false;
                        bool skicka6 = false;
                        bool skicka7 = false;
                        bool skicka8 = false;
                        int backloop = 0;

                        if (currentPort == 0) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[0]);
                                radio.openReadingPipe(1,pipes[5]);
                                transmitterId = 16;
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                skicka1 = true;
                        } else if (currentPort == 1) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[2]);
                                radio.openReadingPipe(1,pipes[6]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 26;
                                skicka3 = true;


                        } else if (currentPort == 2) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[3]);
                                radio.openReadingPipe(1,pipes[7]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 36;
                                skicka5 = true;


                        } else if (currentPort == 3) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[4]);
                                radio.openReadingPipe(1,pipes[8]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 46;
                                skicka7 = true;


                        }

                        while (skicka1 == true)
                        {

                                if(radio.write(&transmitterId, sizeof(transmitterId))) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);

                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka2 = true;
                                                        skicka1 = false;
                                                        Serial.println(transmitterId);

                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }




                        }

                        while (skicka2 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData, sizeof(nockData));
                                if (radio.available(pipes[5])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[5])) {
                                                Serial.println("Start Lissening");
                                                radio.read(&senderId, sizeof(senderId));
                                        }
                                }  else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 17) //Target 1
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka2 = false;
                                        skicka1 = false;

                                }

                        }

                        while (skicka3 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData3) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka4 = true;
                                                        skicka3 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka4 == true)
                        {
                                delay(10);
                                Serial.println("Lyssnar 2");
                                radio.writeAckPayload(1, &nockData2, sizeof(nockData2));
                                if (radio.available(pipes[6])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[6])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Mottagit: ");
                                                Serial.println(senderId);
                                        }
                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 27) //Target 2
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka4 = false;
                                        skicka3 = false;
                                }


                        }
                        while (skicka5 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData5) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka6 = true;
                                                        skicka5 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka6 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData3, sizeof(nockData3));
                                if (radio.available(pipes[7])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[7])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Mottagit: ");
                                                Serial.println(senderId);

                                        }

                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 37) //Target 3
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka5 = false;
                                        skicka6 = false;
                                }

                        }
                        while (skicka7 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData7) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka8 = true;
                                                        skicka7 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka8 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData4, sizeof(nockData4));
                                if (radio.available(pipes[8])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[8])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Mottagit: ");
                                                Serial.println(senderId);
                                        }
                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 47) //Target 4
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka7 = false;
                                        skicka8 = false;
                                        backloop = 0;
                                }


                        }
                        hitCounter++;
                        time2=millis();
                        interval1=(time2-time1);
                        delay(5);
                        lcd.setCursor(0,3);
                        lcd.print("Hits: ");
                        lcd.print(hitCounter);
                        newPort=random(4);
                        while (newPort==currentPort) newPort=random(4);
                        currentPort=newPort;
                }
                delay(5);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Time mode");
                lcd.setCursor(0,2);
                lcd.print(hitCounter);
                lcd.print(" Hits");
                Serial.write(0x0D);
                lcd.setCursor(0,3);
                if (timeSel == 1) {
                        lcd.print("In 10 sec");
                } else if (timeSel == 2) {
                        lcd.print("In 20 sec");
                } else if (timeSel == 3) {
                        lcd.print("In 30 sec");
                }
                DateTime now = RTC.now();
                timeNow = String(now.year()) + "-";
                if (now.month() < 10) {
                        timeNow = timeNow + "0" + String(now.month());
                } else {
                        timeNow = timeNow + String(now.month());
                }
                if (now.day() < 10) {
                        timeNow = timeNow + "-0" + String(now.day());
                } else {
                        timeNow = timeNow + "-" + String(now.day());
                }
                if (now.hour() < 10) {
                        timeNow = timeNow + " 0" + String(now.hour());
                } else {
                        timeNow = timeNow + " " + String(now.hour());
                }
                if (now.minute() < 10) {
                        timeNow = timeNow + ":0" + String(now.minute());
                } else {
                        timeNow = timeNow + ":" + String(now.minute());
                }
                //We add higscore if there is.
                if (i == 1) {
                        tPlayer1 = hitCounter;
                        tDatum1 = timeNow;
                        if (timeSel == 1) {
                                if (tPlayer1 > qshot) {
                                        qshot = tPlayer1;
                                        qplayer = 1;
                                        qdatum = tDatum1;
                                        NewHigh = 1;
                                        SD.remove("htime.txt");
                                        hquickFil = SD.open("htime.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 2) {
                                if (tPlayer1 > qshot2) {
                                        qshot2 = tPlayer1;
                                        qplayer2 = 1;
                                        qdatum2 = tDatum1;
                                        NewHigh = 1;
                                        SD.remove("htime2.txt");
                                        hquickFil = SD.open("htime2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer2);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot2);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum2);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 3) {
                                if (tPlayer1 > qshot3) {
                                        qshot3 = tPlayer1;
                                        qplayer3 = 1;
                                        qdatum3 = tDatum1;
                                        NewHigh = 1;
                                        SD.remove("htime3.txt");
                                        hquickFil = SD.open("htime3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer3);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot3);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum3);
                                                hquickFil.close();
                                        }
                                }
                        }
                } else
                if (i == 2) {
                        tPlayer2 = hitCounter;
                        tDatum2 = timeNow;
                        if (timeSel == 1) {
                                if (tPlayer2 > qshot) {
                                        qshot = tPlayer2;
                                        qplayer = 2;
                                        qdatum = tDatum2;
                                        NewHigh = 1;
                                        SD.remove("htime.txt");
                                        hquickFil = SD.open("htime.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 2) {
                                if (tPlayer2 > qshot2) {
                                        qshot2 = tPlayer2;
                                        qplayer2 = 2;
                                        qdatum2 = tDatum2;
                                        NewHigh = 1;
                                        SD.remove("htime2.txt");
                                        hquickFil = SD.open("htime2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer2);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot2);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum2);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 3) {
                                if (tPlayer2 > qshot3) {
                                        qshot3 = tPlayer2;
                                        qplayer3 = 2;
                                        qdatum3 = tDatum2;
                                        NewHigh = 1;
                                        SD.remove("htime3.txt");
                                        hquickFil = SD.open("htime3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer3);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot3);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum3);
                                                hquickFil.close();
                                        }
                                }
                        }
                } else
                if (i == 3) {
                        tPlayer3 = hitCounter;
                        tDatum3 = timeNow;
                        if (timeSel == 1) {
                                if (tPlayer3 > qshot) {
                                        qshot = tPlayer3;
                                        qplayer = 3;
                                        qdatum = tDatum3;
                                        NewHigh = 1;
                                        SD.remove("htime.txt");
                                        hquickFil = SD.open("htime.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 2) {
                                if (tPlayer3 > qshot2) {
                                        qshot2 = tPlayer3;
                                        qplayer2 = 3;
                                        qdatum2 = tDatum3;
                                        NewHigh = 1;
                                        SD.remove("htime2.txt");
                                        hquickFil = SD.open("htime2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer2);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot2);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum2);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 3) {
                                if (tPlayer3 > qshot3) {
                                        qshot3 = tPlayer3;
                                        qplayer3 = 3;
                                        qdatum3 = tDatum3;
                                        NewHigh = 1;
                                        SD.remove("htime3.txt");
                                        hquickFil = SD.open("htime3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer3);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot3);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum3);
                                                hquickFil.close();
                                        }
                                }
                        }

                } else
                if (i == 4) {
                        tPlayer4 = hitCounter;
                        tDatum4 = timeNow;
                        if (timeSel == 1) {
                                if (tPlayer4 > qshot) {
                                        qshot = tPlayer4;
                                        qplayer = 4;
                                        qdatum = tDatum4;
                                        NewHigh = 1;
                                        SD.remove("htime.txt");
                                        hquickFil = SD.open("htime.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 2) {
                                if (tPlayer4 > qshot2) {
                                        qshot2 = tPlayer4;
                                        qplayer2 = 4;
                                        qdatum2 = tDatum4;
                                        NewHigh = 1;
                                        SD.remove("htime2.txt");
                                        hquickFil = SD.open("htime2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer2);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot2);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum2);
                                                hquickFil.close();
                                        }
                                }
                        } else if (timeSel == 3) {
                                if (tPlayer4 > qshot3) {
                                        qshot3 = tPlayer4;
                                        qplayer3 = 4;
                                        qdatum3 = tDatum4;
                                        NewHigh = 1;
                                        SD.remove("htime3.txt");
                                        hquickFil = SD.open("htime3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(qplayer3);
                                                hquickFil.print(",");
                                                hquickFil.print(qshot3);
                                                hquickFil.print(",");
                                                hquickFil.println(qdatum3);
                                                hquickFil.close();
                                        }
                                }
                        }
                }
                tPrintScore = 1;
                delay(5000);
        }
        //Display hiscore on LCD
        if (NewHigh == 1) {
                if (timeSel == 1) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(qplayer);
                        lcd.setCursor(0,3);
                        lcd.print("Hits: ");
                        lcd.print(qshot);
                        NewHigh = 0;
                        delay(5000);
                } else if (timeSel == 2) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(qplayer2);
                        lcd.setCursor(0,3);
                        lcd.print("Hits: ");
                        lcd.print(qshot2);
                        NewHigh = 0;
                        delay(5000);
                } else if (timeSel == 3) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(qplayer3);
                        lcd.setCursor(0,3);
                        lcd.print("Hits: ");
                        lcd.print(qshot3);
                        NewHigh = 0;
                        delay(5000);
                }
        }

        if (tPrintScore == 1) {
                //Make it possible to print.
                lcd.clear();
                printhigh = 1;
                while (printhigh = 1) {

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Press on select");
                        lcd.setCursor(0,2);
                        lcd.print("to print results");
                        lcd.setCursor(0,3);
                        lcd.print("or else press OK");
                        if (digitalRead(okButton)==HIGH) {
                                printhigh = 0;
                                qqdisplay = 0;
                                mainscreen = 0;
                                tPrintScore = 0;
                                displayCodeEntryScreen();
                                break;
                        }
                        if (digitalRead(selectButton)==HIGH) {
                                printhigh = 0;
                                tPrintScore = 0;
                                printTimed();
                                break;
                        }
                }
        }
        changeInfo = 1;
        resultsTimed = 1;
}

//Rapid Fire wired
void doRapidFire()
{
        for (int ao=1; ao<=players; ao++)
        {
                if (ao == 1) {
                        delay(100);
                } else {
                        lcd.clear();

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Time for");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(ao);
                        lcd.setCursor(0,3);
                        lcd.print("In 30 sec");
                        delay(30000);
                }

                randomSeed(millis());

                int currentPort=random(4);
                int newPort=random(4);
                int maxRounds=6;
                if (rshooting == 0) {
                        maxRounds=6;
                } else if (rshooting == 1) {
                        maxRounds=10;
                } else if (rshooting == 2) {
                        maxRounds=20;
                } else if (rshooting == 3) {
                        maxRounds=32;
                }
                time1=millis();

                delay(5);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Rapid Fire");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(ao);
                delay(2000);

                for (int i=0; i<maxRounds; i++)
                {
                        delay(10);
                        lcd.setCursor(0,3);
                        lcd.print("Round ");
                        lcd.print(i+1);
                        lcd.print(" of ");
                        lcd.print(maxRounds);
                        digitalWrite(led[currentPort],HIGH);
                        switch (currentPort)
                        {
                        case 0:
                                while (digitalRead(ins[0])==LOW) ;
                                break;
                        case 1:
                                while (digitalRead(ins[1])==LOW) ;
                                break;
                        case 2:
                                while (digitalRead(ins[2])==LOW) ;
                                break;
                        case 3:
                                while (digitalRead(ins[3])==LOW) ;
                                break;
                        }
                        digitalWrite(led[currentPort],LOW);
                        newPort=random(4);
                        while (newPort==currentPort) newPort=random(4);

                        currentPort=newPort;
                }

                time2=millis();
                interval1=(time2-time1);
                interval1=interval1/1000;
                delay(5);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Rapid Fire");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(ao);
                lcd.setCursor(0,3);
                lcd.print("Time: ");
                Serial.print(interval1,3);
                lcd.print(interval1,3);
                lcd.print(" sec");
                DateTime now = RTC.now();
                timeNow = String(now.year()) + "-";
                if (now.month() < 10) {
                        timeNow = timeNow + "0" + String(now.month());
                } else {
                        timeNow = timeNow + String(now.month());
                }
                if (now.day() < 10) {
                        timeNow = timeNow + "-0" + String(now.day());
                } else {
                        timeNow = timeNow + "-" + String(now.day());
                }
                if (now.hour() < 10) {
                        timeNow = timeNow + " 0" + String(now.hour());
                } else {
                        timeNow = timeNow + " " + String(now.hour());
                }
                if (now.minute() < 10) {
                        timeNow = timeNow + ":0" + String(now.minute());
                } else {
                        timeNow = timeNow + ":" + String(now.minute());
                }
                //We add the highscore if there is.
                if (ao == 1) {
                        rsPlayer1 = interval1,3;
                        rPlayer1 = interval1,3;
                        dtostrf(rsPlayer1, 4, 3, rsqPlayer1);
                        rDatum1 = timeNow;
                        if (rshooting == 0) {
                                if (rplayer == 0) {
                                        rtime = String(rsqPlayer1);
                                        rplayer = 1;
                                        rdatum = rDatum1;
                                        NewHigh = 1;
                                        SD.remove("hrap.txt");
                                        hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer1 < rtime.toFloat()) {
                                                rtime = String(rsqPlayer1);
                                                rplayer = 1;
                                                rdatum = rDatum1;
                                                NewHigh = 1;
                                                SD.remove("hrap.txt");
                                                hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 1) {
                                if (rPlayer_1 == 0) {
                                        rtime_1 = String(rsqPlayer1);
                                        rPlayer_1 = 1;
                                        rdatum_1 = rDatum1;
                                        NewHigh = 1;
                                        SD.remove("hrap1.txt");
                                        hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_1);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_1);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_1);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer1 < rtime_1.toFloat()) {
                                                rtime_1 = String(rsqPlayer1);
                                                rPlayer_1 = 1;
                                                rdatum_1 = rDatum1;
                                                NewHigh = 1;
                                                SD.remove("hrap1.txt");
                                                hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_1);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_1);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 2) {
                                if (rPlayer_2 == 0) {
                                        rtime_2 = String(rsqPlayer1);
                                        rPlayer_2 = 1;
                                        rdatum_2 = rDatum1;
                                        NewHigh = 1;
                                        SD.remove("hrap2.txt");
                                        hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_2);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_2);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_2);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer1 < rtime_2.toFloat()) {
                                                rtime_2 = String(rsqPlayer1);
                                                rPlayer_2 = 1;
                                                rdatum_2 = rDatum1;
                                                NewHigh = 1;
                                                SD.remove("hrap2.txt");
                                                hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_2);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_2);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        }  else if (rshooting == 3) {
                                if (rPlayer_3 == 0) {
                                        rtime_3 = String(rsqPlayer1);
                                        rPlayer_3 = 1;
                                        rdatum_3 = rDatum1;
                                        NewHigh = 1;
                                        SD.remove("hrap3.txt");
                                        hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_3);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_3);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_3);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer1 < rtime_3.toFloat()) {
                                                rtime_3 = String(rsqPlayer1);
                                                rPlayer_3 = 1;
                                                rdatum_3 = rDatum1;
                                                NewHigh = 1;
                                                SD.remove("hrap3.txt");
                                                hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_3);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_3);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        }
                } else
                if (ao == 2) {
                        rPlayer2 = interval1,3;
                        rsPlayer2 = interval1,3;
                        dtostrf(rsPlayer2,4, 3, rsqPlayer2);
                        rDatum2 = timeNow;
                        if (rshooting == 0) {
                                if (rplayer == 0) {
                                        rtime = String(rsqPlayer2);
                                        rplayer = 2;
                                        rdatum = rDatum2;
                                        NewHigh = 1;
                                        SD.remove("hrap.txt");
                                        hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer2 < rtime.toFloat()) {
                                                rtime = String(rsqPlayer2);
                                                rplayer = 2;
                                                rdatum = rDatum2;
                                                NewHigh = 1;
                                                SD.remove("hrap.txt");
                                                hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 1) {
                                if (rPlayer_1 == 0) {
                                        rtime_1 = String(rsqPlayer2);
                                        rPlayer_1 = 2;
                                        rdatum_1 = rDatum2;
                                        NewHigh = 1;
                                        SD.remove("hrap1.txt");
                                        hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_1);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_1);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_1);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer2 < rtime_1.toFloat()) {
                                                rtime_1 = String(rsqPlayer2);
                                                rPlayer_1 = 2;
                                                rdatum_1 = rDatum2;
                                                NewHigh = 1;
                                                SD.remove("hrap1.txt");
                                                hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_1);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_1);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 2) {
                                if (rPlayer_2 == 0) {
                                        rtime_2 = String(rsqPlayer2);
                                        rPlayer_2 = 2;
                                        rdatum_2 = rDatum2;
                                        NewHigh = 1;
                                        SD.remove("hrap2.txt");
                                        hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_2);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_2);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_2);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer2 < rtime_2.toFloat()) {
                                                rtime_2 = String(rsqPlayer2);
                                                rPlayer_2 = 2;
                                                rdatum_2 = rDatum2;
                                                NewHigh = 1;
                                                SD.remove("hrap2.txt");
                                                hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_2);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_2);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 3) {
                                if (rPlayer_3 == 0) {
                                        rtime_3 = String(rsqPlayer2);
                                        rPlayer_3 = 2;
                                        rdatum_3 = rDatum2;
                                        NewHigh = 1;
                                        SD.remove("hrap3.txt");
                                        hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_3);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_3);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_3);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer2 < rtime_3.toFloat()) {
                                                rtime_3 = String(rsqPlayer2);
                                                rPlayer_3 = 2;
                                                rdatum_3 = rDatum2;
                                                NewHigh = 1;
                                                SD.remove("hrap3.txt");
                                                hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_3);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_3);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        }
                } else
                if (ao == 3) {
                        rPlayer3 = interval1,3;
                        rsPlayer3 = interval1,3;
                        dtostrf(rsPlayer3,4, 3, rsqPlayer3);
                        rDatum3 = timeNow;
                        if (rshooting == 0) {
                                if (rplayer == 0) {
                                        rtime = String(rsqPlayer3);
                                        rplayer = 3;
                                        rdatum = rDatum3;
                                        NewHigh = 1;
                                        SD.remove("hrap.txt");
                                        hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer3 < rtime.toFloat()) {
                                                rtime = String(rsqPlayer3);
                                                rplayer = 3;
                                                rdatum = rDatum3;
                                                NewHigh = 1;
                                                SD.remove("hrap.txt");
                                                hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 1) {
                                if (rPlayer_1 == 0) {
                                        rtime_1 = String(rsqPlayer3);
                                        rPlayer_1 = 3;
                                        rdatum_1 = rDatum3;
                                        NewHigh = 1;
                                        SD.remove("hrap1.txt");
                                        hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_1);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_1);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_1);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer3 < rtime_1.toFloat()) {
                                                rtime_1 = String(rsqPlayer3);
                                                rPlayer_1 = 3;
                                                rdatum_1 = rDatum3;
                                                NewHigh = 1;
                                                SD.remove("hrap1.txt");
                                                hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_1);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_1);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 2) {
                                if (rPlayer_2 == 0) {
                                        rtime_2 = String(rsqPlayer3);
                                        rPlayer_2 = 3;
                                        rdatum_2 = rDatum3;
                                        NewHigh = 1;
                                        SD.remove("hrap2.txt");
                                        hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_2);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_2);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_2);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer3 < rtime_2.toFloat()) {
                                                rtime_2 = String(rsqPlayer3);
                                                rPlayer_2 = 3;
                                                rdatum_2 = rDatum3;
                                                NewHigh = 1;
                                                SD.remove("hrap2.txt");
                                                hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_2);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_2);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 3) {
                                if (rPlayer_3 == 0) {
                                        rtime_3 = String(rsqPlayer3);
                                        rPlayer_3 = 3;
                                        rdatum_3 = rDatum3;
                                        NewHigh = 1;
                                        SD.remove("hrap3.txt");
                                        hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_3);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_3);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_3);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer3 < rtime_3.toFloat()) {
                                                rtime_3 = String(rsqPlayer3);
                                                rPlayer_3 = 3;
                                                rdatum_3 = rDatum3;
                                                NewHigh = 1;
                                                SD.remove("hrap3.txt");
                                                hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_3);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_3);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        }
                } else
                if (ao == 4) {
                        rPlayer4 = interval1,3;
                        rsPlayer4 = interval1,3;
                        dtostrf(rsPlayer4,4, 3, rsqPlayer4);
                        rDatum4 = timeNow;
                        if (rshooting == 0) {
                                if (rplayer == 0) {
                                        rtime = String(rsqPlayer4);
                                        rplayer = 4;
                                        rdatum = rDatum4;
                                        NewHigh = 1;
                                        SD.remove("hrap.txt");
                                        hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer4 < rtime.toFloat()) {
                                                rtime = String(rsqPlayer4);
                                                rplayer = 4;
                                                rdatum = rDatum4;
                                                NewHigh = 1;
                                                SD.remove("hrap.txt");
                                                hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 1) {
                                if (rPlayer_1 == 0) {
                                        rtime_1 = String(rsqPlayer4);
                                        rPlayer_1 = 4;
                                        rdatum_1 = rDatum4;
                                        NewHigh = 1;
                                        SD.remove("hrap1.txt");
                                        hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_1);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_1);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_1);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer4 < rtime_1.toFloat()) {
                                                rtime_1 = String(rsqPlayer4);
                                                rPlayer_1 = 4;
                                                rdatum_1 = rDatum4;
                                                NewHigh = 1;
                                                SD.remove("hrap1.txt");
                                                hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_1);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_1);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 2) {
                                if (rPlayer_2 == 0) {
                                        rtime_2 = String(rsqPlayer4);
                                        rPlayer_2 = 4;
                                        rdatum_2 = rDatum4;
                                        NewHigh = 1;
                                        SD.remove("hrap2.txt");
                                        hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_2);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_2);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_2);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer4 < rtime_2.toFloat()) {
                                                rtime_2 = String(rsqPlayer4);
                                                rPlayer_2 = 4;
                                                rdatum_2 = rDatum4;
                                                NewHigh = 1;
                                                SD.remove("hrap2.txt");
                                                hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_2);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_2);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 3) {
                                if (rPlayer_3 == 0) {
                                        rtime_3 = String(rsqPlayer4);
                                        rPlayer_3 = 4;
                                        rdatum_3 = rDatum4;
                                        NewHigh = 1;
                                        SD.remove("hrap3.txt");
                                        hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_3);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_3);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_3);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer4 < rtime_3.toFloat()) {
                                                rtime_3 = String(rsqPlayer4);
                                                rPlayer_3 = 4;
                                                rdatum_3 = rDatum4;
                                                NewHigh = 1;
                                                SD.remove("hrap3.txt");
                                                hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_3);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_3);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        }
                }
                rPrintScore = 1;
                delay(5000);
        }
        //Display highscore on lcd.
        if (NewHigh == 1) {
                if (rshooting == 0) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(rplayer);
                        lcd.setCursor(0,3);
                        lcd.print("Time: ");
                        lcd.print(rtime);
                        lcd.print(" sec");
                        NewHigh = 0;
                        delay(5000);
                } else if (rshooting == 1) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(rPlayer_1);
                        lcd.setCursor(0,3);
                        lcd.print("Time: ");
                        lcd.print(rtime_1);
                        lcd.print(" sec");
                        NewHigh = 0;
                        delay(5000);
                } else if (rshooting == 2) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(rPlayer_2);
                        lcd.setCursor(0,3);
                        lcd.print("Time: ");
                        lcd.print(rtime_2);
                        lcd.print(" sec");
                        NewHigh = 0;
                        delay(5000);
                } else if (rshooting == 3) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(rPlayer_3);
                        lcd.setCursor(0,3);
                        lcd.print("Time: ");
                        lcd.print(rtime_3);
                        lcd.print(" sec");
                        NewHigh = 0;
                        delay(5000);
                }
        }

        if (rPrintScore == 1) {
                //Make it possible to print.
                lcd.clear();
                printhigh = 1;
                while (printhigh = 1) {

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Press on select");
                        lcd.setCursor(0,2);
                        lcd.print("to print results");
                        lcd.setCursor(0,3);
                        lcd.print("or else press OK");
                        if (digitalRead(okButton)==HIGH) {
                                printhigh = 0;
                                qqdisplay = 0;
                                mainscreen = 0;
                                rPrintScore = 0;
                                displayCodeEntryScreen();
                                break;
                        }
                        if (digitalRead(selectButton)==HIGH) {
                                printhigh = 0;
                                rPrintScore = 0;
                                printRapid();
                                break;
                        }
                }
        }
        changeInfo = 1;
        resultsRapid = 1;
}

//Rapid fire wireless.
void doRapidFireW()
{
        for (int ao=1; ao<=players; ao++)
        {
                if (ao == 1) {
                        delay(100);
                } else {
                        lcd.clear();

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Time for");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(ao);
                        lcd.setCursor(0,3);
                        lcd.print("In 30 sec");
                        delay(30000);
                }

                randomSeed(millis());

                int currentPort=random(4);
                int newPort=random(4);
                int maxRounds=6;
                if (rshooting == 0) {
                        maxRounds=6;
                } else if (rshooting == 1) {
                        maxRounds=10;
                } else if (rshooting == 2) {
                        maxRounds=20;
                } else if (rshooting == 3) {
                        maxRounds=32;
                }
                time1=millis();
                delay(5);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Rapid Fire");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(ao);

                delay(2000);

                for (int i=0; i<maxRounds; i++)
                {
                        delay(10);
                        lcd.setCursor(0,3);
                        lcd.print("Round ");
                        lcd.print(i+1);
                        lcd.print(" of ");
                        lcd.print(maxRounds);
                        bool skicka3 = false;
                        bool skicka4 = false;
                        bool skicka5 = false;
                        bool skicka6 = false;
                        bool skicka7 = false;
                        bool skicka8 = false;
                        int backloop = 0;

                        if (currentPort == 0) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[0]);
                                radio.openReadingPipe(1,pipes[5]);
                                transmitterId = 16;
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                skicka1 = true;
                        } else if (currentPort == 1) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[2]);
                                radio.openReadingPipe(1,pipes[6]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 26;
                                skicka3 = true;


                        } else if (currentPort == 2) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[3]);
                                radio.openReadingPipe(1,pipes[7]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 36;
                                skicka5 = true;


                        } else if (currentPort == 3) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[4]);
                                radio.openReadingPipe(1,pipes[8]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 46;
                                skicka7 = true;


                        }

                        while (skicka1 == true)
                        {

                                if(radio.write(&transmitterId, sizeof(transmitterId))) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);

                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka2 = true;
                                                        skicka1 = false;
                                                        Serial.println(transmitterId);

                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }




                        }

                        while (skicka2 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData, sizeof(nockData));
                                if (radio.available(pipes[5])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[5])) {
                                                Serial.println("Start Lissening");
                                                radio.read(&senderId, sizeof(senderId));

                                        }
                                }  else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 17)    //Target 1
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka2 = false;
                                        skicka1 = false;

                                }


                        }

                        while (skicka3 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData3) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka4 = true;
                                                        skicka3 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka4 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData2, sizeof(nockData2));
                                if (radio.available(pipes[6])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[6])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Recived: ");
                                                Serial.println(senderId);

                                        }

                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 27)    //Target 2
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka4 = false;
                                        skicka3 = false;
                                }


                        }
                        while (skicka5 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData5) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka6 = true;
                                                        skicka5 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka6 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData3, sizeof(nockData3));
                                if (radio.available(pipes[7])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[7])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Recive: ");
                                                Serial.println(senderId);

                                        }

                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 37)    //Target 3
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka5 = false;
                                        skicka6 = false;
                                }


                        }
                        while (skicka7 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData7) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka8 = true;
                                                        skicka7 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka8 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData4, sizeof(nockData4));
                                if (radio.available(pipes[8])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[8])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Recived: ");
                                                Serial.println(senderId);

                                        }

                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 47)    //Target 4
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka7 = false;
                                        skicka8 = false;
                                }


                        }
                        newPort=random(4);
                        while (newPort==currentPort) newPort=random(4);

                        currentPort=newPort;
                }

                time2=millis();
                interval1=(time2-time1);
                interval1=interval1/1000;
                delay(5);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Rapid Fire");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(ao);
                lcd.setCursor(0,3);
                lcd.print("Time: ");
                Serial.print(interval1,3);
                lcd.print(interval1,3);
                lcd.print(" sec");
                DateTime now = RTC.now();
                timeNow = String(now.year()) + "-";
                if (now.month() < 10) {
                        timeNow = timeNow + "0" + String(now.month());
                } else {
                        timeNow = timeNow + String(now.month());
                }
                if (now.day() < 10) {
                        timeNow = timeNow + "-0" + String(now.day());
                } else {
                        timeNow = timeNow + "-" + String(now.day());
                }
                if (now.hour() < 10) {
                        timeNow = timeNow + " 0" + String(now.hour());
                } else {
                        timeNow = timeNow + " " + String(now.hour());
                }
                if (now.minute() < 10) {
                        timeNow = timeNow + ":0" + String(now.minute());
                } else {
                        timeNow = timeNow + ":" + String(now.minute());
                }
                //We add if there is new highscore.
                if (ao == 1) {
                        rsPlayer1 = interval1,3;
                        rPlayer1 = interval1,3;
                        dtostrf(rsPlayer1, 4, 3, rsqPlayer1);
                        rDatum1 = timeNow;
                        if (rshooting == 0) {
                                if (rplayer == 0) {
                                        rtime = String(rsqPlayer1);
                                        rplayer = 1;
                                        rdatum = rDatum1;
                                        NewHigh = 1;
                                        SD.remove("hrap.txt");
                                        hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer1 < rtime.toFloat()) {
                                                rtime = String(rsqPlayer1);
                                                rplayer = 1;
                                                rdatum = rDatum1;
                                                NewHigh = 1;
                                                SD.remove("hrap.txt");
                                                hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 1) {
                                if (rPlayer_1 == 0) {
                                        rtime_1 = String(rsqPlayer1);
                                        rPlayer_1 = 1;
                                        rdatum_1 = rDatum1;
                                        NewHigh = 1;
                                        SD.remove("hrap1.txt");
                                        hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_1);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_1);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_1);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer1 < rtime_1.toFloat()) {
                                                rtime_1 = String(rsqPlayer1);
                                                rPlayer_1 = 1;
                                                rdatum_1 = rDatum1;
                                                NewHigh = 1;
                                                SD.remove("hrap1.txt");
                                                hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_1);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_1);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 2) {
                                if (rPlayer_2 == 0) {
                                        rtime_2 = String(rsqPlayer1);
                                        rPlayer_2 = 1;
                                        rdatum_2 = rDatum1;
                                        NewHigh = 1;
                                        SD.remove("hrap2.txt");
                                        hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_2);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_2);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_2);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer1 < rtime_2.toFloat()) {
                                                rtime_2 = String(rsqPlayer1);
                                                rPlayer_2 = 1;
                                                rdatum_2 = rDatum1;
                                                NewHigh = 1;
                                                SD.remove("hrap2.txt");
                                                hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_2);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_2);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        }  else if (rshooting == 3) {
                                if (rPlayer_3 == 0) {
                                        rtime_3 = String(rsqPlayer1);
                                        rPlayer_3 = 1;
                                        rdatum_3 = rDatum1;
                                        NewHigh = 1;
                                        SD.remove("hrap3.txt");
                                        hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_3);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_3);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_3);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer1 < rtime_3.toFloat()) {
                                                rtime_3 = String(rsqPlayer1);
                                                rPlayer_3 = 1;
                                                rdatum_3 = rDatum1;
                                                NewHigh = 1;
                                                SD.remove("hrap3.txt");
                                                hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_3);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_3);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        }
                } else
                if (ao == 2) {
                        rPlayer2 = interval1,3;
                        rsPlayer2 = interval1,3;
                        dtostrf(rsPlayer2,4, 3, rsqPlayer2);
                        rDatum2 = timeNow;
                        if (rshooting == 0) {
                                if (rplayer == 0) {
                                        rtime = String(rsqPlayer2);
                                        rplayer = 2;
                                        rdatum = rDatum2;
                                        NewHigh = 1;
                                        SD.remove("hrap.txt");
                                        hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer2 < rtime.toFloat()) {
                                                rtime = String(rsqPlayer2);
                                                rplayer = 2;
                                                rdatum = rDatum2;
                                                NewHigh = 1;
                                                SD.remove("hrap.txt");
                                                hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 1) {
                                if (rPlayer_1 == 0) {
                                        rtime_1 = String(rsqPlayer2);
                                        rPlayer_1 = 2;
                                        rdatum_1 = rDatum2;
                                        NewHigh = 1;
                                        SD.remove("hrap1.txt");
                                        hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_1);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_1);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_1);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer2 < rtime_1.toFloat()) {
                                                rtime_1 = String(rsqPlayer2);
                                                rPlayer_1 = 2;
                                                rdatum_1 = rDatum2;
                                                NewHigh = 1;
                                                SD.remove("hrap1.txt");
                                                hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_1);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_1);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 2) {
                                if (rPlayer_2 == 0) {
                                        rtime_2 = String(rsqPlayer2);
                                        rPlayer_2 = 2;
                                        rdatum_2 = rDatum2;
                                        NewHigh = 1;
                                        SD.remove("hrap2.txt");
                                        hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_2);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_2);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_2);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer2 < rtime_2.toFloat()) {
                                                rtime_2 = String(rsqPlayer2);
                                                rPlayer_2 = 2;
                                                rdatum_2 = rDatum2;
                                                NewHigh = 1;
                                                SD.remove("hrap2.txt");
                                                hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_2);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_2);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 3) {
                                if (rPlayer_3 == 0) {
                                        rtime_3 = String(rsqPlayer2);
                                        rPlayer_3 = 2;
                                        rdatum_3 = rDatum2;
                                        NewHigh = 1;
                                        SD.remove("hrap3.txt");
                                        hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_3);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_3);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_3);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer2 < rtime_3.toFloat()) {
                                                rtime_3 = String(rsqPlayer2);
                                                rPlayer_3 = 2;
                                                rdatum_3 = rDatum2;
                                                NewHigh = 1;
                                                SD.remove("hrap3.txt");
                                                hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_3);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_3);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        }
                } else
                if (ao == 3) {
                        rPlayer3 = interval1,3;
                        rsPlayer3 = interval1,3;
                        dtostrf(rsPlayer3,4, 3, rsqPlayer3);
                        rDatum3 = timeNow;
                        if (rshooting == 0) {
                                if (rplayer == 0) {
                                        rtime = String(rsqPlayer3);
                                        rplayer = 3;
                                        rdatum = rDatum3;
                                        NewHigh = 1;
                                        SD.remove("hrap.txt");
                                        hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer3 < rtime.toFloat()) {
                                                rtime = String(rsqPlayer3);
                                                rplayer = 3;
                                                rdatum = rDatum3;
                                                NewHigh = 1;
                                                SD.remove("hrap.txt");
                                                hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 1) {
                                if (rPlayer_1 == 0) {
                                        rtime_1 = String(rsqPlayer3);
                                        rPlayer_1 = 3;
                                        rdatum_1 = rDatum3;
                                        NewHigh = 1;
                                        SD.remove("hrap1.txt");
                                        hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_1);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_1);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_1);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer3 < rtime_1.toFloat()) {
                                                rtime_1 = String(rsqPlayer3);
                                                rPlayer_1 = 3;
                                                rdatum_1 = rDatum3;
                                                NewHigh = 1;
                                                SD.remove("hrap1.txt");
                                                hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_1);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_1);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 2) {
                                if (rPlayer_2 == 0) {
                                        rtime_2 = String(rsqPlayer3);
                                        rPlayer_2 = 3;
                                        rdatum_2 = rDatum3;
                                        NewHigh = 1;
                                        SD.remove("hrap2.txt");
                                        hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_2);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_2);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_2);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer3 < rtime_2.toFloat()) {
                                                rtime_2 = String(rsqPlayer3);
                                                rPlayer_2 = 3;
                                                rdatum_2 = rDatum3;
                                                NewHigh = 1;
                                                SD.remove("hrap2.txt");
                                                hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_2);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_2);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 3) {
                                if (rPlayer_3 == 0) {
                                        rtime_3 = String(rsqPlayer3);
                                        rPlayer_3 = 3;
                                        rdatum_3 = rDatum3;
                                        NewHigh = 1;
                                        SD.remove("hrap3.txt");
                                        hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_3);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_3);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_3);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer3 < rtime_3.toFloat()) {
                                                rtime_3 = String(rsqPlayer3);
                                                rPlayer_3 = 3;
                                                rdatum_3 = rDatum3;
                                                NewHigh = 1;
                                                SD.remove("hrap3.txt");
                                                hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_3);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_3);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        }
                } else
                if (ao == 4) {
                        rPlayer4 = interval1,3;
                        rsPlayer4 = interval1,3;
                        dtostrf(rsPlayer4,4, 3, rsqPlayer4);
                        rDatum4 = timeNow;
                        if (rshooting == 0) {
                                if (rplayer == 0) {
                                        rtime = String(rsqPlayer4);
                                        rplayer = 4;
                                        rdatum = rDatum4;
                                        NewHigh = 1;
                                        SD.remove("hrap.txt");
                                        hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rplayer);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer4 < rtime.toFloat()) {
                                                rtime = String(rsqPlayer4);
                                                rplayer = 4;
                                                rdatum = rDatum4;
                                                NewHigh = 1;
                                                SD.remove("hrap.txt");
                                                hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 1) {
                                if (rPlayer_1 == 0) {
                                        rtime_1 = String(rsqPlayer4);
                                        rPlayer_1 = 4;
                                        rdatum_1 = rDatum4;
                                        NewHigh = 1;
                                        SD.remove("hrap1.txt");
                                        hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_1);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_1);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_1);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer4 < rtime_1.toFloat()) {
                                                rtime_1 = String(rsqPlayer4);
                                                rPlayer_1 = 4;
                                                rdatum_1 = rDatum4;
                                                NewHigh = 1;
                                                SD.remove("hrap1.txt");
                                                hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_1);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_1);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 2) {
                                if (rPlayer_2 == 0) {
                                        rtime_2 = String(rsqPlayer4);
                                        rPlayer_2 = 4;
                                        rdatum_2 = rDatum4;
                                        NewHigh = 1;
                                        SD.remove("hrap2.txt");
                                        hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_2);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_2);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_2);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer4 < rtime_2.toFloat()) {
                                                rtime_2 = String(rsqPlayer4);
                                                rPlayer_2 = 4;
                                                rdatum_2 = rDatum4;
                                                NewHigh = 1;
                                                SD.remove("hrap2.txt");
                                                hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_2);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_2);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        } else if (rshooting == 3) {
                                if (rPlayer_3 == 0) {
                                        rtime_3 = String(rsqPlayer4);
                                        rPlayer_3 = 4;
                                        rdatum_3 = rDatum4;
                                        NewHigh = 1;
                                        SD.remove("hrap3.txt");
                                        hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                        if (hquickFil) {
                                                hquickFil.print(rPlayer_3);
                                                hquickFil.print(",");
                                                hquickFil.print(rtime_3);
                                                hquickFil.print(",");
                                                hquickFil.println(rdatum_3);
                                                hquickFil.close();
                                        }
                                } else {
                                        if (rsPlayer4 < rtime_3.toFloat()) {
                                                rtime_3 = String(rsqPlayer4);
                                                rPlayer_3 = 4;
                                                rdatum_3 = rDatum4;
                                                NewHigh = 1;
                                                SD.remove("hrap3.txt");
                                                hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(rPlayer_3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(rtime_3);
                                                        hquickFil.print(",");
                                                        hquickFil.println(rdatum_3);
                                                        hquickFil.close();
                                                }
                                        }
                                }
                        }
                }
                rPrintScore = 1;
                delay(5000);
        }
        //Display highscore on lcd.
        if (NewHigh == 1) {
                if (rshooting == 0) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(rplayer);
                        lcd.setCursor(0,3);
                        lcd.print("Time: ");
                        lcd.print(rtime);
                        lcd.print(" sec");
                        NewHigh = 0;
                        delay(5000);
                } else if (rshooting == 1) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(rPlayer_1);
                        lcd.setCursor(0,3);
                        lcd.print("Time: ");
                        lcd.print(rtime_1);
                        lcd.print(" sec");
                        NewHigh = 0;
                        delay(5000);
                } else if (rshooting == 2) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(rPlayer_2);
                        lcd.setCursor(0,3);
                        lcd.print("Time: ");
                        lcd.print(rtime_2);
                        lcd.print(" sec");
                        NewHigh = 0;
                        delay(5000);
                } else if (rshooting == 3) {
                        lcd.clear();
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("New highscore to:");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(rPlayer_3);
                        lcd.setCursor(0,3);
                        lcd.print("Time: ");
                        lcd.print(rtime_3);
                        lcd.print(" sec");
                        NewHigh = 0;
                        delay(5000);
                }
        }

        if (rPrintScore == 1) {
                //Enable to print out.
                lcd.clear();
                printhigh = 1;
                while (printhigh = 1) {

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Press on select");
                        lcd.setCursor(0,2);
                        lcd.print("to print results");
                        lcd.setCursor(0,3);
                        lcd.print("or else on OK");
                        if (digitalRead(okButton)==HIGH) {
                                printhigh = 0;
                                qqdisplay = 0;
                                mainscreen = 0;
                                rPrintScore = 0;
                                displayCodeEntryScreen();
                                break;
                        }
                        if (digitalRead(selectButton)==HIGH) {
                                printhigh = 0;
                                rPrintScore = 0;
                                printRapid();
                                break;
                        }
                }
        }
        changeInfo = 1;
        resultsRapid = 1;
}
//Quickdraw wired.
void doQuickDraw()
{
        for (int ao=1; ao<=players; ao++)
        {
                if (ao == 1) {
                        delay(100);
                } else {
                        lcd.clear();

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Time for");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(ao);
                        lcd.setCursor(0,3);
                        lcd.print("In 30 sec");
                        delay(30000);
                }

                randomSeed(millis());

                int firstTime=true;

                int currentPort=random(4);
                int maxRounds=8;

                Serial.write(0x0C); delay(5);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Quickdraw");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(ao);
                delay(2000);

                for (int i=0; i<maxRounds; i++)
                {
                        delay(5);

                        lcd.setCursor(0,2);
                        lcd.print("Round ");
                        lcd.print(i+1);
                        lcd.print(" of ");
                        lcd.print(maxRounds);
                        lcd.setCursor(0,3);
                        lcd.print("Ready ?       ");
                        delay(random(3000)+1000);
                        if (firstTime==true) { Serial.write(0xDF); firstTime=false; }

                        time1=millis();
                        digitalWrite(led[currentPort],HIGH);
                        switch (currentPort)
                        {
                        case 0:
                                while (digitalRead(ins[0])==LOW) ;
                                break;
                        case 1:
                                while (digitalRead(ins[1])==LOW) ;
                                break;
                        case 2:
                                while (digitalRead(ins[2])==LOW) ;
                                break;
                        case 3:
                                while (digitalRead(ins[3])==LOW) ;
                                break;
                        }
                        time2=millis();
                        interval1=(time2-time1);
                        interval1=interval1/1000;
                        digitalWrite(led[currentPort],LOW);
                        delay(5);
                        lcd.setCursor(0,3);
                        lcd.print(interval1,3);
                        lcd.print(" sec");
                        DateTime now = RTC.now();
                        timeNow = String(now.year()) + "-";
                        if (now.month() < 10) {
                                timeNow = timeNow + "0" + String(now.month());
                        } else {
                                timeNow = timeNow + String(now.month());
                        }
                        if (now.day() < 10) {
                                timeNow = timeNow + "-0" + String(now.day());
                        } else {
                                timeNow = timeNow + "-" + String(now.day());
                        }
                        if (now.hour() < 10) {
                                timeNow = timeNow + " 0" + String(now.hour());
                        } else {
                                timeNow = timeNow + " " + String(now.hour());
                        }
                        if (now.minute() < 10) {
                                timeNow = timeNow + ":0" + String(now.minute());
                        } else {
                                timeNow = timeNow + ":" + String(now.minute());
                        }
                        //Is it hiscore ?
                        if (i == 0) {
                                if (ao == 1) {
                                        qP11=interval1,3;
                                        qfP11=interval1,3;
                                        dtostrf(qfP11,4, 3, qsP11);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP21=interval1,3;
                                        qfP21=interval1,3;
                                        dtostrf(qfP21,4, 3, qsP21);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP31=interval1,3;
                                        qfP31=interval1,3;
                                        dtostrf(qfP31,4, 3, qsP31);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP41=interval1,3;
                                        qfP41=interval1,3;
                                        dtostrf(qfP41,4, 3, qsP41);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 1) {
                                if (ao == 1) {
                                        qP12=interval1,3;
                                        qfP12=interval1,3;
                                        dtostrf(qfP12,4, 3, qsP12);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP22=interval1,3;
                                        qfP22=interval1,3;
                                        dtostrf(qfP22,4, 3, qsP22);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP32=interval1,3;
                                        qfP32=interval1,3;
                                        dtostrf(qfP32,4, 3, qsP32);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP42=interval1,3;
                                        qfP42=interval1,3;
                                        dtostrf(qfP42,4, 3, qsP42);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 2) {
                                if (ao == 1) {
                                        qP13=interval1,3;
                                        qfP13=interval1,3;
                                        dtostrf(qfP13,4, 3, qsP13);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP23=interval1,3;
                                        qfP23=interval1,3;
                                        dtostrf(qfP23,4, 3, qsP23);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP33=interval1,3;
                                        qfP33=interval1,3;
                                        dtostrf(qfP33,4, 3, qsP33);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP43=interval1,3;
                                        qfP43=interval1,3;
                                        dtostrf(qfP43,4, 3, qsP43);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 3) {
                                if (ao == 1) {
                                        qP14=interval1,3;
                                        qfP14=interval1,3;
                                        dtostrf(qfP14,4, 3, qsP14);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP24=interval1,3;
                                        qfP24=interval1,3;
                                        dtostrf(qfP24,4, 3, qsP24);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP34=interval1,3;
                                        qfP34=interval1,3;
                                        dtostrf(qfP34,4, 3, qsP34);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP44=interval1,3;
                                        qfP44=interval1,3;
                                        dtostrf(qfP44,4, 3, qsP44);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 4) {
                                if (ao == 1) {
                                        qP15=interval1,3;
                                        qfP15=interval1,3;
                                        dtostrf(qfP15,4, 3, qsP15);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP25=interval1,3;
                                        qfP25=interval1,3;
                                        dtostrf(qfP25,4, 3, qsP25);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP35=interval1,3;
                                        qfP35=interval1,3;
                                        dtostrf(qfP35,4, 3, qsP35);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP45=interval1,3;
                                        qfP45=interval1,3;
                                        dtostrf(qfP45,4, 3, qsP45);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 5) {
                                if (ao == 1) {
                                        qP16=interval1,3;
                                        qfP16=interval1,3;
                                        dtostrf(qfP16,4, 3, qsP16);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP26=interval1,3;
                                        qfP26=interval1,3;
                                        dtostrf(qfP26,4, 3, qsP26);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP36=interval1,3;
                                        qfP36=interval1,3;
                                        dtostrf(qfP36,4, 3, qsP36);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP46=interval1,3;
                                        qfP46=interval1,3;
                                        dtostrf(qfP46,4, 3, qsP46);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 6) {
                                if (ao == 1) {
                                        qP17=interval1,3;
                                        qfP17=interval1,3;
                                        dtostrf(qfP17,4, 3, qsP17);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP27=interval1,3;
                                        qfP27=interval1,3;
                                        dtostrf(qfP27,4, 3, qsP27);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP37=interval1,3;
                                        qfP37=interval1,3;
                                        dtostrf(qfP37,4, 3, qsP37);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP47=interval1,3;
                                        qfP47=interval1,3;
                                        dtostrf(qfP47,4, 3, qsP47);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 7) {
                                if (ao == 1) {
                                        float scoreres;
                                        float highscores;
                                        qP18=interval1,3;
                                        qfP18=interval1,3;
                                        dtostrf(qfP18,4, 3, qsP18);
                                        qDatum1=timeNow;
                                        highscores = qftime1 + qftime2 + qftime3 + qftime4 + qftime5 + qftime6 + qftime7 + qftime8;
                                        scoreres = qfP11 + qfP12 + qfP13 + qfP14 + qfP15 + qfP16 + qfP17 + qfP18;
                                        if (qqplayer == 0) {
                                                qtime1 = String(qsP11);
                                                qftime1 = qfP11;
                                                qtime2 = String(qsP12);
                                                qftime2 = qfP12;
                                                qtime3 = String(qsP13);
                                                qftime3 = qfP13;
                                                qtime4 = String(qsP14);
                                                qftime4 = qfP14;
                                                qtime5 = String(qsP15);
                                                qftime5 = qfP15;
                                                qtime6 = String(qsP16);
                                                qftime6 = qfP16;
                                                qtime7 = String(qsP17);
                                                qftime7 = qfP17;
                                                qtime8 = String(qsP18);
                                                qftime8 = qfP18;
                                                qqplayer = 1;
                                                qqdatum = qDatum1;
                                                NewHigh = 1;
                                                SD.remove("hqu.txt");
                                                hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(qqplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime4);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime5);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime6);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime7);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime8);
                                                        hquickFil.print(",");
                                                        hquickFil.println(qqdatum);
                                                        hquickFil.close();
                                                }
                                        }else {
                                                if (scoreres < highscores) {
                                                        qtime1 = String(qsP11);
                                                        qftime1 = qfP11;
                                                        qtime2 = String(qsP12);
                                                        qftime2 = qfP12;
                                                        qtime3 = String(qsP13);
                                                        qftime3 = qfP13;
                                                        qtime4 = String(qsP14);
                                                        qftime4 = qfP14;
                                                        qtime5 = String(qsP15);
                                                        qftime5 = qfP15;
                                                        qtime6 = String(qsP16);
                                                        qftime6 = qfP16;
                                                        qtime7 = String(qsP17);
                                                        qftime7 = qfP17;
                                                        qtime8 = String(qsP18);
                                                        qftime8 = qfP18;
                                                        qqplayer = 1;
                                                        qqdatum = qDatum1;
                                                        NewHigh = 1;
                                                        SD.remove("hqu.txt");
                                                        hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                        if (hquickFil) {
                                                                hquickFil.print(qqplayer);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime1);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime2);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime3);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime4);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime5);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime6);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime7);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime8);
                                                                hquickFil.print(",");
                                                                hquickFil.println(qqdatum);
                                                                hquickFil.close();
                                                        }
                                                }
                                        }
                                } else if (ao == 2) {
                                        float scoreres;
                                        float highscores;
                                        qP28=interval1,3;
                                        qfP28=interval1,3;
                                        dtostrf(qfP28,4, 3, qsP28);
                                        qDatum2=timeNow;
                                        highscores = qftime1 + qftime2 + qftime3 + qftime4 + qftime5 + qftime6 + qftime7 + qftime8;
                                        scoreres = qfP21 + qfP22 + qfP23 + qfP24 + qfP25 + qfP26 + qfP27 + qfP28;
                                        if (qqplayer == 0) {
                                                qtime1 = String(qsP21);
                                                qftime1 = qfP21;
                                                qtime2 = String(qsP22);
                                                qftime2 = qfP22;
                                                qtime3 = String(qsP23);
                                                qftime3 = qfP23;
                                                qtime4 = String(qsP24);
                                                qftime4 = qfP24;
                                                qtime5 = String(qsP25);
                                                qftime5 = qfP25;
                                                qtime6 = String(qsP26);
                                                qftime6 = qfP26;
                                                qtime7 = String(qsP27);
                                                qftime7 = qfP27;
                                                qtime8 = String(qsP28);
                                                qftime8 = qfP28;
                                                qqplayer = 2;
                                                qqdatum = qDatum2;
                                                NewHigh = 1;
                                                SD.remove("hqu.txt");
                                                hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(qqplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime4);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime5);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime6);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime7);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime8);
                                                        hquickFil.print(",");
                                                        hquickFil.println(qqdatum);
                                                        hquickFil.close();
                                                }
                                        }else {
                                                if (scoreres < highscores) {
                                                        qtime1 = String(qsP21);
                                                        qftime1 = qfP21;
                                                        qtime2 = String(qsP22);
                                                        qftime2 = qfP22;
                                                        qtime3 = String(qsP23);
                                                        qftime3 = qfP23;
                                                        qtime4 = String(qsP24);
                                                        qftime4 = qfP24;
                                                        qtime5 = String(qsP25);
                                                        qftime5 = qfP25;
                                                        qtime6 = String(qsP26);
                                                        qftime6 = qfP26;
                                                        qtime7 = String(qsP27);
                                                        qftime7 = qfP27;
                                                        qtime8 = String(qsP28);
                                                        qftime8 = qfP28;
                                                        qqplayer = 2;
                                                        qqdatum = qDatum2;
                                                        NewHigh = 1;
                                                        SD.remove("hqu.txt");
                                                        hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                        if (hquickFil) {
                                                                hquickFil.print(qqplayer);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime1);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime2);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime3);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime4);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime5);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime6);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime7);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime8);
                                                                hquickFil.print(",");
                                                                hquickFil.println(qqdatum);
                                                                hquickFil.close();
                                                        }
                                                }
                                        }
                                } else if (ao == 3) {
                                        float scoreres;
                                        float highscores;
                                        qP38=interval1,3;
                                        qfP38=interval1,3;
                                        dtostrf(qfP38,4, 3, qsP38);
                                        qDatum3=timeNow;
                                        highscores = qftime1 + qftime2 + qftime3 + qftime4 + qftime5 + qftime6 + qftime7 + qftime8;
                                        scoreres = qfP31 + qfP32 + qfP33 + qfP34 + qfP35 + qfP36 + qfP37 + qfP38;
                                        if (qqplayer == 0) {
                                                qtime1 = String(qsP31);
                                                qftime1 = qfP31;
                                                qtime2 = String(qsP32);
                                                qftime2 = qfP32;
                                                qtime3 = String(qsP33);
                                                qftime3 = qfP33;
                                                qtime4 = String(qsP34);
                                                qftime4 = qfP34;
                                                qtime5 = String(qsP35);
                                                qftime5 = qfP35;
                                                qtime6 = String(qsP36);
                                                qftime6 = qfP36;
                                                qtime7 = String(qsP37);
                                                qftime7 = qfP37;
                                                qtime8 = String(qsP38);
                                                qftime8 = qfP38;
                                                qqplayer = 3;
                                                qqdatum = qDatum3;
                                                NewHigh = 1;
                                                SD.remove("hqu.txt");
                                                hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(qqplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime4);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime5);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime6);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime7);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime8);
                                                        hquickFil.print(",");
                                                        hquickFil.println(qqdatum);
                                                        hquickFil.close();
                                                }
                                        }else {
                                                if (scoreres < highscores) {
                                                        qtime1 = String(qsP31);
                                                        qftime1 = qfP31;
                                                        qtime2 = String(qsP32);
                                                        qftime2 = qfP32;
                                                        qtime3 = String(qsP33);
                                                        qftime3 = qfP33;
                                                        qtime4 = String(qsP34);
                                                        qftime4 = qfP34;
                                                        qtime5 = String(qsP35);
                                                        qftime5 = qfP35;
                                                        qtime6 = String(qsP36);
                                                        qftime6 = qfP36;
                                                        qtime7 = String(qsP37);
                                                        qftime7 = qfP37;
                                                        qtime8 = String(qsP38);
                                                        qftime8 = qfP38;
                                                        qqplayer = 3;
                                                        qqdatum = qDatum3;
                                                        NewHigh = 1;
                                                        SD.remove("hqu.txt");
                                                        hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                        if (hquickFil) {
                                                                hquickFil.print(qqplayer);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime1);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime2);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime3);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime4);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime5);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime6);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime7);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime8);
                                                                hquickFil.print(",");
                                                                hquickFil.println(qqdatum);
                                                                hquickFil.close();
                                                        }
                                                }
                                        }
                                } else if (ao == 4) {
                                        float scoreres;
                                        float highscores;
                                        qP48=interval1,3;
                                        qfP48=interval1,3;
                                        dtostrf(qfP48,4, 3, qsP48);
                                        qDatum4=timeNow;
                                        highscores = qftime1 + qftime2 + qftime3 + qftime4 + qftime5 + qftime6 + qftime7 + qftime8;
                                        scoreres = qfP41 + qfP42 + qfP43 + qfP44 + qfP45 + qfP46 + qfP47 + qfP48;
                                        if (qqplayer == 0) {
                                                qtime1 = String(qsP41);
                                                qftime1 = qfP41;
                                                qtime2 = String(qsP42);
                                                qftime2 = qfP42;
                                                qtime3 = String(qsP43);
                                                qftime3 = qfP43;
                                                qtime4 = String(qsP44);
                                                qftime4 = qfP44;
                                                qtime5 = String(qsP45);
                                                qftime5 = qfP45;
                                                qtime6 = String(qsP46);
                                                qftime6 = qfP46;
                                                qtime7 = String(qsP47);
                                                qftime7 = qfP47;
                                                qtime8 = String(qsP48);
                                                qftime8 = qfP48;
                                                qqplayer = 4;
                                                qqdatum = qDatum4;
                                                NewHigh = 1;
                                                SD.remove("hqu.txt");
                                                hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(qqplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime4);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime5);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime6);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime7);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime8);
                                                        hquickFil.print(",");
                                                        hquickFil.println(qqdatum);
                                                        hquickFil.close();
                                                }
                                        }else {
                                                if (scoreres < highscores) {
                                                        qtime1 = String(qsP41);
                                                        qftime1 = qfP41;
                                                        qtime2 = String(qsP42);
                                                        qftime2 = qfP42;
                                                        qtime3 = String(qsP43);
                                                        qftime3 = qfP43;
                                                        qtime4 = String(qsP44);
                                                        qftime4 = qfP44;
                                                        qtime5 = String(qsP45);
                                                        qftime5 = qfP45;
                                                        qtime6 = String(qsP46);
                                                        qftime6 = qfP46;
                                                        qtime7 = String(qsP47);
                                                        qftime7 = qfP47;
                                                        qtime8 = String(qsP48);
                                                        qftime8 = qfP48;
                                                        qqplayer = 4;
                                                        qqdatum = qDatum4;
                                                        NewHigh = 1;
                                                        SD.remove("hqu.txt");
                                                        hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                        if (hquickFil) {
                                                                hquickFil.print(qqplayer);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime1);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime2);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime3);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime4);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime5);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime6);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime7);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime8);
                                                                hquickFil.print(",");
                                                                hquickFil.println(qqdatum);
                                                                hquickFil.close();
                                                        }
                                                }
                                        }
                                }
                        }
                        qPrintScore = 1;
                        delay(2000);
                        currentPort=random(4);
                }
                delay(5000);
        }
        //Display highscore on lcd.
        if (NewHigh == 1) {
                lcd.clear();
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("New highscore to:");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(qqplayer);
                lcd.setCursor(0,3);
                lcd.print("Round 1: ");
                lcd.print(qtime1);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 2: ");
                lcd.print(qtime2);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 3: ");
                lcd.print(qtime3);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 4: ");
                lcd.print(qtime4);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 5: ");
                lcd.print(qtime5);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 6: ");
                lcd.print(qtime6);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 7: ");
                lcd.print(qtime7);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 8: ");
                lcd.print(qtime8);
                lcd.print(" sec");
                NewHigh = 0;
                delay(5000);
        }

        if (qPrintScore == 1) {
                //Make it possible to print.
                lcd.clear();
                printhigh = 1;
                while (printhigh = 1) {

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Press on select");
                        lcd.setCursor(0,2);
                        lcd.print("to print results");
                        lcd.setCursor(0,3);
                        lcd.print("or else on OK");
                        if (digitalRead(okButton)==HIGH) {
                                printhigh = 0;
                                qqdisplay = 0;
                                mainscreen = 0;
                                qPrintScore = 0;
                                displayCodeEntryScreen();
                                break;
                        }
                        if (digitalRead(selectButton)==HIGH) {
                                printhigh = 0;
                                qPrintScore = 0;
                                printQuick();
                                break;
                        }
                }
        }
        changeInfo = 1;
        resultsQuick = 1;
}
//Quickdraw wireless.
void doQuickDrawW()
{
        for (int ao=1; ao<=players; ao++)
        {
                if (ao == 1) {
                        delay(100);
                } else {
                        lcd.clear();

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Time for");
                        lcd.setCursor(0,2);
                        lcd.print("Player: ");
                        lcd.print(ao);
                        lcd.setCursor(0,3);
                        lcd.print("In 30 sec");
                        delay(30000);
                }

                randomSeed(millis());

                int firstTime=true;

                int currentPort=random(4);
                int maxRounds=8;

                Serial.write(0x0C); delay(5);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Quickdraw");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(ao);
                delay(2000);

                for (int i=0; i<maxRounds; i++)
                {
                        delay(5);

                        lcd.setCursor(0,2);
                        lcd.print("Round ");
                        lcd.print(i+1);
                        lcd.print(" of ");
                        lcd.print(maxRounds);
                        lcd.setCursor(0,3);
                        lcd.print("Ready ?       ");
                        delay(random(3000)+1000);
                        if (firstTime==true) { Serial.write(0xDF); firstTime=false; }// C note

                        time1=millis();
                        bool skicka3 = false;
                        bool skicka4 = false;
                        bool skicka5 = false;
                        bool skicka6 = false;
                        bool skicka7 = false;
                        bool skicka8 = false;
                        int backloop = 0;


                        if (currentPort == 0) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[0]);
                                radio.openReadingPipe(1,pipes[5]);
                                transmitterId = 16;
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                skicka1 = true;
                        } else if (currentPort == 1) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[2]);
                                radio.openReadingPipe(1,pipes[6]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 26;
                                skicka3 = true;


                        } else if (currentPort == 2) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[3]);
                                radio.openReadingPipe(1,pipes[7]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 36;
                                skicka5 = true;


                        } else if (currentPort == 3) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[4]);
                                radio.openReadingPipe(1,pipes[8]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 46;
                                skicka7 = true;


                        }

                        while (skicka1 == true)
                        {

                                if(radio.write(&transmitterId, sizeof(transmitterId))) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);

                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka2 = true;
                                                        skicka1 = false;
                                                        Serial.println(transmitterId);

                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }




                        }

                        while (skicka2 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData, sizeof(nockData));
                                if (radio.available(pipes[5])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[5])) {
                                                Serial.println("Start Lissening");
                                                radio.read(&senderId, sizeof(senderId));

                                        }
                                }  else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 17)    //Target 1
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka2 = false;
                                        skicka1 = false;
                                }

                        }

                        while (skicka3 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData3) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka4 = true;
                                                        skicka3 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka4 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData2, sizeof(nockData2));
                                if (radio.available(pipes[6])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[6])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Recived: ");
                                                Serial.println(senderId);
                                        }

                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 27)    //Target 2
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka4 = false;
                                        skicka3 = false;
                                }


                        }
                        while (skicka5 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData5) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka6 = true;
                                                        skicka5 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka6 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData3, sizeof(nockData3));
                                if (radio.available(pipes[7])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[7])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Mottagit: ");
                                                Serial.println(senderId);
                                        }

                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 37)    //Target 3
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka5 = false;
                                        skicka6 = false;
                                }


                        }
                        while (skicka7 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData7) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka8 = true;
                                                        skicka7 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka8 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData4, sizeof(nockData4));
                                if (radio.available(pipes[8])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[8])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Recived: ");
                                                Serial.println(senderId);
                                        }

                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 47)    //Target 4
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka7 = false;
                                        skicka8 = false;
                                }


                        }
                        time2=millis();
                        interval1=(time2-time1);
                        interval1=interval1/1000;
                        delay(5);
                        lcd.setCursor(0,3);
                        lcd.print(interval1,3);
                        lcd.print(" sec");
                        DateTime now = RTC.now();
                        timeNow = String(now.year()) + "-";
                        if (now.month() < 10) {
                                timeNow = timeNow + "0" + String(now.month());
                        } else {
                                timeNow = timeNow + String(now.month());
                        }
                        if (now.day() < 10) {
                                timeNow = timeNow + "-0" + String(now.day());
                        } else {
                                timeNow = timeNow + "-" + String(now.day());
                        }
                        if (now.hour() < 10) {
                                timeNow = timeNow + " 0" + String(now.hour());
                        } else {
                                timeNow = timeNow + " " + String(now.hour());
                        }
                        if (now.minute() < 10) {
                                timeNow = timeNow + ":0" + String(now.minute());
                        } else {
                                timeNow = timeNow + ":" + String(now.minute());
                        }
                        //Is there new higscore ?
                        if (i == 0) {
                                if (ao == 1) {
                                        qP11=interval1,3;
                                        qfP11=interval1,3;
                                        dtostrf(qfP11,4, 3, qsP11);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP21=interval1,3;
                                        qfP21=interval1,3;
                                        dtostrf(qfP21,4, 3, qsP21);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP31=interval1,3;
                                        qfP31=interval1,3;
                                        dtostrf(qfP31,4, 3, qsP31);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP41=interval1,3;
                                        qfP41=interval1,3;
                                        dtostrf(qfP41,4, 3, qsP41);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 1) {
                                if (ao == 1) {
                                        qP12=interval1,3;
                                        qfP12=interval1,3;
                                        dtostrf(qfP12,4, 3, qsP12);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP22=interval1,3;
                                        qfP22=interval1,3;
                                        dtostrf(qfP22,4, 3, qsP22);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP32=interval1,3;
                                        qfP32=interval1,3;
                                        dtostrf(qfP32,4, 3, qsP32);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP42=interval1,3;
                                        qfP42=interval1,3;
                                        dtostrf(qfP42,4, 3, qsP42);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 2) {
                                if (ao == 1) {
                                        qP13=interval1,3;
                                        qfP13=interval1,3;
                                        dtostrf(qfP13,4, 3, qsP13);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP23=interval1,3;
                                        qfP23=interval1,3;
                                        dtostrf(qfP23,4, 3, qsP23);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP33=interval1,3;
                                        qfP33=interval1,3;
                                        dtostrf(qfP33,4, 3, qsP33);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP43=interval1,3;
                                        qfP43=interval1,3;
                                        dtostrf(qfP43,4, 3, qsP43);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 3) {
                                if (ao == 1) {
                                        qP14=interval1,3;
                                        qfP14=interval1,3;
                                        dtostrf(qfP14,4, 3, qsP14);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP24=interval1,3;
                                        qfP24=interval1,3;
                                        dtostrf(qfP24,4, 3, qsP24);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP34=interval1,3;
                                        qfP34=interval1,3;
                                        dtostrf(qfP34,4, 3, qsP34);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP44=interval1,3;
                                        qfP44=interval1,3;
                                        dtostrf(qfP44,4, 3, qsP44);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 4) {
                                if (ao == 1) {
                                        qP15=interval1,3;
                                        qfP15=interval1,3;
                                        dtostrf(qfP15,4, 3, qsP15);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP25=interval1,3;
                                        qfP25=interval1,3;
                                        dtostrf(qfP25,4, 3, qsP25);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP35=interval1,3;
                                        qfP35=interval1,3;
                                        dtostrf(qfP35,4, 3, qsP35);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP45=interval1,3;
                                        qfP45=interval1,3;
                                        dtostrf(qfP45,4, 3, qsP45);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 5) {
                                if (ao == 1) {
                                        qP16=interval1,3;
                                        qfP16=interval1,3;
                                        dtostrf(qfP16,4, 3, qsP16);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP26=interval1,3;
                                        qfP26=interval1,3;
                                        dtostrf(qfP26,4, 3, qsP26);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP36=interval1,3;
                                        qfP36=interval1,3;
                                        dtostrf(qfP36,4, 3, qsP36);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP46=interval1,3;
                                        qfP46=interval1,3;
                                        dtostrf(qfP46,4, 3, qsP46);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 6) {
                                if (ao == 1) {
                                        qP17=interval1,3;
                                        qfP17=interval1,3;
                                        dtostrf(qfP17,4, 3, qsP17);
                                        qDatum1=timeNow;
                                } else if (ao == 2) {
                                        qP27=interval1,3;
                                        qfP27=interval1,3;
                                        dtostrf(qfP27,4, 3, qsP27);
                                        qDatum2=timeNow;
                                } else if (ao == 3) {
                                        qP37=interval1,3;
                                        qfP37=interval1,3;
                                        dtostrf(qfP37,4, 3, qsP37);
                                        qDatum3=timeNow;
                                } else if (ao == 4) {
                                        qP47=interval1,3;
                                        qfP47=interval1,3;
                                        dtostrf(qfP47,4, 3, qsP47);
                                        qDatum4=timeNow;
                                }
                        } else if (i == 7) {
                                if (ao == 1) {
                                        float scoreres;
                                        float highscores;
                                        qP18=interval1,3;
                                        qfP18=interval1,3;
                                        dtostrf(qfP18,4, 3, qsP18);
                                        qDatum1=timeNow;
                                        highscores = qftime1 + qftime2 + qftime3 + qftime4 + qftime5 + qftime6 + qftime7 + qftime8;
                                        scoreres = qfP11 + qfP12 + qfP13 + qfP14 + qfP15 + qfP16 + qfP17 + qfP18;
                                        if (qqplayer == 0) {
                                                qtime1 = String(qsP11);
                                                qftime1 = qfP11;
                                                qtime2 = String(qsP12);
                                                qftime2 = qfP12;
                                                qtime3 = String(qsP13);
                                                qftime3 = qfP13;
                                                qtime4 = String(qsP14);
                                                qftime4 = qfP14;
                                                qtime5 = String(qsP15);
                                                qftime5 = qfP15;
                                                qtime6 = String(qsP16);
                                                qftime6 = qfP16;
                                                qtime7 = String(qsP17);
                                                qftime7 = qfP17;
                                                qtime8 = String(qsP18);
                                                qftime8 = qfP18;
                                                qqplayer = 1;
                                                qqdatum = qDatum1;
                                                NewHigh = 1;
                                                SD.remove("hqu.txt");
                                                hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(qqplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime4);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime5);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime6);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime7);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime8);
                                                        hquickFil.print(",");
                                                        hquickFil.println(qqdatum);
                                                        hquickFil.close();
                                                }
                                        }else {
                                                if (scoreres < highscores) {
                                                        qtime1 = String(qsP11);
                                                        qftime1 = qfP11;
                                                        qtime2 = String(qsP12);
                                                        qftime2 = qfP12;
                                                        qtime3 = String(qsP13);
                                                        qftime3 = qfP13;
                                                        qtime4 = String(qsP14);
                                                        qftime4 = qfP14;
                                                        qtime5 = String(qsP15);
                                                        qftime5 = qfP15;
                                                        qtime6 = String(qsP16);
                                                        qftime6 = qfP16;
                                                        qtime7 = String(qsP17);
                                                        qftime7 = qfP17;
                                                        qtime8 = String(qsP18);
                                                        qftime8 = qfP18;
                                                        qqplayer = 1;
                                                        qqdatum = qDatum1;
                                                        NewHigh = 1;
                                                        SD.remove("hqu.txt");
                                                        hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                        if (hquickFil) {
                                                                hquickFil.print(qqplayer);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime1);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime2);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime3);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime4);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime5);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime6);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime7);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime8);
                                                                hquickFil.print(",");
                                                                hquickFil.println(qqdatum);
                                                                hquickFil.close();
                                                        }
                                                }
                                        }
                                } else if (ao == 2) {
                                        float scoreres;
                                        float highscores;
                                        qP28=interval1,3;
                                        qfP28=interval1,3;
                                        dtostrf(qfP28,4, 3, qsP28);
                                        qDatum2=timeNow;
                                        highscores = qftime1 + qftime2 + qftime3 + qftime4 + qftime5 + qftime6 + qftime7 + qftime8;
                                        scoreres = qfP21 + qfP22 + qfP23 + qfP24 + qfP25 + qfP26 + qfP27 + qfP28;
                                        if (qqplayer == 0) {
                                                qtime1 = String(qsP21);
                                                qftime1 = qfP21;
                                                qtime2 = String(qsP22);
                                                qftime2 = qfP22;
                                                qtime3 = String(qsP23);
                                                qftime3 = qfP23;
                                                qtime4 = String(qsP24);
                                                qftime4 = qfP24;
                                                qtime5 = String(qsP25);
                                                qftime5 = qfP25;
                                                qtime6 = String(qsP26);
                                                qftime6 = qfP26;
                                                qtime7 = String(qsP27);
                                                qftime7 = qfP27;
                                                qtime8 = String(qsP28);
                                                qftime8 = qfP28;
                                                qqplayer = 2;
                                                qqdatum = qDatum2;
                                                NewHigh = 1;
                                                SD.remove("hqu.txt");
                                                hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(qqplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime4);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime5);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime6);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime7);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime8);
                                                        hquickFil.print(",");
                                                        hquickFil.println(qqdatum);
                                                        hquickFil.close();
                                                }
                                        }else {
                                                if (scoreres < highscores) {
                                                        qtime1 = String(qsP21);
                                                        qftime1 = qfP21;
                                                        qtime2 = String(qsP22);
                                                        qftime2 = qfP22;
                                                        qtime3 = String(qsP23);
                                                        qftime3 = qfP23;
                                                        qtime4 = String(qsP24);
                                                        qftime4 = qfP24;
                                                        qtime5 = String(qsP25);
                                                        qftime5 = qfP25;
                                                        qtime6 = String(qsP26);
                                                        qftime6 = qfP26;
                                                        qtime7 = String(qsP27);
                                                        qftime7 = qfP27;
                                                        qtime8 = String(qsP28);
                                                        qftime8 = qfP28;
                                                        qqplayer = 2;
                                                        qqdatum = qDatum2;
                                                        NewHigh = 1;
                                                        SD.remove("hqu.txt");
                                                        hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                        if (hquickFil) {
                                                                hquickFil.print(qqplayer);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime1);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime2);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime3);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime4);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime5);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime6);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime7);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime8);
                                                                hquickFil.print(",");
                                                                hquickFil.println(qqdatum);
                                                                hquickFil.close();
                                                        }
                                                }
                                        }
                                } else if (ao == 3) {
                                        float scoreres;
                                        float highscores;
                                        qP38=interval1,3;
                                        qfP38=interval1,3;
                                        dtostrf(qfP38,4, 3, qsP38);
                                        qDatum3=timeNow;
                                        highscores = qftime1 + qftime2 + qftime3 + qftime4 + qftime5 + qftime6 + qftime7 + qftime8;
                                        scoreres = qfP31 + qfP32 + qfP33 + qfP34 + qfP35 + qfP36 + qfP37 + qfP38;
                                        if (qqplayer == 0) {
                                                qtime1 = String(qsP31);
                                                qftime1 = qfP31;
                                                qtime2 = String(qsP32);
                                                qftime2 = qfP32;
                                                qtime3 = String(qsP33);
                                                qftime3 = qfP33;
                                                qtime4 = String(qsP34);
                                                qftime4 = qfP34;
                                                qtime5 = String(qsP35);
                                                qftime5 = qfP35;
                                                qtime6 = String(qsP36);
                                                qftime6 = qfP36;
                                                qtime7 = String(qsP37);
                                                qftime7 = qfP37;
                                                qtime8 = String(qsP38);
                                                qftime8 = qfP38;
                                                qqplayer = 3;
                                                qqdatum = qDatum3;
                                                NewHigh = 1;
                                                SD.remove("hqu.txt");
                                                hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(qqplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime4);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime5);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime6);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime7);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime8);
                                                        hquickFil.print(",");
                                                        hquickFil.println(qqdatum);
                                                        hquickFil.close();
                                                }
                                        }else {
                                                if (scoreres < highscores) {
                                                        qtime1 = String(qsP31);
                                                        qftime1 = qfP31;
                                                        qtime2 = String(qsP32);
                                                        qftime2 = qfP32;
                                                        qtime3 = String(qsP33);
                                                        qftime3 = qfP33;
                                                        qtime4 = String(qsP34);
                                                        qftime4 = qfP34;
                                                        qtime5 = String(qsP35);
                                                        qftime5 = qfP35;
                                                        qtime6 = String(qsP36);
                                                        qftime6 = qfP36;
                                                        qtime7 = String(qsP37);
                                                        qftime7 = qfP37;
                                                        qtime8 = String(qsP38);
                                                        qftime8 = qfP38;
                                                        qqplayer = 3;
                                                        qqdatum = qDatum3;
                                                        NewHigh = 1;
                                                        SD.remove("hqu.txt");
                                                        hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                        if (hquickFil) {
                                                                hquickFil.print(qqplayer);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime1);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime2);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime3);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime4);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime5);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime6);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime7);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime8);
                                                                hquickFil.print(",");
                                                                hquickFil.println(qqdatum);
                                                                hquickFil.close();
                                                        }
                                                }
                                        }
                                } else if (ao == 4) {
                                        float scoreres;
                                        float highscores;
                                        qP48=interval1,3;
                                        qfP48=interval1,3;
                                        dtostrf(qfP48,4, 3, qsP48);
                                        qDatum4=timeNow;
                                        highscores = qftime1 + qftime2 + qftime3 + qftime4 + qftime5 + qftime6 + qftime7 + qftime8;
                                        scoreres = qfP41 + qfP42 + qfP43 + qfP44 + qfP45 + qfP46 + qfP47 + qfP48;
                                        if (qqplayer == 0) {
                                                qtime1 = String(qsP41);
                                                qftime1 = qfP41;
                                                qtime2 = String(qsP42);
                                                qftime2 = qfP42;
                                                qtime3 = String(qsP43);
                                                qftime3 = qfP43;
                                                qtime4 = String(qsP44);
                                                qftime4 = qfP44;
                                                qtime5 = String(qsP45);
                                                qftime5 = qfP45;
                                                qtime6 = String(qsP46);
                                                qftime6 = qfP46;
                                                qtime7 = String(qsP47);
                                                qftime7 = qfP47;
                                                qtime8 = String(qsP48);
                                                qftime8 = qfP48;
                                                qqplayer = 4;
                                                qqdatum = qDatum4;
                                                NewHigh = 1;
                                                SD.remove("hqu.txt");
                                                hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                if (hquickFil) {
                                                        hquickFil.print(qqplayer);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime1);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime2);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime3);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime4);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime5);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime6);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime7);
                                                        hquickFil.print(",");
                                                        hquickFil.print(qtime8);
                                                        hquickFil.print(",");
                                                        hquickFil.println(qqdatum);
                                                        hquickFil.close();
                                                }
                                        }else {
                                                if (scoreres < highscores) {
                                                        qtime1 = String(qsP41);
                                                        qftime1 = qfP41;
                                                        qtime2 = String(qsP42);
                                                        qftime2 = qfP42;
                                                        qtime3 = String(qsP43);
                                                        qftime3 = qfP43;
                                                        qtime4 = String(qsP44);
                                                        qftime4 = qfP44;
                                                        qtime5 = String(qsP45);
                                                        qftime5 = qfP45;
                                                        qtime6 = String(qsP46);
                                                        qftime6 = qfP46;
                                                        qtime7 = String(qsP47);
                                                        qftime7 = qfP47;
                                                        qtime8 = String(qsP48);
                                                        qftime8 = qfP48;
                                                        qqplayer = 4;
                                                        qqdatum = qDatum4;
                                                        NewHigh = 1;
                                                        SD.remove("hqu.txt");
                                                        hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                                        if (hquickFil) {
                                                                hquickFil.print(qqplayer);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime1);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime2);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime3);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime4);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime5);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime6);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime7);
                                                                hquickFil.print(",");
                                                                hquickFil.print(qtime8);
                                                                hquickFil.print(",");
                                                                hquickFil.println(qqdatum);
                                                                hquickFil.close();
                                                        }
                                                }
                                        }
                                }
                        }
                        qPrintScore = 1;
                        delay(2000);
                        currentPort=random(4);
                }
                delay(5000);
        }
        //Display hiscore on lcd.
        if (NewHigh == 1) {
                lcd.clear();
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("New highscore to:");
                lcd.setCursor(0,2);
                lcd.print("Player: ");
                lcd.print(qqplayer);
                lcd.setCursor(0,3);
                lcd.print("Round 1: ");
                lcd.print(qtime1);
                lcd.print(" sek");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 2: ");
                lcd.print(qtime2);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 3: ");
                lcd.print(qtime3);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 4: ");
                lcd.print(qtime4);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 5: ");
                lcd.print(qtime5);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 6: ");
                lcd.print(qtime6);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 7: ");
                lcd.print(qtime7);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,3);
                lcd.print("Round 8: ");
                lcd.print(qtime8);
                lcd.print(" sec");
                NewHigh = 0;
                delay(5000);
        }

        if (qPrintScore == 1) {
                //Make it possible to print.
                lcd.clear();
                printhigh = 1;
                while (printhigh = 1) {

                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Press on select");
                        lcd.setCursor(0,2);
                        lcd.print("to print results");
                        lcd.setCursor(0,3);
                        lcd.print("or else on OK");
                        if (digitalRead(okButton)==HIGH) {
                                printhigh = 0;
                                qqdisplay = 0;
                                mainscreen = 0;
                                qPrintScore = 0;
                                displayCodeEntryScreen();
                                break;
                        }
                        if (digitalRead(selectButton)==HIGH) {
                                printhigh = 0;
                                qPrintScore = 0;
                                printQuick();
                                break;
                        }
                }
        }
        changeInfo = 1;
        resultsQuick = 1;
}


//Swedish characters on lcd.
void createSweCaracters(){

        byte AwithRing[8] = {
                B00100,
                B01010,
                B01110,
                B00001,
                B01111,
                B10001,
                B01111,
        };

        byte AwithDots[8] = {
                B01010,
                B00000,
                B01110,
                B00001,
                B01111,
                B10001,
                B01111,
        };

        byte OwithDots[8] = {
                B01010,
                B00000,
                B01110,
                B10001,
                B10001,
                B10001,
                B01110,
        };

        byte CapitalAwithRing[8] = {
                B00100,
                B01010,
                B01110,
                B10001,
                B11111,
                B10001,
                B10001,
        };

        byte CapitalAwithDots[8] = {
                B01010,
                B00000,
                B01110,
                B10001,
                B11111,
                B10001,
                B10001,
        };

        byte CapitalOwithDots[8] = {
                B01010,
                B00000,
                B01110,
                B10001,
                B10001,
                B10001,
                B01110,
        };

        byte arrow_u[8] = {
                0b00000,
                0b11110,
                0b00110,
                0b01010,
                0b10010,
                0b00000,
                0b00000,
                0b00000
        };

        lcd.createChar(1, AwithRing);
        lcd.createChar(2, AwithDots);
        lcd.createChar(3, OwithDots);
        lcd.createChar(4, CapitalAwithRing);
        lcd.createChar(5, CapitalAwithDots);
        lcd.createChar(6, CapitalOwithDots);
        lcd.createChar(7, arrow_u);

}
//Display latest timed mode
void latestTime() {
        if (resultsTimed == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Timed mode");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: 1");
                lcd.setCursor(0,2);
                lcd.print("Hits: ");
                lcd.print(tPlayer1);
                lcd.setCursor(0,3);
                lcd.print(tDatum1);
                delay(7000);
                if (players == 2) {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 2");
                        lcd.setCursor(0,2);
                        lcd.print("Hits: ");
                        lcd.print(tPlayer2);
                        lcd.setCursor(0,3);
                        lcd.print(tDatum2);
                        delay(7000);
                }
                if (players == 3) {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 2");
                        lcd.setCursor(0,2);
                        lcd.print("Hits: ");
                        lcd.print(tPlayer2);
                        lcd.setCursor(0,3);
                        lcd.print(tDatum2);
                        delay(7000);
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 3");
                        lcd.setCursor(0,2);
                        lcd.print("Hits: ");
                        lcd.print(tPlayer3);
                        lcd.setCursor(0,3);
                        lcd.print(tDatum3);
                        delay(7000);
                }
                if (players == 4) {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 2");
                        lcd.setCursor(0,2);
                        lcd.print("Hits: ");
                        lcd.print(tPlayer2);
                        lcd.setCursor(0,3);
                        lcd.print(tDatum2);
                        delay(7000);
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 3");
                        lcd.setCursor(0,2);
                        lcd.print("Hits: ");
                        lcd.print(tPlayer3);
                        lcd.setCursor(0,3);
                        lcd.print(tDatum3);
                        delay(7000);
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 4");
                        lcd.setCursor(0,2);
                        lcd.print("Hits: ");
                        lcd.print(tPlayer4);
                        lcd.setCursor(0,3);
                        lcd.print(tDatum4);
                        delay(7000);
                }
        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Timed mode");
                lcd.setCursor(0,3);
                lcd.print("Not played!!");
                delay(4000);
        }
        latestRapid();
}
//Display latest rapid fire
void latestRapid() {
        if (resultsRapid == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Rapid Fire");
                if (rshooting == 0) {
                        lcd.setCursor(0,2);
                        lcd.print("6 Hits");
                } else if (rshooting == 1) {
                        lcd.setCursor(0,2);
                        lcd.print("10 Hits");
                } else if (rshooting == 2) {
                        lcd.setCursor(0,2);
                        lcd.print("20 Hits");
                } else if (rshooting == 3) {
                        lcd.setCursor(0,2);
                        lcd.print("32 Hits");
                }
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: 1");
                lcd.setCursor(0,2);
                lcd.print("Time: ");
                lcd.print(rsPlayer1);
                lcd.print(" sec");
                lcd.setCursor(0,3);
                lcd.print(rDatum1);
                delay(7000);
                if (players == 2) {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 2");
                        lcd.setCursor(0,2);
                        lcd.print("Time: ");
                        lcd.print(rsPlayer2);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(rDatum2);
                        delay(7000);
                }
                if (players == 3) {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 2");
                        lcd.setCursor(0,2);
                        lcd.print("Time: ");
                        lcd.print(rsPlayer2);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(rDatum2);
                        delay(7000);
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 3");
                        lcd.setCursor(0,2);
                        lcd.print("Time: ");
                        lcd.print(rsPlayer3);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(rDatum3);
                        delay(7000);
                }
                if (players == 4) {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 2");
                        lcd.setCursor(0,2);
                        lcd.print("Time: ");
                        lcd.print(rsPlayer2);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(rDatum2);
                        delay(7000);
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 3");
                        lcd.setCursor(0,2);
                        lcd.print("Time: ");
                        lcd.print(rsPlayer3);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(rDatum3);
                        delay(7000);
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 4");
                        lcd.setCursor(0,2);
                        lcd.print("Time: ");
                        lcd.print(rsPlayer4);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(rDatum4);
                        delay(7000);
                }
        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Rapid Fire");
                lcd.setCursor(0,3);
                lcd.print("Not played!!");
                delay(4000);
        }
        latestQuick();
}
//Display latest quick draw.
void latestQuick() {
        if (resultsQuick == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Quickdraw");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: 1");
                lcd.setCursor(0,2);
                lcd.print("Round 1: ");
                lcd.print(qfP11);
                lcd.print(" sec");
                lcd.setCursor(0,3);
                lcd.print(qDatum1);
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 2: ");
                lcd.print(qfP12);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 3: ");
                lcd.print(qfP13);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 4: ");
                lcd.print(qfP14);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 5: ");
                lcd.print(qfP15);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 6: ");
                lcd.print(qfP16);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 7: ");
                lcd.print(qfP17);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 8: ");
                lcd.print(qfP18);
                lcd.print(" sec");
                delay(7000);
                if (players == 2) {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 2");
                        lcd.setCursor(0,2);
                        lcd.print("Round 1: ");
                        lcd.print(qfP21);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(qDatum2);
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 2: ");
                        lcd.print(qfP22);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 3: ");
                        lcd.print(qfP23);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 4: ");
                        lcd.print(qfP24);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 5: ");
                        lcd.print(qfP25);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 6: ");
                        lcd.print(qfP26);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 7: ");
                        lcd.print(qfP27);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 8: ");
                        lcd.print(qfP28);
                        lcd.print(" sec");
                        delay(7000);
                }
                if (players == 3) {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 2");
                        lcd.setCursor(0,2);
                        lcd.print("Round 1: ");
                        lcd.print(qfP21);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(qDatum2);
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 2: ");
                        lcd.print(qfP22);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 3: ");
                        lcd.print(qfP23);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 4: ");
                        lcd.print(qfP24);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 5: ");
                        lcd.print(qfP25);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 6: ");
                        lcd.print(qfP26);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 7: ");
                        lcd.print(qfP27);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 8: ");
                        lcd.print(qfP28);
                        lcd.print(" sec");
                        delay(7000);
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 3");
                        lcd.setCursor(0,2);
                        lcd.print("Round 1: ");
                        lcd.print(qfP31);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(qDatum3);
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 2: ");
                        lcd.print(qfP32);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 3: ");
                        lcd.print(qfP33);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 4: ");
                        lcd.print(qfP34);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 5: ");
                        lcd.print(qfP35);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 6: ");
                        lcd.print(qfP36);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 7: ");
                        lcd.print(qfP37);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 8: ");
                        lcd.print(qfP38);
                        lcd.print(" sec");
                        delay(7000);
                }
                if (players == 4) {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 2");
                        lcd.setCursor(0,2);
                        lcd.print("Round 1: ");
                        lcd.print(qfP21);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(qDatum2);
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 2: ");
                        lcd.print(qfP22);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 3: ");
                        lcd.print(qfP23);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 4: ");
                        lcd.print(qfP24);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 5: ");
                        lcd.print(qfP25);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 6: ");
                        lcd.print(qfP26);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 7: ");
                        lcd.print(qfP27);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 8: ");
                        lcd.print(qfP28);
                        lcd.print(" sec");
                        delay(7000);
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 3");
                        lcd.setCursor(0,2);
                        lcd.print("Round 1: ");
                        lcd.print(qfP31);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(qDatum3);
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 2: ");
                        lcd.print(qfP32);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 3: ");
                        lcd.print(qfP33);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 4: ");
                        lcd.print(qfP34);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 5: ");
                        lcd.print(qfP35);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 6: ");
                        lcd.print(qfP36);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 7: ");
                        lcd.print(qfP37);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 8: ");
                        lcd.print(qfP38);
                        lcd.print(" sec");
                        delay(7000);
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,1);
                        lcd.print("Player: 4");
                        lcd.setCursor(0,2);
                        lcd.print("Round 1: ");
                        lcd.print(qfP41);
                        lcd.print(" sec");
                        lcd.setCursor(0,3);
                        lcd.print(qDatum4);
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 2: ");
                        lcd.print(qfP42);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 3: ");
                        lcd.print(qfP43);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 4: ");
                        lcd.print(qfP44);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 5: ");
                        lcd.print(qfP45);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 6: ");
                        lcd.print(qfP46);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 7: ");
                        lcd.print(qfP47);
                        lcd.print(" sec");
                        delay(2000);
                        lcd.setCursor(0,2);
                        lcd.print("Round 8: ");
                        lcd.print(qfP48);
                        lcd.print(" sec");
                        delay(7000);
                }
        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Quickdraw");
                lcd.setCursor(0,3);
                lcd.print("Not played!!");
                delay(4000);
        }
        playMode = 1;
        displayCodeEntryScreen();
}
//Hiscore on timed mode
void timeHigh() {
        int ind1;
        int ind2;
        int ind3;
        int shotint;

        String pquick;
        String shot;
        String datum;
        changeInfo = 0;
        hquickFil = SD.open("htime.txt");
        if (hquickFil) {
                Serial.println("htime.txt:");
                String l_line;
                while (hquickFil.available()) {
                        l_line = hquickFil.readStringUntil('\n');
                        l_line.trim();
                        if (l_line != "") {
                                int l_start_posn = 0;
                                ind1 = l_line.indexOf(',');
                                pquick = l_line.substring(0, ind1);
                                ind2 = l_line.indexOf(',', ind1+1 );
                                shot = l_line.substring(ind1+1, ind2);
                                ind3 = l_line.indexOf(',', ind2+1 );
                                datum = l_line.substring(ind2+1, ind3);
                                qplayer = pquick.toInt();
                                qshot = shot.toInt();
                                shotint = shot.toInt();
                                qdatum = datum;
                                if (qshot == 0) {
                                        qdisplay = 0;
                                } else {
                                        qdisplay = 1;
                                }

                        }
                }
                hquickFil.close();
        }
        hquickFil = SD.open("htime2.txt");
        if (hquickFil) {
                Serial.println("htime2.txt:");
                String l_line;
                while (hquickFil.available()) {
                        l_line = hquickFil.readStringUntil('\n');
                        l_line.trim();
                        if (l_line != "") {
                                int l_start_posn = 0;
                                ind1 = l_line.indexOf(',');
                                pquick = l_line.substring(0, ind1);
                                ind2 = l_line.indexOf(',', ind1+1 );
                                shot = l_line.substring(ind1+1, ind2);
                                ind3 = l_line.indexOf(',', ind2+1 );
                                datum = l_line.substring(ind2+1, ind3);
                                qplayer2 = pquick.toInt();
                                qshot2 = shot.toInt();
                                shotint = shot.toInt();
                                qdatum2 = datum;
                                if (qshot2 == 0) {
                                        qdisplay2 = 0;
                                } else {
                                        qdisplay2 = 1;
                                }

                        }

                }
                hquickFil.close();
        }
        hquickFil = SD.open("htime3.txt");
        if (hquickFil) {
                Serial.println("htime3.txt:");
                String l_line;
                while (hquickFil.available()) {
                        l_line = hquickFil.readStringUntil('\n');
                        l_line.trim();
                        if (l_line != "") {
                                int l_start_posn = 0;
                                ind1 = l_line.indexOf(',');
                                pquick = l_line.substring(0, ind1);
                                ind2 = l_line.indexOf(',', ind1+1 );
                                shot = l_line.substring(ind1+1, ind2);
                                ind3 = l_line.indexOf(',', ind2+1 );
                                datum = l_line.substring(ind2+1, ind3);
                                qplayer3 = pquick.toInt();
                                qshot3 = shot.toInt();
                                shotint = shot.toInt();
                                qdatum3 = datum;
                                if (qshot3 == 0) {
                                        qdisplay3 = 0;
                                } else {
                                        qdisplay3 = 1;
                                }

                        }

                }
                hquickFil.close();
        }
        if (qdisplay == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Time mode");
                lcd.setCursor(0,3);
                lcd.print("10 sec");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: ");
                lcd.print(qplayer);
                lcd.setCursor(0,2);
                lcd.print("Hits:");
                lcd.print(qshot);
                lcd.setCursor(0,3);
                lcd.print(qdatum);
                delay(7000);

        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Best on Time mode");
                lcd.setCursor(0,2);
                lcd.print("10 sec");
                lcd.setCursor(0,3);
                lcd.print("No results");
                delay(4000);
        }

        if (qdisplay2 == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Time mode");
                lcd.setCursor(0,3);
                lcd.print("20 sec");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: ");
                lcd.print(qplayer2);
                lcd.setCursor(0,2);
                lcd.print("Hits:");
                lcd.print(qshot2);
                lcd.setCursor(0,3);
                lcd.print(qdatum2);
                delay(7000);
        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Best on Time mode");
                lcd.setCursor(0,2);
                lcd.print("20 sec");
                lcd.setCursor(0,3);
                lcd.print("No results");
                delay(4000);
        }

        if (qdisplay3 == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Time mode");
                lcd.setCursor(0,3);
                lcd.print("30 sec");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: ");
                lcd.print(qplayer3);
                lcd.setCursor(0,2);
                lcd.print("Hits:");
                lcd.print(qshot3);
                lcd.setCursor(0,3);
                lcd.print(qdatum3);
                delay(7000);
        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Best on Time mode");
                lcd.setCursor(0,2);
                lcd.print("30 sec");
                lcd.setCursor(0,3);
                lcd.print("No results");
                delay(4000);
        }

        rapidHigh();
}
//Display hiscore on rapid fire.
void rapidHigh() {
        int ind1;
        int ind2;
        int ind3;
        int shotint;

        String pquick;
        String shot;
        String datum;
        hquickFil = SD.open("hrap.txt");
        if (hquickFil) {
                Serial.println("hrap.txt:");
                String l_line;
                while (hquickFil.available()) {
                        l_line = hquickFil.readStringUntil('\n');
                        l_line.trim();
                        if (l_line != "") {
                                int l_start_posn = 0;
                                ind1 = l_line.indexOf(',');
                                pquick = l_line.substring(0, ind1);
                                ind2 = l_line.indexOf(',', ind1+1 );
                                shot = l_line.substring(ind1+1, ind2);
                                ind3 = l_line.indexOf(',', ind2+1 );
                                datum = l_line.substring(ind2+1, ind3);
                                rplayer = pquick.toInt();
                                rtime = shot;
                                rdatum = datum;
                                if (rplayer == 0) {
                                        rdisplay = 0;
                                } else {
                                        rdisplay = 1;
                                }

                        }

                }
                hquickFil.close();
        }
        hquickFil = SD.open("hrap1.txt");
        if (hquickFil) {
                Serial.println("hrap1.txt:");
                String l_line;
                while (hquickFil.available()) {
                        l_line = hquickFil.readStringUntil('\n');
                        l_line.trim();
                        if (l_line != "") {
                                int l_start_posn = 0;
                                ind1 = l_line.indexOf(',');
                                pquick = l_line.substring(0, ind1);
                                ind2 = l_line.indexOf(',', ind1+1 );
                                shot = l_line.substring(ind1+1, ind2);
                                ind3 = l_line.indexOf(',', ind2+1 );
                                datum = l_line.substring(ind2+1, ind3);
                                rPlayer_1 = pquick.toInt();
                                rtime_1 = shot;
                                rdatum_1 = datum;
                                if (rPlayer_1 == 0) {
                                        rdisplay_1 = 0;
                                } else {
                                        rdisplay_1 = 1;
                                }

                        }

                }
                hquickFil.close();
        }
        hquickFil = SD.open("hrap2.txt");
        if (hquickFil) {
                Serial.println("hrap2.txt:");
                String l_line;
                while (hquickFil.available()) {
                        l_line = hquickFil.readStringUntil('\n');
                        l_line.trim();
                        if (l_line != "") {
                                int l_start_posn = 0;
                                ind1 = l_line.indexOf(',');
                                pquick = l_line.substring(0, ind1);
                                ind2 = l_line.indexOf(',', ind1+1 );
                                shot = l_line.substring(ind1+1, ind2);
                                ind3 = l_line.indexOf(',', ind2+1 );
                                datum = l_line.substring(ind2+1, ind3);
                                rPlayer_2 = pquick.toInt();
                                rtime_2 = shot;
                                rdatum_2 = datum;
                                if (rPlayer_2 == 0) {
                                        rdisplay_2 = 0;
                                } else {
                                        rdisplay_2 = 1;
                                }

                        }

                }
                hquickFil.close();
        }
        hquickFil = SD.open("hrap3.txt");
        if (hquickFil) {
                Serial.println("hrap3.txt:");
                String l_line;
                while (hquickFil.available()) {
                        l_line = hquickFil.readStringUntil('\n');
                        l_line.trim();
                        if (l_line != "") {
                                int l_start_posn = 0;
                                ind1 = l_line.indexOf(',');
                                pquick = l_line.substring(0, ind1);
                                ind2 = l_line.indexOf(',', ind1+1 );
                                shot = l_line.substring(ind1+1, ind2);
                                ind3 = l_line.indexOf(',', ind2+1 );
                                datum = l_line.substring(ind2+1, ind3);
                                rPlayer_3 = pquick.toInt();
                                rtime_3 = shot;
                                rdatum_3 = datum;
                                if (rPlayer_3 == 0) {
                                        rdisplay_3 = 0;
                                } else {
                                        rdisplay_3 = 1;
                                }

                        }

                }
                hquickFil.close();
        }
        if (rdisplay == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Rapid Fire");
                lcd.setCursor(0,3);
                lcd.print("6 Hits");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: ");
                lcd.print(rplayer);
                lcd.setCursor(0,2);
                lcd.print("Time: ");
                lcd.print(rtime);
                lcd.print(" sec");
                lcd.setCursor(0,3);
                lcd.print(rdatum);
                delay(7000);
                //quickHigh();
        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Rapid Fire");
                lcd.setCursor(0,3);
                lcd.print("6 Hits");
                delay(4000);
                lcd.setCursor(0,3);
                lcd.print("No results");
                delay(4000);
                //quickHigh();
        }

        if (rdisplay_1 == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Rapid Fire");
                lcd.setCursor(0,3);
                lcd.print("10 Hits");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: ");
                lcd.print(rPlayer_1);
                lcd.setCursor(0,2);
                lcd.print("Time: ");
                lcd.print(rtime_1);
                lcd.print(" sec");
                lcd.setCursor(0,3);
                lcd.print(rdatum_1);
                delay(7000);
                //quickHigh();
        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Rapid Fire");
                lcd.setCursor(0,3);
                lcd.print("10 Hits");
                delay(4000);
                lcd.setCursor(0,3);
                lcd.print("No results");
                delay(4000);
                //quickHigh();
        }
        if (rdisplay_2 == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Rapid Fire");
                lcd.setCursor(0,3);
                lcd.print("20 Hits");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: ");
                lcd.print(rPlayer_2);
                lcd.setCursor(0,2);
                lcd.print("Time: ");
                lcd.print(rtime_2);
                lcd.print(" sec");
                lcd.setCursor(0,3);
                lcd.print(rdatum_2);
                delay(7000);
                //quickHigh();
        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Rapid Fire");
                lcd.setCursor(0,3);
                lcd.print("20 Hits");
                delay(4000);
                lcd.setCursor(0,3);
                lcd.print("No results");
                delay(4000);
                //quickHigh();
        }
        if (rdisplay_3 == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Rapid Fire");
                lcd.setCursor(0,3);
                lcd.print("32 Hits");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: ");
                lcd.print(rPlayer_3);
                lcd.setCursor(0,2);
                lcd.print("Time: ");
                lcd.print(rtime_3);
                lcd.print(" sec");
                lcd.setCursor(0,3);
                lcd.print(rdatum_3);
                delay(7000);
                quickHigh();
        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Rapid Fire");
                lcd.setCursor(0,3);
                lcd.print("32 Hits");
                delay(4000);
                lcd.setCursor(0,3);
                lcd.print("No results");
                delay(4000);
                quickHigh();
        }
}
//Display highscore on Quickdraw.
void quickHigh() {
        int ind1;
        int ind2;
        int ind3;
        int ind4;
        int ind5;
        int ind6;
        int ind7;
        int ind8;
        int ind9;
        int ind10;
        int shotint;

        String pquick;
        String tid1;
        String tid2;
        String tid3;
        String tid4;
        String tid5;
        String tid6;
        String tid7;
        String tid8;
        String shot;
        String datum;
        hquickFil = SD.open("hqu.txt");
        if (hquickFil) {
                Serial.println("hqu.txt:");
                String l_line;
                while (hquickFil.available()) {
                        l_line = hquickFil.readStringUntil('\n');
                        l_line.trim();
                        if (l_line != "") {
                                int l_start_posn = 0;
                                ind1 = l_line.indexOf(',');
                                pquick = l_line.substring(0, ind1);
                                ind2 = l_line.indexOf(',', ind1+1 );
                                tid1 = l_line.substring(ind1+1, ind2);
                                ind3 = l_line.indexOf(',', ind2+1 );
                                tid2 = l_line.substring(ind2+1, ind3);
                                ind4 = l_line.indexOf(',', ind3+1 );
                                tid3 = l_line.substring(ind3+1, ind4);
                                ind5 = l_line.indexOf(',', ind4+1 );
                                tid4 = l_line.substring(ind4+1, ind5);
                                ind6 = l_line.indexOf(',', ind5+1 );
                                tid5 = l_line.substring(ind5+1, ind6);
                                ind7 = l_line.indexOf(',', ind6+1 );
                                tid6 = l_line.substring(ind6+1, ind7);
                                ind8 = l_line.indexOf(',', ind7+1 );
                                tid7 = l_line.substring(ind7+1, ind8);
                                ind9 = l_line.indexOf(',', ind8+1 );
                                tid8 = l_line.substring(ind8+1, ind9);
                                ind10 = l_line.indexOf(',', ind9+1 );
                                datum = l_line.substring(ind9+1, ind10);
                                qqplayer = pquick.toInt();
                                qtime1 = tid1;
                                qftime1 = tid1.toFloat();
                                qtime2 = tid2;
                                qftime2 = tid2.toFloat();
                                qtime3 = tid3;
                                qftime3 = tid3.toFloat();
                                qtime4 = tid4;
                                qftime4 = tid4.toFloat();
                                qtime5 = tid5;
                                qftime5 = tid5.toFloat();
                                qtime6 = tid6;
                                qftime6 = tid6.toFloat();
                                qtime7 = tid7;
                                qftime7 = tid7.toFloat();
                                qtime8 = tid8;
                                qftime8 = tid8.toFloat();
                                qqdatum = datum;
                                if (qqplayer == 0) {
                                        qqdisplay = 0;
                                } else {
                                        qqdisplay = 1;
                                }

                        }

                }
                hquickFil.close();
        }
        if (qqdisplay == 1) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Quick Draw");
                delay(4000);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Player: ");
                lcd.print(qqplayer);
                lcd.setCursor(0,2);
                lcd.print("Round 1: ");
                lcd.print(qtime1);
                lcd.print(" sec");
                lcd.setCursor(0,3);
                lcd.print(qqdatum);
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 2: ");
                lcd.print(qtime2);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 3: ");
                lcd.print(qtime3);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 4: ");
                lcd.print(qtime4);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 5: ");
                lcd.print(qtime5);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 6: ");
                lcd.print(qtime6);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 7: ");
                lcd.print(qtime7);
                lcd.print(" sec");
                delay(2000);
                lcd.setCursor(0,2);
                lcd.print("Round 8: ");
                lcd.print(qtime8);
                lcd.print(" sec");
                delay(7000);

        } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Best on Quick Draw");
                lcd.setCursor(0,3);
                lcd.print("No results");
                delay(4000);

                changeInfo = 1;
        }
        //Make them print highscore.
        lcd.clear();
        printhigh = 1;
        while (printhigh = 1) {

                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,1);
                lcd.print("Press on select");
                lcd.setCursor(0,2);
                lcd.print("to print results");
                lcd.setCursor(0,3);
                lcd.print("or else on OK");
                if (digitalRead(okButton)==HIGH) {
                        printhigh = 0;
                        mainscreen = 0;
                        displayCodeEntryScreen();
                        changeInfo = 1;
                        break;
                }
                if (digitalRead(selectButton)==HIGH) {
                        printhigh = 0;
                        printHigh();
                        break;
                }
        }
}

void printRapid() {
//Printing for Rapid Fire.
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,2);
        lcd.print("Printing...");
        DateTime now = RTC.now();
        timeNow = String(now.year()) + "-";
        if (now.month() < 10) {
                timeNow = timeNow + "0" + String(now.month());
        } else {
                timeNow = timeNow + String(now.month());
        }
        if (now.day() < 10) {
                timeNow = timeNow + "-0" + String(now.day());
        } else {
                timeNow = timeNow + "-" + String(now.day());
        }
        if (now.hour() < 10) {
                timeNow = timeNow + " 0" + String(now.hour());
        } else {
                timeNow = timeNow + " " + String(now.hour());
        }
        if (now.minute() < 10) {
                timeNow = timeNow + ":0" + String(now.minute());
        } else {
                timeNow = timeNow + ":" + String(now.minute());
        }
        printer.printBitmap(gun2_width, gun2_height, gun2_data);
        printer.justify('C');
        printer.setSize('L');
        printer.println(F("AO Shooting Game"));
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("v3.12 Created by Andreas Olsson"));
        printer.println(F("--------------"));
        printer.justify('C');
        printer.setSize('L');
        printer.println(F("Rapid Fire"));
        if (rshooting == 0) {
                printer.println(F("6 Hits"));
        } else if (rshooting == 1) {
                printer.println(F("10 Hits"));
        } else if (rshooting == 2) {
                printer.println(F("20 Hits"));
        } else if (rshooting == 3) {
                printer.println(F("32 Hits"));
        }
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("--------------"));
        printer.justify('L');
        printer.setSize('M');
        printer.println(F("Player: 1"));
        printer.print(F("Time: "));
        printer.print(rsqPlayer1);
        printer.println(F(" sec"));
        printer.print(F("Date: "));
        printer.println(rDatum1);
        if (players == 2) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 2"));
                printer.print(F("Tid: "));
                printer.print(rsqPlayer2);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(rDatum2);
        } else if (players == 3) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 2"));
                printer.print(F("Time: "));
                printer.print(rsqPlayer2);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(rDatum2);
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 3"));
                printer.print(F("Time: "));
                printer.print(rsqPlayer3);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(rDatum3);
        } else if (players == 4) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 2"));
                printer.print(F("Time: "));
                printer.print(rsqPlayer2);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(rDatum2);
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 3"));
                printer.print(F("Time: "));
                printer.print(rsqPlayer3);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(rDatum3);
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 4"));
                printer.print(F("Time: "));
                printer.print(rsqPlayer4);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(rDatum4);
        }

        printer.justify('C');
        printer.setSize('S');
        printer.println(F("--------------"));
        printer.print(F("Printed: "));
        printer.println(timeNow);

        printer.feed(3);

        printer.sleep();
        delay(3000L);
        printer.wake();
        printer.setDefault();
        mainscreen = 0;
        displayCodeEntryScreen();
        changeInfo = 1;
}

void printTimed() {
//Printing for timed mode.
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,2);
        lcd.print("Printing...");
        DateTime now = RTC.now();
        timeNow = String(now.year()) + "-";
        if (now.month() < 10) {
                timeNow = timeNow + "0" + String(now.month());
        } else {
                timeNow = timeNow + String(now.month());
        }
        if (now.day() < 10) {
                timeNow = timeNow + "-0" + String(now.day());
        } else {
                timeNow = timeNow + "-" + String(now.day());
        }
        if (now.hour() < 10) {
                timeNow = timeNow + " 0" + String(now.hour());
        } else {
                timeNow = timeNow + " " + String(now.hour());
        }
        if (now.minute() < 10) {
                timeNow = timeNow + ":0" + String(now.minute());
        } else {
                timeNow = timeNow + ":" + String(now.minute());
        }
        printer.printBitmap(gun2_width, gun2_height, gun2_data);
        printer.justify('C');
        printer.setSize('L');
        printer.println(F("AO Shooting Game"));
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("v3.12 Created by Andreas Olsson"));
        printer.println(F("--------------"));
        printer.justify('C');
        printer.setSize('L');
        printer.print(F("Timed mode"));
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("--------------"));
        printer.justify('L');
        printer.setSize('M');
        printer.println(F("Player: 1"));
        printer.print(F("Hits: "));
        printer.print(tPlayer1);
        printer.print(F("Date: "));
        printer.println(tDatum1);
        if (players == 2) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 2"));
                printer.print(F("Hits: "));
                printer.print(tPlayer2);
                printer.print(F("Date: "));
                printer.println(tDatum2);
        } else if (players == 3) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 2"));
                printer.print(F("Hits: "));
                printer.print(tPlayer2);
                printer.print(F("Date: "));
                printer.println(tDatum2);
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 3"));
                printer.print(F("Hits: "));
                printer.print(tPlayer3);
                printer.println(F(" st"));
                printer.print(F("Date: "));
                printer.println(tDatum3);
        } else if (players == 4) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 2"));
                printer.print(F("Hits: "));
                printer.print(tPlayer2);
                printer.print(F("Date: "));
                printer.println(tDatum2);
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 3"));
                printer.print(F("Hits: "));
                printer.print(tPlayer3);
                printer.print(F("Date: "));
                printer.println(tDatum3);
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 4"));
                printer.print(F("Hits: "));
                printer.print(tPlayer4);
                printer.print(F("Date: "));
                printer.println(tDatum4);
        }
        if (timeSel == 1) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Played: 10 sec"));
        } else if (timeSel == 2) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Played: 20 sec"));
        } if (timeSel == 3) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Played: 30 sec"));
        }
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("--------------"));
        printer.print(F("Printed: "));
        printer.println(timeNow);

        printer.feed(3);

        printer.sleep();
        delay(3000L);
        printer.wake();
        printer.setDefault();
        mainscreen = 0;
        displayCodeEntryScreen();
        changeInfo = 1;
}

void printQuick() {
//Printing for quickdraw.
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,2);
        lcd.print("Printing...");
        DateTime now = RTC.now();
        timeNow = String(now.year()) + "-";
        if (now.month() < 10) {
                timeNow = timeNow + "0" + String(now.month());
        } else {
                timeNow = timeNow + String(now.month());
        }
        if (now.day() < 10) {
                timeNow = timeNow + "-0" + String(now.day());
        } else {
                timeNow = timeNow + "-" + String(now.day());
        }
        if (now.hour() < 10) {
                timeNow = timeNow + " 0" + String(now.hour());
        } else {
                timeNow = timeNow + " " + String(now.hour());
        }
        if (now.minute() < 10) {
                timeNow = timeNow + ":0" + String(now.minute());
        } else {
                timeNow = timeNow + ":" + String(now.minute());
        }
        printer.printBitmap(gun2_width, gun2_height, gun2_data);
        printer.justify('C');
        printer.setSize('L');
        printer.println(F("AO Shooting Game"));
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("v3.12 Created by Andreas Olsson"));
        printer.println(F("--------------"));
        printer.justify('C');
        printer.setSize('L');
        printer.println(F("Quickdraw"));
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("--------------"));
        printer.justify('L');
        printer.setSize('M');
        printer.println(F("Player: 1"));
        printer.print(F("Round 1: "));
        printer.print(qsP11);
        printer.println(F(" sec"));
        printer.print(F("Round 2: "));
        printer.print(qsP12);
        printer.println(F(" sec"));
        printer.print(F("Round 3: "));
        printer.print(qsP13);
        printer.println(F(" sec"));
        printer.print(F("Round 4: "));
        printer.print(qsP14);
        printer.println(F(" sec"));
        printer.print(F("Round 5: "));
        printer.print(qsP15);
        printer.println(F(" sec"));
        printer.print(F("Round 6: "));
        printer.print(qsP16);
        printer.println(F(" sec"));
        printer.print(F("Round 7: "));
        printer.print(qsP17);
        printer.println(F(" sec"));
        printer.print(F("Round 8: "));
        printer.print(qsP18);
        printer.println(F(" sec"));
        printer.print(F("Date: "));
        printer.println(qDatum1);
        if (players == 2) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 2"));
                printer.print(F("Round 1: "));
                printer.print(qsP21);
                printer.println(F(" sec"));
                printer.print(F("Round 2: "));
                printer.print(qsP22);
                printer.println(F(" sec"));
                printer.print(F("Round 3: "));
                printer.print(qsP23);
                printer.println(F(" sec"));
                printer.print(F("Round 4: "));
                printer.print(qsP24);
                printer.println(F(" sec"));
                printer.print(F("Round 5: "));
                printer.print(qsP25);
                printer.println(F(" sec"));
                printer.print(F("Round 6: "));
                printer.print(qsP26);
                printer.println(F(" sec"));
                printer.print(F("Round 7: "));
                printer.print(qsP27);
                printer.println(F(" sec"));
                printer.print(F("Round 8: "));
                printer.print(qsP28);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(qDatum2);
        } else if (players == 3) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 2"));
                printer.print(F("Round 1: "));
                printer.print(qsP21);
                printer.println(F(" sec"));
                printer.print(F("Round 2: "));
                printer.print(qsP22);
                printer.println(F(" sec"));
                printer.print(F("Round 3: "));
                printer.print(qsP23);
                printer.println(F(" sec"));
                printer.print(F("Round 4: "));
                printer.print(qsP24);
                printer.println(F(" sec"));
                printer.print(F("Round 5: "));
                printer.print(qsP25);
                printer.println(F(" sec"));
                printer.print(F("Round 6: "));
                printer.print(qsP26);
                printer.println(F(" sec"));
                printer.print(F("Round 7: "));
                printer.print(qsP27);
                printer.println(F(" sec"));
                printer.print(F("Round 8: "));
                printer.print(qsP28);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(qDatum2);
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 3"));
                printer.print(F("Round 1: "));
                printer.print(qsP31);
                printer.println(F(" sec"));
                printer.print(F("Round 2: "));
                printer.print(qsP32);
                printer.println(F(" sec"));
                printer.print(F("Round 3: "));
                printer.print(qsP33);
                printer.println(F(" sec"));
                printer.print(F("Round 4: "));
                printer.print(qsP34);
                printer.println(F(" sec"));
                printer.print(F("Round 5: "));
                printer.print(qsP35);
                printer.println(F(" sec"));
                printer.print(F("Round 6: "));
                printer.print(qsP36);
                printer.println(F(" sec"));
                printer.print(F("Round 7: "));
                printer.print(qsP37);
                printer.println(F(" sec"));
                printer.print(F("Round 8: "));
                printer.print(qsP38);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(qDatum3);
        } else if (players == 4) {
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 2"));
                printer.print(F("Round 1: "));
                printer.print(qsP21);
                printer.println(F(" sec"));
                printer.print(F("Round 2: "));
                printer.print(qsP22);
                printer.println(F(" sec"));
                printer.print(F("Round 3: "));
                printer.print(qsP23);
                printer.println(F(" sec"));
                printer.print(F("Round 4: "));
                printer.print(qsP24);
                printer.println(F(" sec"));
                printer.print(F("Round 5: "));
                printer.print(qsP25);
                printer.println(F(" sec"));
                printer.print(F("Round 6: "));
                printer.print(qsP26);
                printer.println(F(" sec"));
                printer.print(F("Round 7: "));
                printer.print(qsP27);
                printer.println(F(" sec"));
                printer.print(F("Round 8: "));
                printer.print(qsP28);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(qDatum2);
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 3"));
                printer.print(F("Round 1: "));
                printer.print(qsP31);
                printer.println(F(" sec"));
                printer.print(F("Round 2: "));
                printer.print(qsP32);
                printer.println(F(" sec"));
                printer.print(F("Round 3: "));
                printer.print(qsP33);
                printer.println(F(" sec"));
                printer.print(F("Round 4: "));
                printer.print(qsP34);
                printer.println(F(" sec"));
                printer.print(F("Round 5: "));
                printer.print(qP35);
                printer.println(F(" sec"));
                printer.print(F("Round 6: "));
                printer.print(qsP36);
                printer.println(F(" sec"));
                printer.print(F("Round 7: "));
                printer.print(qsP37);
                printer.println(F(" sec"));
                printer.print(F("Round 8: "));
                printer.print(qsP38);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(qDatum3);
                printer.justify('C');
                printer.setSize('S');
                printer.println(F("--------------"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("Player: 4"));
                printer.print(F("Round 1: "));
                printer.print(qsP41);
                printer.println(F(" sec"));
                printer.print(F("Round 2: "));
                printer.print(qsP42);
                printer.println(F(" sec"));
                printer.print(F("Round 3: "));
                printer.print(qsP43);
                printer.println(F(" sec"));
                printer.print(F("Round 4: "));
                printer.print(qsP44);
                printer.println(F(" sec"));
                printer.print(F("Round 5: "));
                printer.print(qsP45);
                printer.println(F(" sec"));
                printer.print(F("Round 6: "));
                printer.print(qsP46);
                printer.println(F(" sec"));
                printer.print(F("Round 7: "));
                printer.print(qsP47);
                printer.println(F(" sec"));
                printer.print(F("Round 8: "));
                printer.print(qsP48);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(qDatum4);
        }

        printer.justify('C');
        printer.setSize('S');
        printer.println(F("--------------"));
        printer.print(F("Printed: "));
        printer.println(timeNow);

        printer.feed(3);

        printer.sleep();
        delay(3000L);
        printer.wake();
        printer.setDefault();
        mainscreen = 0;
        displayCodeEntryScreen();
        changeInfo = 1;
}

void printHigh() {
        //Printing hiscore.
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,2);
        lcd.print("Printing...");
        DateTime now = RTC.now();
        timeNow = String(now.year()) + "-";
        if (now.month() < 10) {
                timeNow = timeNow + "0" + String(now.month());
        } else {
                timeNow = timeNow + String(now.month());
        }
        if (now.day() < 10) {
                timeNow = timeNow + "-0" + String(now.day());
        } else {
                timeNow = timeNow + "-" + String(now.day());
        }
        if (now.hour() < 10) {
                timeNow = timeNow + " 0" + String(now.hour());
        } else {
                timeNow = timeNow + " " + String(now.hour());
        }
        if (now.minute() < 10) {
                timeNow = timeNow + ":0" + String(now.minute());
        } else {
                timeNow = timeNow + ":" + String(now.minute());
        }
        printer.printBitmap(gun2_width, gun2_height, gun2_data);
        printer.justify('C');
        printer.setSize('L');
        printer.println(F("AO Shooting Game"));
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("v3.12 Created by Andreas Olsson"));
        printer.println(F("--------------"));
        printer.justify('C');
        printer.setSize('L');
        printer.println(F("Highscore"));
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("--------------"));
        printer.justify('C');
        printer.setSize('M');
        printer.print(F("Timed mode"));
        if (qdisplay == 1) {
                printer.justify('L');
                printer.setSize('M');
                printer.print(F("Player: "));
                printer.println(qplayer);
                printer.print(F("Hits: "));
                printer.print(qshot);
                printer.print(F("Date: "));
                printer.println(qdatum);
                printer.println(F("Played 10 sec"));
        } else {
                printer.justify('L');
                printer.setSize('S');
                printer.print(F("No results on 10 sec"));
        }
        if (qdisplay2 == 1) {
                printer.justify('L');
                printer.setSize('M');
                printer.print(F("Player: "));
                printer.println(qplayer2);
                printer.print(F("Hits: "));
                printer.print(qshot2);
                printer.println(F(" st"));
                printer.print(F("Date: "));
                printer.println(qdatum2);
                printer.println(F("Played 20 sec"));
        } else {
                printer.justify('L');
                printer.setSize('S');
                printer.print(F("No results on 20 sec"));
        }
        if (qdisplay3 == 1) {
                printer.justify('L');
                printer.setSize('M');
                printer.print(F("Player: "));
                printer.println(qplayer3);
                printer.print(F("Hits: "));
                printer.print(qshot3);
                printer.println(F(" st"));
                printer.print(F("Date: "));
                printer.println(qdatum3);
                printer.println(F("Played 30 sec"));
        } else {
                printer.justify('L');
                printer.setSize('S');
                printer.print(F("No results on 30 sec"));
        }
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("--------------"));
        printer.justify('C');
        printer.setSize('M');
        printer.println(F("Rapid Fire:"));
        if (rdisplay == 1) {
                printer.println(F("6 Hits"));
                printer.justify('L');
                printer.setSize('M');
                printer.print(F("Player: "));
                printer.println(rplayer);
                printer.print(F("Time: "));
                printer.print(rtime);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(rdatum);
        } else {
                printer.println(F("6 Hits"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("No results"));
        }
        if (rdisplay_1 == 1) {
                printer.println(F("10 Hits"));
                printer.justify('L');
                printer.setSize('M');
                printer.print(F("Player: "));
                printer.println(rPlayer_1);
                printer.print(F("Time: "));
                printer.print(rtime_1);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(rdatum_1);
        } else {
                printer.println(F("10 Hits"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("No results"));
        }
        if (rdisplay_2 == 1) {
                printer.println(F("20 Hits"));
                printer.justify('L');
                printer.setSize('M');
                printer.print(F("Player: "));
                printer.println(rPlayer_2);
                printer.print(F("Time: "));
                printer.print(rtime_2);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(rdatum_2);
        } else {
                printer.println(F("20 Hits"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("No results"));
        }
        if (rdisplay_3 == 1) {
                printer.println(F("32 Hits"));
                printer.justify('L');
                printer.setSize('M');
                printer.print(F("Player: "));
                printer.println(rPlayer_3);
                printer.print(F("Time: "));
                printer.print(rtime_3);
                printer.println(F(" sec"));
                printer.print(F("Date: "));
                printer.println(rdatum_3);
        } else {
                printer.println(F("32 Hits"));
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("No results"));
        }
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("--------------"));
        printer.justify('C');
        printer.setSize('M');
        printer.println(F("Quick Draw:"));
        if (qqdisplay == 1) {
                printer.justify('L');
                printer.setSize('M');
                printer.print(F("Player: "));
                printer.println(qqplayer);
                printer.print(F("Round 1: "));
                printer.print(qtime1);
                printer.println(F(" sec"));
                printer.print(F("Round 2: "));
                printer.print(qtime2);
                printer.println(F(" sec"));
                printer.print(F("Round 3: "));
                printer.print(qtime3);
                printer.println(F(" sec"));
                printer.print(F("Round 4: "));
                printer.print(qtime4);
                printer.println(F(" sec"));
                printer.print(F("Round 5: "));
                printer.print(qtime5);
                printer.println(F(" sec"));
                printer.print(F("Round 6: "));
                printer.print(qtime6);
                printer.println(F(" sec"));
                printer.print(F("Round 7: "));
                printer.print(qtime7);
                printer.println(F(" sec"));
                printer.print(F("Round 8: "));
                printer.print(qtime1);
                printer.println(F(" sec"));
                printer.print(F("Datum: "));
                printer.println(qqdatum);
        } else {
                printer.justify('L');
                printer.setSize('M');
                printer.println(F("No results"));
        }
        printer.justify('C');
        printer.setSize('S');
        printer.println(F("--------------"));
        printer.print(F("Printed: "));
        printer.println(timeNow);

        printer.feed(3);

        printer.sleep();
        delay(3000L);
        printer.wake();
        printer.setDefault();
        mainscreen = 0;
        displayCodeEntryScreen();
        changeInfo = 1;
}
//Option menu.
void menu() {
        mainmenu = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("->Menu");
        while (mainmenu == 1) {
                if (digitalRead(selectButton)==HIGH) {
                        if (menuStatus == 0) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("->Menu");
                                lcd.setCursor(0,2);
                                lcd.print("-->Highscore");
                                lcd.setCursor(0,3);
                                lcd.print("Select with OK");
                                menuStatus = 1;
                                menumode = 1;
                                delay(500);
                        } else if (menuStatus == 1) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("->Menu");
                                lcd.setCursor(0,2);
                                lcd.print("-->Players");
                                lcd.setCursor(0,3);
                                lcd.print("Select with OK");
                                menuStatus = 2;
                                menumode = 3;
                                delay(500);

                        } else if (menuStatus == 2) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("->Menu");
                                lcd.setCursor(0,2);
                                lcd.print("-->Reset Highscore");
                                lcd.setCursor(0,3);
                                lcd.print("Select with OK");
                                menuStatus = 4;
                                menumode = 4;
                                delay(500);

                        } else if (menuStatus == 4) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("->Meny");
                                lcd.setCursor(0,2);
                                lcd.print("-->Test skjut");
                                lcd.setCursor(0,3);
                                lcd.print("Aktivera med OK");
                                menuStatus = 5;
                                menumode = 5;
                                delay(500);

                        } else if (menuStatus == 5) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("->Menu");
                                lcd.setCursor(0,2);
                                lcd.print("-->Time interval");
                                lcd.setCursor(0,3);
                                lcd.print("Select with OK");
                                menuStatus = 6;
                                menumode = 6;
                                delay(500);

                        } else if (menuStatus == 6) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("->Menu");
                                lcd.setCursor(0,2);
                                lcd.print("-->Latest results");
                                lcd.setCursor(0,3);
                                lcd.print("Select with OK");
                                menuStatus = 7;
                                menumode = 7;
                                delay(500);

                        } else if (menuStatus == 7) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("->Menu");
                                lcd.setCursor(0,2);
                                lcd.print("-->Date & Time");
                                lcd.setCursor(0,3);
                                lcd.print("Select with OK");
                                menuStatus = 8;
                                menumode = 8;
                                delay(500);

                        } else if (menuStatus == 8) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("->Menu");
                                lcd.setCursor(0,2);
                                lcd.print("-->Rapid Hits");
                                lcd.setCursor(0,3);
                                lcd.print("Select with OK");
                                menuStatus = 3;
                                menumode = 9;
                                delay(500);

                        } else if (menuStatus == 3) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("  AO Shooting Game");
                                lcd.setCursor(0,1);
                                lcd.print("->Menu");
                                lcd.setCursor(0,2);
                                lcd.print("-->Back");
                                lcd.setCursor(0,3);
                                lcd.print("Select with OK");
                                menuStatus = 0;
                                menumode = 2;
                                delay(500);

                        }
                }
                if (digitalRead(okButton)==HIGH && menumode == 2) {
                        displayCodeEntryScreen();
                        mainmenu = 0;
                        menumode = 0;
                        menuStatus = 0;
                        changeInfo = 1;
                        break;
                }
                if (digitalRead(okButton)==HIGH && menumode == 1) {
                        timeHigh();
                        mainmenu = 0;
                        menumode = 0;
                        menuStatus = 0;
                        break;
                }
                if (digitalRead(okButton)==HIGH && menumode == 3) {
                        playMode = 0;
                        enterPlayMode();
                        mainmenu = 0;
                        menumode = 0;
                        menuStatus = 0;
                        break;
                }
                if (digitalRead(okButton)==HIGH && menumode == 4) {
                        playMode = 0;
                        resetHigh = 1;
                        resetHighscore();
                        mainmenu = 0;
                        menumode = 0;
                        menuStatus = 0;
                        break;
                }
                if (digitalRead(okButton)==HIGH && menumode == 5) {
                        playMode = 0;
                        testShotGame = 1;
                        testShot();
                        mainmenu = 0;
                        menumode = 0;
                        menuStatus = 0;
                        break;
                }
                if (digitalRead(okButton)==HIGH && menumode == 6) {
                        playMode = 0;
                        setTimedTime();
                        mainmenu = 0;
                        menumode = 0;
                        menuStatus = 0;
                        break;
                }
                if (digitalRead(okButton)==HIGH && menumode == 7) {
                        playMode = 0;
                        mainmenu = 0;
                        menumode = 0;
                        menuStatus = 0;
                        latestTime();
                        break;
                }
                if (digitalRead(okButton)==HIGH && menumode == 8) {
                        playMode = 0;
                        setTheTime();
                        mainmenu = 0;
                        menumode = 0;
                        menuStatus = 0;
                        break;
                }
                if (digitalRead(okButton)==HIGH && menumode == 9) {
                        playMode = 0;
                        setRapidFire();
                        mainmenu = 0;
                        menumode = 0;
                        menuStatus = 0;
                        break;
                }
        }
}
//Set the shooting for rapid fire.
void setRapidFire() {
        int rset = 0;
        int rshot = 0;
        int setRap = 0;
        int godkann = 0;
        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Select hits:");
        setRap = 1;
        while (setRap == 1) {
                if (digitalRead(selectButton)==HIGH) {
                        if (rset == 0) {
                                rshot = 0;
                                lcd.setCursor(0,2);
                                lcd.print("6 Hits ");
                                rset = 1;
                                delay(500);
                        } else if (rset == 1) {
                                rshot = 1;
                                lcd.setCursor(0,2);
                                lcd.print("10 Hits");
                                rset = 2;
                                delay(500);
                        } else if (rset == 2) {
                                rshot = 2;
                                lcd.setCursor(0,2);
                                lcd.print("20 Hits");
                                rset = 3;
                                delay(500);
                        } else if (rset == 3) {
                                rshot = 3;
                                lcd.setCursor(0,2);
                                lcd.print("32 Hits");
                                rset = 0;
                                delay(500);
                        }
                }
                if (digitalRead(okButton)==HIGH) {
                        setRap = 0;
                        godkann = 1;
                        break;
                }

        }
        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Press select to");
        lcd.setCursor(0,2);
        lcd.print("use");
        lcd.print("if not press OK");
        if (rshot == 0) {
                lcd.setCursor(0,3);
                lcd.print("6 Hits ");

        } else if (rshot == 1) {
                lcd.setCursor(0,3);
                lcd.print("10 Hits");
        } else if (rshot == 2) {
                lcd.setCursor(0,3);
                lcd.print("20 Hits");
        } else if (rshot == 3) {
                lcd.setCursor(0,3);
                lcd.print("32 Hits");
        }
        while (godkann == 1) {
                if (digitalRead(selectButton)==HIGH) {
                        godkann = 0;
                        playMode = 1;
                        rshooting = rshot;
                        if (rshot == 0) {
                                SD.remove("rsset.txt");
                                hquickFil = SD.open("rsset.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0");
                                        hquickFil.close();
                                }
                        } else if (rshot == 1) {
                                SD.remove("rsset.txt");
                                hquickFil = SD.open("rsset.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("1");
                                        hquickFil.close();
                                }
                        } else if (rshot == 2) {
                                SD.remove("rsset.txt");
                                hquickFil = SD.open("rsset.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("2");
                                        hquickFil.close();
                                }
                        } else if (rshot == 3) {
                                SD.remove("rsset.txt");
                                hquickFil = SD.open("rsset.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("3");
                                        hquickFil.close();
                                }
                        }
                        displayCodeEntryScreen();
                        break;
                }

                if (digitalRead(okButton)==HIGH) {
                        godkann = 0;
                        playMode = 1;
                        displayCodeEntryScreen();
                        break;
                }
        }
}
//Set the clock in the game.
void setTheTime() {
        int setYear = 0;
        int setMonth = 0;
        int stepYear = 0;
        int setYtime = 1;
        int setMtime = 0;
        int setDtime = 0;
        int setHtime = 0;
        int setMItime = 0;
        int setStime = 0;
        int godkann = 0;
        int yset = 2016;
        int mset = 1;
        int dset = 1;
        int hset = 0;
        int miset = 0;
        int sset = 0;
        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Select year:");
        lcd.setCursor(0,2);
        lcd.print(yset);
        setYear = 1;
        while (setYtime == 1) {

                if (digitalRead(selectButton)==HIGH) {
                        if (yset <= 2022) {
                                yset = yset +1;
                                lcd.setCursor(0,2);
                                lcd.print(yset);
                                delay(500);
                        } else {
                                yset = 2016;
                                lcd.setCursor(0,2);
                                lcd.print(yset);
                                delay(500);
                        }
                }
                if (digitalRead(okButton)==HIGH) {
                        setYear = 0;
                        setMonth = 1;
                        setYtime = 0;
                        setMtime = 1;
                        break;
                }


        }
        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Select month:");
        lcd.setCursor(0,2);
        lcd.print(mset);
        while (setMtime == 1) {

                if (digitalRead(selectButton)==HIGH) {
                        if (mset <= 11) {
                                mset = mset +1;
                                lcd.setCursor(0,2);
                                lcd.print(mset);
                                delay(500);
                        } else {
                                mset = 1;
                                lcd.setCursor(0,2);
                                lcd.print("       ");
                                lcd.setCursor(0,2);
                                lcd.print(mset);
                                delay(500);
                        }
                }

                if (digitalRead(okButton)==HIGH) {
                        setYear = 0;
                        setMonth = 0;
                        setYtime = 0;
                        setMtime = 0;
                        setDtime = 1;
                        break;
                }


        }
        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Select day:");
        lcd.setCursor(0,2);
        lcd.print(dset);
        while (setDtime == 1) {
                if (digitalRead(selectButton)==HIGH) {
                        if (dset <= 30) {
                                dset = dset +1;
                                lcd.setCursor(0,2);
                                lcd.print(dset);
                                delay(500);
                        } else {
                                dset = 1;
                                lcd.setCursor(0,2);
                                lcd.print("       ");
                                lcd.setCursor(0,2);
                                lcd.print(dset);
                                delay(500);
                        }
                }

                if (digitalRead(okButton)==HIGH) {
                        setYear = 0;
                        setMonth = 0;
                        setYtime = 0;
                        setMtime = 0;
                        setDtime = 0;
                        setHtime = 1;
                        break;
                }

        }

        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Select hour:");
        lcd.setCursor(0,2);
        lcd.print(hset);
        while (setHtime == 1) {
                if (digitalRead(selectButton)==HIGH) {
                        if (hset <= 22) {
                                hset = hset +1;
                                lcd.setCursor(0,2);
                                lcd.print(hset);
                                delay(500);
                        } else {
                                hset = 0;
                                lcd.setCursor(0,2);
                                lcd.print("       ");
                                lcd.setCursor(0,2);
                                lcd.print(hset);
                                delay(500);
                        }
                }

                if (digitalRead(okButton)==HIGH) {
                        setYear = 0;
                        setMonth = 0;
                        setYtime = 0;
                        setMtime = 0;
                        setMItime = 1;
                        break;
                }

        }

        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Select minute:");
        lcd.setCursor(0,2);
        lcd.print(miset);
        while (setMItime == 1) {
                if (digitalRead(selectButton)==HIGH) {
                        if (miset <= 58) {
                                miset = miset +1;
                                lcd.setCursor(0,2);
                                lcd.print(miset);
                                delay(500);
                        } else {
                                miset = 0;
                                lcd.setCursor(0,2);
                                lcd.print("       ");
                                lcd.setCursor(0,2);
                                lcd.print(miset);
                                delay(500);
                        }
                }

                if (digitalRead(okButton)==HIGH) {
                        setYear = 0;
                        setMonth = 0;
                        setYtime = 0;
                        setMtime = 0;
                        setStime = 1;
                        break;
                }

        }

        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Select secound:");
        lcd.setCursor(0,2);
        lcd.print(sset);
        while (setMItime == 1) {
                if (digitalRead(selectButton)==HIGH) {
                        if (sset <= 58) {
                                sset = sset +1;
                                lcd.setCursor(0,2);
                                lcd.print(sset);
                                delay(500);
                        } else {
                                sset = 0;
                                lcd.setCursor(0,2);
                                lcd.print("       ");
                                lcd.setCursor(0,2);
                                lcd.print(sset);
                                delay(500);
                        }
                }

                if (digitalRead(okButton)==HIGH) {
                        setYear = 0;
                        setMonth = 0;
                        setYtime = 0;
                        setMtime = 0;
                        setStime = 0;
                        godkann = 1;
                        break;
                }

        }

        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Press select to");
        lcd.setCursor(0,2);
        lcd.print("use or else OK");
        lcd.setCursor(0,3);
        lcd.print(yset);
        lcd.print("-");
        if (mset < 10) {
                lcd.print("0");
                lcd.print(mset);
        } else {
                lcd.print(mset);
        }
        lcd.print("-");
        if (dset < 10) {
                lcd.print("0");
                lcd.print(dset);
        } else {
                lcd.print(dset);
        }
        lcd.print(" ");
        if (hset < 10) {
                lcd.print("0");
                lcd.print(hset);
        } else {
                lcd.print(hset);
        }
        lcd.print(":");
        if (miset < 10) {
                lcd.print("0");
                lcd.print(miset);
        } else {
                lcd.print(miset);
        }
        lcd.print(":");
        if (sset < 10) {
                lcd.print("0");
                lcd.print(sset);
        } else {
                lcd.print(sset);
        }
        while (godkann == 1) {
                if (digitalRead(selectButton)==HIGH) {
                        godkann = 0;
                        playMode = 1;
                        RTC.adjust(DateTime(yset, mset, dset, hset, miset, sset));
                        displayCodeEntryScreen();
                        break;
                }

                if (digitalRead(okButton)==HIGH) {
                        godkann = 0;
                        playMode = 1;
                        displayCodeEntryScreen();
                        break;
                }
        }
}
//Select players.
void enterPlayMode() {
        int playOK = 0;
        if (playMode == 0) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Select players:");
                lcd.setCursor(0,3);
                lcd.print("1");
                while (playMode == 0) {
                        selectState = digitalRead(selectButton);

                        if (digitalRead(selectButton)==HIGH) {
                                playOK = 1;
                                if (steps == 0) {
                                        lcd.setCursor(0,3);
                                        lcd.print("1");
                                        players = 1;
                                        steps = 1;
                                        delay(500);
                                } else if (steps == 1) {
                                        lcd.setCursor(0,3);
                                        lcd.print("2");
                                        players = 2;
                                        steps = 2;
                                        delay(500);
                                } else if (steps == 2) {
                                        lcd.setCursor(0,3);
                                        lcd.print("3");
                                        players = 3;
                                        steps = 3;
                                        delay(500);
                                } else if (steps == 3) {
                                        lcd.setCursor(0,3);
                                        lcd.print("4");
                                        players = 4;
                                        steps = 0;
                                        delay(500);
                                }
                        }
                        okState = digitalRead(okButton);
                        if (digitalRead(okButton)==HIGH && playOK == 1) {
                                playMode = 1;
                                playOK = 0;
                                break;
                        }

                }
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  AO Shooting Game");
                lcd.setCursor(0,2);
                lcd.print("Players selected:");
                lcd.setCursor(0,3);
                lcd.print(players);
                delay(3000);
                playMode = 1;
                displayCodeEntryScreen();
                changeInfo = 1;
        }
}
//Select wired or wireless.
void setGameType() {
        delay(500);
        int playOK = 0;
        int selectTypeS = 1;
        int selectSteps = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Select sensor type");
        lcd.setCursor(0,2);
        lcd.print("Save with OK button");
        lcd.setCursor(0,3);
        lcd.print("- -");
        while (selectTypeS == 1) {
                if (digitalRead(selectButton)==HIGH) {
                        playOK = 1;
                        if (selectSteps == 0) {
                                lcd.setCursor(0,3);
                                lcd.print("Cable ");
                                GameModel = 1;
                                selectSteps = 1;
                                delay(500);
                        } else if (selectSteps == 1) {
                                lcd.setCursor(0,3);
                                lcd.print("Wireless");
                                GameModel = 0;
                                selectSteps = 0;
                                delay(500);
                        }

                }

                if (digitalRead(okButton)==HIGH && playOK == 1) {
                        selectSteps = 1;
                        selectTypeS = 0;
                        if (GameModel == 1) {
                                lcd.setCursor(0,3);
                                lcd.print("Cable selected");
                        } else {
                                lcd.setCursor(0,3);
                                lcd.print("Wireless selected");
                        }

                        playOK = 0;
                        break;
                }
        }
}
//Select time in Timed Mode.
void setTimedTime() {
        delay(500);
        int selectSteps = 1;
        int playOK = 0;
        selectTimed = 1;
        int timeChoose = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Select secounds");
        lcd.setCursor(0,2);
        lcd.print("Save with OK button");
        lcd.setCursor(0,3);
        lcd.print("10 sec");
        while (selectTimed == 1) {
                if (digitalRead(selectButton)==HIGH) {
                        playOK = 1;
                        if (selectSteps == 0) {
                                lcd.setCursor(0,3);
                                lcd.print("10 sec");
                                timedRun = 10000;
                                timeChoose = 1;
                                timeSel = 1;
                                selectSteps = 1;
                                delay(500);
                        } else if (selectSteps == 1) {
                                lcd.setCursor(0,3);
                                lcd.print("20 sec");
                                timedRun = 20000;
                                timeChoose = 2;
                                timeSel = 2;
                                selectSteps = 2;
                                delay(500);
                        } else if (selectSteps == 2) {
                                lcd.setCursor(0,3);
                                lcd.print("30 sec");
                                timedRun = 30000;
                                timeChoose = 3;
                                timeSel = 3;
                                selectSteps = 0;
                                delay(500);
                        }
                }
                if (digitalRead(okButton)==HIGH && playOK == 1) {
                        selectSteps = 1;
                        selectTimed = 0;

                        playOK = 0;
                        break;
                }
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,2);
        lcd.print("Secounds selected:");
        lcd.setCursor(0,3);
        if (timeChoose == 1) {
                lcd.print("10 sec");
                SD.remove("tset.txt");
                hquickFil = SD.open("tset.txt", FILE_WRITE);
                if (hquickFil) {
                        hquickFil.println("1");
                        hquickFil.close();
                }
        } else if (timeChoose == 2) {
                lcd.print("20 sec");
                SD.remove("tset.txt");
                hquickFil = SD.open("tset.txt", FILE_WRITE);
                if (hquickFil) {
                        hquickFil.println("2");
                        hquickFil.close();
                }
        } else if (timeChoose == 3) {
                lcd.print("30 sec");
                SD.remove("tset.txt");
                hquickFil = SD.open("tset.txt", FILE_WRITE);
                if (hquickFil) {
                        hquickFil.println("3");
                        hquickFil.close();
                }
        }
        delay(3000);
        playMode = 1;
        displayCodeEntryScreen();
}
//Clear the higscore.
void resetHighscore() {
        delay(500);
        resetHigh = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Press select to");
        lcd.setCursor(0,2);
        lcd.print("clear highscores");
        lcd.setCursor(0,3);
        lcd.print("Or else OK");
        while (resetHigh == 1) {
                okState = digitalRead(okButton);
                if (digitalRead(okButton)==HIGH && resetHigh == 1) {
                        resetHigh = 0;
                        playMode = 1;
                        displayCodeEntryScreen();
                        changeInfo = 1;
                        break;
                }
                if (digitalRead(selectButton)==HIGH && resetHigh == 1) {
                        SD.remove("htime.txt");
                        SD.remove("htime2.txt");
                        SD.remove("htime3.txt");
                        SD.remove("hqu.txt");
                        SD.remove("hrap.txt");
                        SD.remove("hrap1.txt");
                        SD.remove("hrap2.txt");
                        SD.remove("hrap3.txt");
                        SD.remove("s1.txt");
                        SD.remove("s2.txt");
                        SD.remove("s3.txt");
                        SD.remove("s4.txt");
                        if (SD.exists("htime.txt")) {


                        } else {
                                hquickFil = SD.open("htime.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0,0,2016-05-05 10:00");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("htime2.txt")) {


                        } else {
                                hquickFil = SD.open("htime2.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0,0,2016-05-05 10:00");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("htime3.txt")) {


                        } else {
                                hquickFil = SD.open("htime3.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0,0,2016-05-05 10:00");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("hqu.txt")) {


                        } else {
                                hquickFil = SD.open("hqu.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0,0,0,0,0,0,0,0,0,2016-05-05 10:00");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("hrap.txt")) {


                        } else {
                                hquickFil = SD.open("hrap.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0,0,2016-05-05 10:00");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("hrap1.txt")) {


                        } else {
                                hquickFil = SD.open("hrap1.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0,0,2016-05-05 10:00");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("hrap2.txt")) {


                        } else {
                                hquickFil = SD.open("hrap2.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0,0,2016-05-05 10:00");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("hrap3.txt")) {


                        } else {
                                hquickFil = SD.open("hrap3.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0,0,2016-05-05 10:00");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("s1.txt")) {


                        } else {
                                hquickFil = SD.open("s1.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("s2.txt")) {


                        } else {
                                hquickFil = SD.open("s2.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("s3.txt")) {


                        } else {
                                hquickFil = SD.open("s3.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0");
                                        hquickFil.close();
                                }
                        }
                        if (SD.exists("s4.txt")) {


                        } else {
                                hquickFil = SD.open("s4.txt", FILE_WRITE);
                                if (hquickFil) {
                                        hquickFil.println("0");
                                        hquickFil.close();
                                }
                        }
                        resetHigh = 0;
                        playMode = 1;
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  AO Shooting Game");
                        lcd.setCursor(0,2);
                        lcd.print("Highscore cleared!!");
                        delay(5000);
                        displayCodeEntryScreen();
                        changeInfo = 1;
                        break;
                }
        }
}
//Start test shooting.
void testShot() {
        playMode = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Test shoot on");
        lcd.setCursor(0,2);
        lcd.print("on targets.");
        lcd.setCursor(0,3);
        lcd.print("When target turns on");
        delay(5000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,1);
        lcd.print("Shoot and when hits");
        lcd.setCursor(0,2);
        lcd.print("The next will activate");
        lcd.setCursor(0,3);
        lcd.print("Good luck!!");
        delay(5000);
        int activate1 = 0;
        int activate2 = 0;
        int activate3 = 0;
        int activate4 = 0;
        int currentPort = 0;
        activate2 = 1;
        while (testShotGame == 1) {
                if (GameModel == 1) { //For wired.
                        digitalWrite(led[currentPort],HIGH);
                        switch (currentPort)
                        {
                        case 0:
                                while (digitalRead(ins[0])==LOW) ;
                                break;
                        case 1:
                                while (digitalRead(ins[1])==LOW) ;
                                break;
                        case 2:
                                while (digitalRead(ins[2])==LOW) ;
                                break;
                        case 3:
                                while (digitalRead(ins[3])==LOW) ;
                                activate1 = 1;
                                break;
                        }
                        digitalWrite(led[currentPort],LOW);
                } else { //For wireless
                        bool skicka3 = false;
                        bool skicka4 = false;
                        bool skicka5 = false;
                        bool skicka6 = false;
                        bool skicka7 = false;
                        bool skicka8 = false;
                        int backloop = 0;
                        if (currentPort == 0) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[0]);
                                radio.openReadingPipe(1,pipes[5]);
                                transmitterId = 16;
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                skicka1 = true;
                        } else if (currentPort == 1) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[2]);
                                radio.openReadingPipe(1,pipes[6]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 26;
                                skicka3 = true;
                        } else if (currentPort == 2) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[3]);
                                radio.openReadingPipe(1,pipes[7]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 36;
                                skicka5 = true;


                        } else if (currentPort == 3) {
                                radio.stopListening();
                                radio.openWritingPipe(pipes[4]);
                                radio.openReadingPipe(1,pipes[8]);
                                Serial.print("Port: ");
                                Serial.println(currentPort);
                                transmitterId = 46;
                                skicka7 = true;


                        }

                        while (skicka1 == true)
                        {

                                if(radio.write(&transmitterId, sizeof(transmitterId))) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);

                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka2 = true;
                                                        skicka1 = false;
                                                        Serial.println(transmitterId);

                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }




                        }

                        while (skicka2 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData, sizeof(nockData));
                                if (radio.available(pipes[5])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[5])) {
                                                Serial.println("Start Lissening");
                                                radio.read(&senderId, sizeof(senderId));

                                        }
                                }  else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 17) //Target 1
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka2 = false;
                                        skicka1 = false;
                                }


                                okState = digitalRead(okButton);
                                if (digitalRead(okButton)==HIGH) {
                                        skicka2 = false;
                                        skicka1 = false;
                                        ackMessg = 0;
                                        activate1 = 1;
                                        break;
                                }

                        }

                        while (skicka3 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData3) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka4 = true;
                                                        skicka3 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka4 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData2, sizeof(nockData2));
                                if (radio.available(pipes[6])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[6])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Recived: ");
                                                Serial.println(senderId);

                                        }

                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 27) //Target 2
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka4 = false;
                                        skicka3 = false;
                                }

                                if (digitalRead(okButton)==HIGH) {
                                        skicka4 = false;
                                        skicka3 = false;
                                        activate1 = 1;
                                        ackMessg = 0;
                                        break;
                                }


                        }
                        while (skicka5 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData5) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka6 = true;
                                                        skicka5 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka6 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData3, sizeof(nockData3));
                                if (radio.available(pipes[7])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[7])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Recived: ");
                                                Serial.println(senderId);
                                        }
                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 37) //Target 3
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka5 = false;
                                        skicka6 = false;
                                }

                                if (digitalRead(okButton)==HIGH) {
                                        skicka6 = false;
                                        skicka5 = false;
                                        activate1 = 1;
                                        ackMessg = 0;
                                        break;
                                }



                        }
                        while (skicka7 == true)
                        {
                                if(radio.write(&transmitterId, sizeof(transmitterId)))
                                {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        if ( radio.isAckPayloadAvailable() ) {
                                                radio.read(&ackMessg,sizeof(ackMessg));
                                                if (ackMessg == ackData7) {

                                                        Serial.println("Sending OK");
                                                        Serial.print("Acknowledge received: ");
                                                        Serial.println(ackMessg);
                                                        skicka8 = true;
                                                        skicka7 = false;
                                                        Serial.println(transmitterId);
                                                        radio.startListening();
                                                }
                                        }
                                }
                                else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }


                        }

                        while (skicka8 == true)
                        {
                                delay(10);
                                radio.writeAckPayload(1, &nockData4, sizeof(nockData4));
                                if (radio.available(pipes[8])) {
                                        digitalWrite(transmittLed, HIGH);
                                        digitalWrite(transmittErrorLed, LOW);
                                        while (radio.available(pipes[8])) {
                                                radio.read(&senderId, sizeof(senderId));
                                                Serial.print("Recived: ");
                                                Serial.println(senderId);
                                        }
                                } else {
                                        digitalWrite(transmittLed, LOW);
                                        digitalWrite(transmittErrorLed, HIGH);
                                }

                                if (senderId == 47) //Target 4
                                {
                                        Serial.println("Target Activated ");
                                        Serial.println(senderId);
                                        Serial.println(" Correct , ");
                                        radio.stopListening();
                                        senderId = 0;
                                        ackMessg = 0;
                                        skicka7 = false;
                                        skicka8 = false;
                                        activate1 = 1;
                                }

                                if (digitalRead(okButton)==HIGH) {
                                        skicka7 = false;
                                        skicka8 = false;
                                        activate1 = 1;
                                        ackMessg = 0;
                                        break;
                                }


                        }


                }

                currentPort = currentPort +1;
                if (activate1 == 1) {
                        activate1 = 0;
                        currentPort = 0;
                        testShotGame = 0;
                        displayCodeEntryScreen();
                        break;
                        changeInfo = 1;
                }

        }
}
//Clear the screen for the game.
void displayCodeEntryScreen()
{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  AO Shooting Game");
        lcd.setCursor(0,2);
        lcd.print("Select game and start");
        lcd.setCursor(0,3);
        lcd.print("Players: ");
        lcd.print(players);
        delay(500);
        changeInfo = 1;
}
//Flash the led for wired.
void doPost()
{
        Serial.write(0xD2);
        Serial.write(0xE3);
        Serial.write(0xDC);
        Serial.write(0xDE);
        for (int lop=0; lop<20; lop++)
        {
                theDelay+=10;
                if (theDelay>500) theDelay=1;

                for (int i=0; i<numPorts; i++)
                {
                        {
                                digitalWrite(led[i], HIGH);
                        }
                }
                delay(theDelay);
                for (int i=0; i<numPorts; i++)
                {
                        digitalWrite(led[i], LOW);
                }
                delay(theDelay);
        }
}
