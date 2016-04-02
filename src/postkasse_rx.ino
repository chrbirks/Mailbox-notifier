/*
 * Arduino Pro Mini, ATmega328, 3.3 V, 8 MHz
 * COM 3
 * Radio pins 9, 10
 */
#include <LowPower.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);

const byte rxAddr[6] = "00001";

unsigned long lastReceivedData = 0;
short triggerCount = 0;

//int ledPin = 13;
//int ledState = LOW;

void setup()
{
  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, ledState);

  //while (!Serial);
  Serial.begin(9600);

  radio.begin();

  radio.setPALevel(RF24_PA_LOW);
  //radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(0, rxAddr);

  radio.startListening();
}

void loop() {

  //Serial.println("Waiting for data...");
  if (radio.available()) {
    while (radio.available()) {
      unsigned long data;

      /*
         Receive data
      */
      radio.read(&data, sizeof(unsigned long));
      Serial.print(F("Got data "));
      Serial.println(data);
      if (data != lastReceivedData) {
        lastReceivedData = data;
        triggerCount++;
        //ledState = HIGH;
        Serial.print(F("Got new data "));
        Serial.println(data);
      }
      Serial.print(F("triggerCount="));
      Serial.println(triggerCount);

      /*
         Send ack
      */
      radio.stopListening();
      Serial.println(data);
      if (!radio.write(&data, sizeof(unsigned long))) {
        Serial.println(F("Failed to send"));
      } else {
        Serial.println("Sent ack");
      }
      radio.startListening();
    }

  }

  //digitalWrite(ledPin, ledState);
}

