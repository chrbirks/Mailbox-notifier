# Mailbox-notifier
Sender/receiver module for notification when mail has arrived.

TX module:
Based on an Arduino Nano V3.0 (5V) and a NRF24L01+ tranceiver. A mechanical switch detects when the mailbox has been opened and transmits a pule to the RX module.

RX module:
ased on an Arduino Nano V3.0 (5V) and a NRF24L01+ tranceiver for communication with the RX module and a ESP8266 WiFi module for internet communication.

TODO:
Integrate the ESP8266 to commuicate with the Blynk token "f35ff02badd24562bdcea753176d2042".
