/*
Parsing Blender 3D Data for Servo Control - Arduino Sketch  

  Author: Usman  
  Date: 8-MAR-2025  
  Website: www.electronicstree.com  
  Email: help@electronicstree.com  

Blender to Arduino: Receives and parses coordinate data for servo control.

This program listens for incoming serial data and attempts to extract coordinate values from it.  
The blendixserial library parses the incoming data, validates it, and stores it in an array of received coordinates.  
The receiver extracts each (x, y, z) coordinate set, displays them on the Serial Monitor for verification,  
and uses the Z-axis value to control a servo motor.

Servo Control:
- The Z-axis value (ranging from 0 to 180) is used to set the servo position.
- The servo position is updated only when Z changes to prevent unnecessary movement.
- A SoftwareSerial port is used for debugging without interfering with the main serial connection.

Hardware Requirements:
- Arduino Uno / Nano (or similar)
- Servo Motor (SG90, MG995, or compatible)
- blendixserial Library (for receiving Blender data)
- USB-to-TTL Converter (optional, for debugging)


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
SoftwareSerial can be unreliable at higher baud rates and may interfere with other timing-sensitive operations.  
If you need a more reliable serial connection, consider using a hardware serial port (if available) or a different serial library.

If you encounter any errors or bugs while using the blendixserial library or this code,  
please feel free to report them. Your feedback is valuable for improvement!

Thank you for your help!
*/

#include <blendixserial.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial debugSerial(10, 11);  // RX, TX for debugging
blendixserial blendix;
Servo myServo; // Create a Servo object

int lastServoPos = -1; // Store the last servo position (-1 ensures first update happens)

void setup() {
  Serial.begin(9600); // Start serial communication
  blendix.setRxSets(3); // Set expected number of received sets
  debugSerial.begin(9600);

  myServo.attach(9); // Attach the servo to pin 9 (change as needed)
}

void loop() {
  static String serialData = ""; // Buffer to accumulate incoming serial data

  while (Serial.available()) {
    char receivedChar = Serial.read(); // Read one character at a time
    serialData += receivedChar; // Append the character to the buffer

    // Attempt to parse the data whenever something is received
    if (blendix.parseReceivedData(serialData)) {
      processCoordinates(); // Extract Z value and use it
      serialData = ""; // Clear the buffer for the next data set
    }
  }
}

void processCoordinates() {
  int numSets = blendix.getReceivedNumSets();
  if (numSets > 0) { // Ensure there's at least one set of data
    float x, y, z;
    if (blendix.getReceivedCoordinates(0, x, y, z)) { // Get first object's coordinates
      int servoPos = constrain(z, 0, 180); // Ensure it's within the servo's range

      if (servoPos != lastServoPos) { // Only update if the value has changed
        myServo.write(servoPos); // Move the servo
        lastServoPos = servoPos; // Update last known position

        debugSerial.print("Servo moved to: ");
        debugSerial.println(servoPos);
      }
    }
  } else {
    debugSerial.println("No coordinate sets received.");
  }
}
