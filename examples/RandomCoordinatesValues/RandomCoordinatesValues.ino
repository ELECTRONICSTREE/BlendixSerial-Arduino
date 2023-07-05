#include <BlendixSerial.h>

BlendixSerial blendix;

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int setNum = 1; setNum <= 3; setNum++) {
    // Generate random fake coordinates
    int x = generateRandomValue();
    int y = generateRandomValue();
    int z = generateRandomValue();

    // Setting coordinates for the current set number
    blendix.setCoordinates(setNum, x, y, z);
  }

  // Setting text
  blendix.setText("Hello Blendix");

  // Getting the formatted output
  String Output = blendix.getFormattedOutput();

  // Printing the formatted output
  Serial.println(Output);

  delay(1000);
}

// Function to generate random fake values for the coordinates
int generateRandomValue() {
  return random(-20, 20); // Modify the range as per your requirements
}
