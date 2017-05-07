/*
 * Arduino Wireless target sensor
 * Made by Andreas Olsson v3.12
 * 
 * This is for all targets
 * Each target have there own code, they are listening for unicq numbers.
 * 
 * The check loop is for the start up of the game, to enable an test communication
 * with all the targets to verify that they can comunicate.
 * 
 * ID numbers for targets, don't use same numbers for all targets.
 * 
 * Recive ID: 16 > Target 1, 26 > Target 2, 36 > Target 3, 46 > Target 4
 * Send ID: 17 > Target 1, 27 > Target 2, 37 > Target 3, 47 > Target 4
 * Test ID: 91 > Target 1, 92 > Target 2, 93 > Target 3, 94 > Target 4
 * OK Test ID: 81 > Target 1, 82 > Target 2, 83 > Target 3, 84 > Target 4
 * AckPayLoad: 61,62,51 > Target 1, 63,64,52 > Target 2, 65,66,53 > Target 3, 67,68,54 > Target 4
 * 
 * Change the Define text to switch target. SENSOR1, SENSOR2, SENSOR3, SENSOR4
 */
#define SENSOR1

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//Knock sensor on pin A0
const int knockSensor = A0;
int theDelay=1; //Delay for game test

//ID That is given
int senderId; //16 is for target one
//For sending id
int transmitterId = 0;

// Output LED
int LEDpin = 3; //Blue
int LEDpin1 = 5; //Red
int LEDpin2 = 7; //Green

//Enable knock sensor
boolean knock = false;
//Enable sending back
boolean sending = false;
boolean checkok = false;
#ifdef SENSOR1
int ValTest = 91;
#endif
#ifdef SENSOR2
int ValTest = 92;
#endif
#ifdef SENSOR3
int ValTest = 93;
#endif
#ifdef SENSOR4
int ValTest = 94;
#endif

#ifdef SENSOR1
int ValSig = 16;
#endif
#ifdef SENSOR2
int ValSig = 26;
#endif
#ifdef SENSOR3
int ValSig = 36;
#endif
#ifdef SENSOR4
int ValSig = 46;
#endif

#ifdef SENSOR1
int ackData = 61;
int ackData2 = 62;
int nockData = 51;
#endif
#ifdef SENSOR2
int ackData = 63;
int ackData2 = 64;
int nockData = 52;
#endif
#ifdef SENSOR3
int ackData = 65;
int ackData2 = 66;
int nockData = 53;
#endif
#ifdef SENSOR4
int ackData = 67;
int ackData2 = 68;
int nockData = 54;
#endif
int ackMessg;

RF24 radio(9, 10);

#ifdef SENSOR1
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
#endif
#ifdef SENSOR2
const uint64_t pipes[2] = { 0xF0F0F0F0C3LL, 0xF0F0F0F0D2LL };
#endif
#ifdef SENSOR3
const uint64_t pipes[2] = { 0xF0F0F0F0C1LL, 0xF0F0F0F0D2LL };
#endif
#ifdef SENSOR4
const uint64_t pipes[2] = { 0xF0F0F0F0E3LL, 0xF0F0F0F0D2LL };
#endif

void setup() {
Serial.begin(9600);
#ifdef SENSOR1
transmitterId = 17; //This id is for target one
#endif
#ifdef SENSOR2
transmitterId = 27; //This id is for target two
#endif
#ifdef SENSOR3
transmitterId = 37; //This id is for target three
#endif
#ifdef SENSOR4
transmitterId = 47; //This id is for target four
#endif
//Enable all the LED pins as output
pinMode(LEDpin, OUTPUT);
digitalWrite(LEDpin, LOW); // target signal
pinMode(LEDpin1, OUTPUT);
digitalWrite(LEDpin1, LOW); // connection
pinMode(LEDpin2, OUTPUT);
digitalWrite(LEDpin2, HIGH); // power is on
doTest();

radio.begin();

radio.setDataRate(RF24_250KBPS);
radio.setChannel(120); //Sending on channel 120

radio.openWritingPipe(pipes[1]);
radio.openReadingPipe(1,pipes[0]);
radio.enableAckPayload();
radio.writeAckPayload(1, &ackData, sizeof(ackData));
radio.startListening();
Serial.println(senderId);

}

void loop() {
  startSensor(); //Start the game loop

}

void doCheck()
{
  Serial.println("Start Lamps");
for (int lop=0; lop<20; lop++)
{
 theDelay+=10;
 if (theDelay>500) theDelay=1;
 
  digitalWrite(LEDpin, HIGH);
  digitalWrite(LEDpin1, LOW);
  digitalWrite(LEDpin2, LOW); 
  delay(theDelay); //Wait a second
  digitalWrite(LEDpin, LOW);
  digitalWrite(LEDpin1, HIGH);
  digitalWrite(LEDpin2, LOW);
  delay(theDelay); //Wait a second
  digitalWrite(LEDpin, LOW);
  digitalWrite(LEDpin1, LOW);
  digitalWrite(LEDpin2, HIGH);
  delay(theDelay); //Wait a second
}
digitalWrite(LEDpin, LOW);
digitalWrite(LEDpin1, HIGH);
digitalWrite(LEDpin2, HIGH);   
Serial.println("Stop Lamps");
}

void doTest()
{
   
for (int lop=0; lop<20; lop++)
{
 theDelay+=10;
 if (theDelay>500) theDelay=1;
 
  digitalWrite(LEDpin, HIGH);
  digitalWrite(LEDpin1, LOW); 
  delay(theDelay); //Wait a second
  digitalWrite(LEDpin, LOW);
  digitalWrite(LEDpin1, HIGH);
  delay(theDelay); //Wait a second
} 

}//End doTest()

void startSensor()
{
radio.writeAckPayload(1, &ackData, sizeof(ackData));  
if (radio.available()) {
  Serial.println("Radio Found");

digitalWrite(LEDpin1, LOW);
radio.read(&senderId, sizeof(senderId));
Serial.println(senderId);
Serial.println(ackData);
delay(40);

Serial.print("Received = ");
Serial.println(senderId);
radio.startListening();
 } else {
Serial.print("No connection");
digitalWrite(LEDpin1, HIGH);
radio.startListening();
}

while (senderId == ValTest) //Test ID
{
Serial.print("Test Recived");
#ifdef SENSOR1  
transmitterId = 81;
#endif
#ifdef SENSOR2
transmitterId = 82;
#endif
#ifdef SENSOR3
transmitterId = 83;
#endif
#ifdef SENSOR4
transmitterId = 84;
#endif

  radio.stopListening();
  Serial.println("Redy to send test");
  Serial.println(transmitterId);
  bool ok = radio.write(&transmitterId, sizeof(transmitterId));
   delay(30);
         if (ok) {
          Serial.print(transmitterId);
          if ( radio.isAckPayloadAvailable() ) {
          radio.read(&ackMessg,sizeof(ackMessg));
          if (ackMessg == ackData2) {  
          senderId = 0;
          checkok = true; //Enable Check OK Loop
           
           }
          }
         
         } 
        else {
          Serial.print("Failed Sending");
          //senderId = 0;
          digitalWrite(LEDpin1, HIGH);
          }
} //Test Stop

//If the id is for this target
while (senderId == ValSig)
{
#ifdef SENSOR1
transmitterId = 17; //This id is for target one
#endif
#ifdef SENSOR2
transmitterId = 27; //This id is for target two
#endif
#ifdef SENSOR3
transmitterId = 37; //This id is for target three
#endif
#ifdef SENSOR4
transmitterId = 47; //This id is for target four
#endif
 
digitalWrite(LEDpin, HIGH);
Serial.print("Target ID ");
Serial.print(senderId);
Serial.print(" On , ");
knock = true;
delay(20);
//Run while loop until we registrate the hit
while (knock == true)
{
int val= analogRead(knockSensor);
  if (digitalRead(knockSensor) == HIGH)
  {
  Serial.print("Knock");
  digitalWrite(LEDpin, LOW); //Knock is made stop light
  
    radio.stopListening();
    delay(20);
  bool ok = radio.write(&transmitterId, sizeof(transmitterId));
   delay(20);
         if (ok) {
          if ( radio.isAckPayloadAvailable() ) {
          radio.read(&ackMessg,sizeof(ackMessg));
          if (ackMessg == nockData) {  
          
          Serial.print(transmitterId);
          senderId = 0;
          knock = false;
         }}} 
        else {
          Serial.print("Failed Sending");
          digitalWrite(LEDpin1, HIGH);
          }
  } 
 } //While loop stop
} //Sender ID stop




 //Make a new loop for sending the ok test
if (checkok == true)
{
  
  checkok = false;
  doCheck();
  Serial.print("Leave Check OK");
} //While loop stop  


} //End StartSensor()
