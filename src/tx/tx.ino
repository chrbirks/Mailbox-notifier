/*
 * Arduino Nano V3.0, ATmega328, 5 V
 * COM 5
 * Radio pins 7, 8
*/

//#include <LowPower.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//RF24 radio(7, 8);
RF24 radio(9, 10);

const byte rxAddr[6] = "00001";
const int interruptPin = 3;

volatile boolean triggered = false;

void setup()
{
  pinMode(interruptPin, INPUT);

  //while (!Serial);
  Serial.begin(9600);

  attachInterrupt(interruptPin, wakeUp, RISING); // TODO: should be falling?

  radio.begin();
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_LOW);
  //radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(rxAddr);

  radio.stopListening();
}

void wakeUp() {
  triggered = true;
}

void loop()
{

  Serial.println("Waiting for interrupt...");
  //if (triggered) {
  if (true) { // TODO: remove after debugging
    Serial.println("Interrupted");
    unsigned long firstTxTime = micros();
    Serial.print("firstTxTime=");
    Serial.print(firstTxTime);
    Serial.println("");
    /*
       Transmit trigger
    */

    boolean ackReceived = false;
    while (!ackReceived) {

      /*
         Transmit data
      */
      radio.stopListening();
      unsigned long txTime = micros();
      transmit(firstTxTime);

      /*
         Listen and wait for ack
      */
      radio.startListening();

      boolean timeout = false;
      while (!radio.available()) {
        //Serial.print("Elapsed time = ");
        //Serial.println(micros() - txTime);
        if (micros() - txTime > 5000000) { // 5 seconds
          timeout = true;
          break;
        }
      }

      if (timeout) {
        Serial.println(F("Failed to get ack. Retrying..."));
      } else {
        /*
           Receive ack
        */
        Serial.println("Got data");
        unsigned long rxTime;
        radio.read(&rxTime, sizeof(unsigned long));
        if (rxTime == firstTxTime) {
          Serial.print(F("Got ack "));
          Serial.println(rxTime);

          ackReceived = true;
          triggered = false;
        }
      } // if timeout else
      delay(500);
    } // while ackReceived
    Serial.println("************************************");
  } // if triggered
}

void transmit(unsigned long txTime) {
  if (!radio.write(&txTime, sizeof(unsigned long))) {
    Serial.println(F("Failed to send"));
  } else {
    Serial.print(F("Sent data "));
    Serial.println(txTime);
  }
}
