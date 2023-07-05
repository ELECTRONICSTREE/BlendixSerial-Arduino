#include <BlendixSerial.h>

BlendixSerial blendix;

void setup() {
  Serial.begin(9600);
}

void loop() {
 
  // Setting coordinates for set 1
  blendix.setCoordinates(1, 10, 20, 30);
  
  // Setting text
  blendix.setText("Hello, Blendix!");
  
  // Getting the formatted output
  String Output = blendix.getFormattedOutput();
  
  // Printing the formatted output
  Serial.println(Output);
  
  delay(2000);
  
  // Resetting coordinates
  blendix.resetCoordinates();
  
  // Updating text
  blendix.setText("Reset and Update");
  
  // Getting the updated formatted output
  Output = blendix.getFormattedOutput();
  
  // Printing the updated formatted output
  Serial.println(Output);

  delay(2000);
}