/*
 Sending Data to Blender -  Arduino Sketch

  Author: Usman 
  Date: 5-MAR-2025
  Website: www.electronicstree.com
  Email: help@electronicstree.com


 Arduino to Blender : Sends Coordinate and Text Data to Blender via Serial.
 --------------------------------------------
 This Arduino code demonstrates how to use the blendixserial library to send
 coordinate data and a text message to a Blender addon via serial communication.
 The code initializes serial communication, configures the coordinate data type and 
 number of coordinate sets, and generates random floating-point coordinates. 
 These coordinates, along with a text message, are then formatted into a string using
 the blendixserial library and sent over the serial port.


 If you encounter any errors or bugs while using the blendixserial library or this code,
  please feel free to report them. Your feedback is valuable for improvement!

 Thank you for your help!


*/


#include "blendixserial.h"

blendixserial blendix;  // Create an instance of blendixserial

void setup() {
    Serial.begin(9600);  // Start Serial communication

    // Set coordinate type to FLOAT (or COORD_TYPE_INT for integers)
    blendix.setCoordinateType(COORD_TYPE_FLOAT);

    // Set number of transmitted coordinate sets
    blendix.setTxSets(3);

    // Seed random generator with an unpredictable value.
    randomSeed(analogRead(0));  
}

void loop() {
    // Generate random float values (scaled to a range, e.g., 0.0 to 100.0)
    for (int i = 1; i <= 3; i++) {
        float x = random(0, 100) / 10.0;  // Convert to float by dividing by 10
        float y = random(0, 100) / 10.0;
        float z = random(0, 100) / 10.0;
        
        blendix.setCoordinates(i, x, y, z);
    }

    // Set a dynamic text message
    blendix.setText("blendixserial blender addon");

    // Prepare and send formatted data
    uint8_t outputBuffer[100];
    blendix.getFormattedOutput(outputBuffer, sizeof(outputBuffer));

    Serial.println((char*)outputBuffer);

    // Send new random values every 1 second
    delay(1000);
}

