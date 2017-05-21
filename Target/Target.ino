/*
   Arduino Wireless target sensor
   Made by Andreas Olsson v3.12

   This is for all targets
   Each target have there own code, they are listening for unicq numbers.

   The check loop is for the start up of the game, to enable an test communication
   with all the targets to verify that they can comunicate.

   ID numbers for targets, don't use same numbers for all targets.

   Recive ID: 16 > Target 1, 26 > Target 2, 36 > Target 3, 46 > Target 4
   Send ID: 17 > Target 1, 27 > Target 2, 37 > Target 3, 47 > Target 4
   Test ID: 91 > Target 1, 92 > Target 2, 93 > Target 3, 94 > Target 4
   OK Test ID: 81 > Target 1, 82 > Target 2, 83 > Target 3, 84 > Target 4
   AckPayLoad: 61,62,51 > Target 1, 63,64,52 > Target 2, 65,66,53 > Target 3, 67,68,54 > Target 4

   Change the Define text to switch target. SENSOR1, SENSOR2, SENSOR3, SENSOR4
 */
#define SENSOR1
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//Knock sensor on pin A0
const int knockSensor = A0;
int theDelay = 1; //Delay for game test

//ID That is given
int senderId; //16 is for target one
//For sending id
int transmitterId = 0;

int testloop = 0;
int shootloop = 0;
int secloop = 0;
int loopok = 1;

// Output LED
int LEDpin = 3; //Blue
int LEDpin1 = 5; //Red
int LEDpin2 = 7; //Green

//Enable knock sensor
boolean knock = false;
//Verify the knock is OK
boolean knokisok = false;
//Enable sending back
boolean sending = false;
boolean checkok = false;
boolean testar = false;
boolean skjuter = false;
boolean testingloop = false;
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
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
const long interval = 30000; //This is where you set the safe time incase it stucks in transmitting the sending or hanging. 30000 is 30 sec
int looprun = 5; //This is how many times a loop will be running before we retry sending.
int secrun = 20; //This is a secure loop runs befor it exit the loops.

RF24 radio(9, 10);

#ifdef SENSOR1
//const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xABCDABCD71LL };
//const uint8_t pipes[][2] = {"1Node","2Node"};
#endif
#ifdef SENSOR2
//const uint8_t pipes[][2] = {"3Node","4Node"};
const uint64_t pipes[2] = { 0xF0F0F0F0C3LL, 0xF0F0F0F096LL };
//const uint64_t pipes[2] = { 0xF0F0F0F0C3LL, 0xF0F0F0F0D2LL };
#endif
#ifdef SENSOR3
//const uint64_t pipes[2] = { 0xF0F0F0F0C1LL, 0xF0F0F0F0D2LL };
const uint64_t pipes[2] = { 0xF0F0F0F0C1LL, 0x3A3A3A3AD2LL };
#endif
#ifdef SENSOR4
//const uint64_t pipes[2] = { 0xF0F0F0F0E3LL, 0xF0F0F0F0D2LL };
const uint64_t pipes[2] = { 0xF0F0F0F0E3LL, 0x3A3A3A3AC3LL };
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
        radio.setPALevel(RF24_PA_LOW);
        radio.setDataRate(RF24_1MBPS);
        radio.setRetries(15, 15);
        radio.setChannel(120); //Sending on channel 120
        radio.setPayloadSize(8);
        radio.openWritingPipe(pipes[1]);
        radio.openReadingPipe(1, pipes[0]);
        radio.enableAckPayload();
        radio.writeAckPayload(1, &ackData, sizeof(ackData)); //Sending out the first AckPayLoad
        radio.startListening();
        Serial.println(senderId);

}

void loop() {
        //  startSensor(); //Start the game loop

        boolean knockisok = false;
        //Serial.println("Waiting");
        //delay(5);

        if (radio.available(pipes[0])) {
                Serial.println("Radio Found");
                while (radio.available(pipes[0])) {
                        digitalWrite(LEDpin1, LOW);
                        digitalWrite(LEDpin2, HIGH);
                        radio.read(&senderId, sizeof(senderId));
                        Serial.println(senderId);
                        Serial.println(ackData);
                        radio.writeAckPayload(1, &ackData, sizeof(ackData));
                        Serial.print("Reading radio");

                }
        } else {
                //Serial.println("No connection");
                //senderId = 0;
                digitalWrite(LEDpin1, HIGH);
                digitalWrite(LEDpin2, LOW);
                //radio.startListening();
        }



        if (senderId == ValTest)     //Test ID
        {
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
              testingloop = true;

        }     //Test Stop

        //If the id is for this target
        if (senderId == ValSig)
        {

    #ifdef SENSOR1
                transmitterId = 17;     //This id is for target one
    #endif
    #ifdef SENSOR2
                transmitterId = 27;     //This id is for target two
    #endif
    #ifdef SENSOR3
                transmitterId = 37;     //This id is for target three
    #endif
    #ifdef SENSOR4
                transmitterId = 47;     //This id is for target four
    #endif

                digitalWrite(LEDpin, HIGH);
                Serial.println("Target ID ");
                Serial.print(senderId);
                Serial.println(" On , ");
                knock = true;
                knockisok = false;
                delay(20);
                //Run while loop until we registrate the hit

        }     //Sender ID stop

        if (senderId == nockData) {
                //Incase it goes to listening and we get the ackMessg for Transmitting shot.
                Serial.println("Knock Active");
                knockisok = true;
        }

        while (testingloop == true)
        {
        //This loop holds the test signal.
        delay(5);
        radio.stopListening();
        testloop = testloop + 1;
        secloop = secloop + 1;
        Serial.print("Loop run: ");
        Serial.println(testloop);
        Serial.print("Secure Loop: ");
        Serial.println(secloop);
        if (radio.write(&transmitterId, sizeof(transmitterId))) {
                        digitalWrite(LEDpin1, LOW); //Transmitter is workin, stop the red light
                        digitalWrite(LEDpin2, HIGH); //Transmitter is workin, start green light
                        //delay(5);
                        if ( radio.isAckPayloadAvailable() ) {
                                radio.read(&ackMessg, sizeof(ackMessg));
                                if (ackMessg == ackData2) {
                                        radio.startListening();
                                        Serial.println("Sending back ok");
                                        senderId = 0;
                                        ackMessg = 0;
                                        testloop = 0;
                                        secloop = 0;
                                        checkok = true;
                                        testingloop = false;
                                }
                        }
                } else {
                        //Serial.println("Failed Sending");
                        digitalWrite(LEDpin1, HIGH);
                        digitalWrite(LEDpin2, LOW);
                }

                while (secloop >= secrun) {
                  //This loop is to get back out to the start again incase it stucks.
                  radio.powerDown();
                  delay(50);
                  radio.powerUp();
                  radio.setPALevel(RF24_PA_LOW);
                  radio.setDataRate(RF24_1MBPS);
                  radio.setRetries(15, 15);
                  radio.setChannel(120); //Sending on channel 120
                  radio.setPayloadSize(8);
                  radio.stopListening();
                  radio.openWritingPipe(pipes[1]);
                  radio.openReadingPipe(1, pipes[0]);
                  radio.enableAckPayload();
                  Serial.println("Loop run max");
                  radio.startListening();
                  senderId = 0;
                  ackMessg = 0;
                  testloop = 0;
                  secloop = 0;
                  //checkok = true;
                  testingloop = false;
                  break;
                }

                while (testloop >= looprun) {
                        radio.powerDown();
                        delay(50);
                        radio.powerUp();
                        radio.setPALevel(RF24_PA_LOW);
                        radio.setDataRate(RF24_1MBPS);
                        radio.setRetries(15, 15);
                        radio.setChannel(120); //Sending on channel 120
                        radio.setPayloadSize(8);
                        radio.stopListening();
                        radio.openWritingPipe(pipes[1]);
                        radio.openReadingPipe(1, pipes[0]);
                        radio.enableAckPayload();
                        Serial.println("Loop run max");
                        radio.startListening();
                        testloop = 0;
                }

        }


        //Make a new loop for sending the ok test
        while (knock == true)
        {
                //We keep it in this loop to wait for the knock sensor to go High. Then we start the trasmitting loop.
                int val = analogRead(knockSensor);
                if (digitalRead(knockSensor) == HIGH)
                {
                        Serial.println("Knock");
                        digitalWrite(LEDpin, LOW);       //Knock is made stop light
                        knock = false;       //We got the knock, exit that loop
                        knockisok = true;       //The hit is ok, go to the sending loop
                }

        }

        while (knockisok == true)
        {
                delay(5);
                radio.stopListening();
                shootloop = shootloop + 1;
                secloop = secloop +1;
                Serial.print("Loop: ");
                Serial.println(shootloop);
                Serial.print("Secure Loop: ");
                Serial.println(secloop);

                //This loop is to send the knock OK signal
                //Serial.println("Knock sending loop");


                //  delay(50);
                if (radio.write(&transmitterId, sizeof(transmitterId))) {
                        digitalWrite(LEDpin1, LOW); //Transmitter is workin, stop the red light
                        digitalWrite(LEDpin2, HIGH); //Transmitter is workin, start green light
                        //delay(5);
                        if ( radio.isAckPayloadAvailable() ) {
                                radio.read(&ackMessg, sizeof(ackMessg));
                                if (ackMessg == nockData) {
                                        radio.startListening();
                                        Serial.println("Sending back ok");
                                        senderId = 0;
                                        ackMessg = 0;
                                        shootloop = 0;
                                        secloop = 0;
                                        knockisok = false;
                                        skjuter = false;
                                }
                        }
                } else {
                        //Serial.println("Failed Sending");
                        digitalWrite(LEDpin1, HIGH);
                        digitalWrite(LEDpin2, LOW);
                }

                //radio.startListening();

                while (secloop >= secrun) {
                  //This loop is to get back out to the start again incase it stucks.
                  radio.powerDown();
                  delay(50);
                  radio.powerUp();
                  radio.setPALevel(RF24_PA_LOW);
                  radio.setDataRate(RF24_1MBPS);
                  radio.setRetries(15, 15);
                  radio.setChannel(120); //Sending on channel 120
                  radio.setPayloadSize(8);
                  radio.stopListening();
                  radio.openWritingPipe(pipes[1]);
                  radio.openReadingPipe(1, pipes[0]);
                  radio.enableAckPayload();
                  Serial.println("Loop run max");
                  radio.startListening();
                  senderId = 0;
                  ackMessg = 0;
                  shootloop = 0;
                  secloop = 0;
                  knockisok=false;
                  break;
                }

                while (shootloop >= looprun) {
                        radio.powerDown();
                        delay(50);
                        radio.powerUp();
                        radio.setPALevel(RF24_PA_LOW);
                        radio.setDataRate(RF24_1MBPS);
                        radio.setRetries(15, 15);
                        radio.setChannel(120); //Sending on channel 120
                        radio.setPayloadSize(8);
                        radio.stopListening();
                        radio.openWritingPipe(pipes[1]);
                        radio.openReadingPipe(1, pipes[0]);
                        radio.enableAckPayload();
                        Serial.println("Loop run max");
                        radio.startListening();
                        shootloop = 0;
                }



        }       //While loop stop

        if (checkok == true)
        {
                checkok = false;
                doCheck();
                Serial.println("Leave Check OK");
        }     //Stop Check OK
}

void doCheck()
{
        Serial.println("Start Lamps");
        for (int lop = 0; lop < 20; lop++)
        {
                theDelay += 10;
                if (theDelay > 500) theDelay = 1;
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

        for (int lop = 0; lop < 20; lop++)
        {
                theDelay += 10;
                if (theDelay > 500) theDelay = 1;

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



} //End StartSensor()
