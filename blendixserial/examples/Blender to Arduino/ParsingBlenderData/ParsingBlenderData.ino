/*

Parsing Blender 3D Data -  Arduino Sketch

  Author: Usman 
  Date: 5-MAR-2025
  Website: www.electronicstree.com
  Email: help@electronicstree.com

Blender to Arduino : Receives and parses coordinate data from blender via serial.

This program listens for incoming serial data and attempts to extract coordinate values from it.
The blendixserial library parses the incoming data, validates it, and stores it in an array of
received coordinates. The receiver extracts each (x, y, z) coordinate set and displays them on
the Serial Monitor for verification.

This code is intended for Arduino boards with a single hardware serial port
, such as the Arduino Uno, Nano, and similar models. The hardware serial port
is used to communicate with Blender for sending and receiving data. Since 
both the Uno and Nano have only one serial port (pins 0 & 1), which is shared
with the USB connection to Blender, you can't use the Serial Monitor or pins 
0 & 1 for other communications. To enable debugging, the SoftwareSerial library
is utilized, allowing you to create additional serial ports on the Arduino’s 
digital pins. Additionally, by using a USB-to-TTL converter, you can add 
another serial port to an Arduino Uno or Nano, enabling communication with the PC .

  For guidance on using a USB-to-TTL converter with Arduino, you can read the following article:
  https://electronicstree.com/usb-to-ttl-with-arduino-uno-r3/


Note:
SoftwareSerial can be unreliable at higher baud rates and can interfere with other 
timing-sensitive operations. If you need a more reliable serial connection, consider 
using a hardware serial port (if available) or a different serial library.

 If you encounter any errors or bugs while using the blendixserial library or this code,
  please feel free to report them. Your feedback is valuable for improvement!

 Thank you for your help!


*/

#include <blendixserial.h>
#include <SoftwareSerial.h>


SoftwareSerial debugSerial(10, 11);  // RX, TX
blendixserial blendix;

void setup() {
  Serial.begin(9600); // Start serial communication
  blendix.setRxSets(3); // Set expected number of received sets
  debugSerial.begin(9600);
}

void loop() {
  static String serialData = ""; // Buffer to accumulate incoming serial data

  while (Serial.available()) {
    char receivedChar = Serial.read(); // Read one character at a time
    serialData += receivedChar; // Append the character to the buffer

    // Attempt to parse the data directly whenever something is received
    if (blendix.parseReceivedData(serialData)) {
      printReceivedCoordinates(); // Print received coordinates
      serialData = ""; // Clear the buffer for the next data set
    }
  }
}

void printReceivedCoordinates() {
  int numSets = blendix.getReceivedNumSets();
  for (int i = 0; i < numSets; i++) {
    float x, y, z;
    if (blendix.getReceivedCoordinates(i, x, y, z)) {
      debugSerial.print("Set ");
      debugSerial.print(i + 1);
      debugSerial.print(": X=");
      debugSerial.print(x);
      debugSerial.print(", Y=");
      debugSerial.print(y);
      debugSerial.print(", Z=");
      debugSerial.println(z);
    } else {
      debugSerial.print("Failed to retrieve coordinates for set ");
      debugSerial.println(i + 1);
    }
  }
}
