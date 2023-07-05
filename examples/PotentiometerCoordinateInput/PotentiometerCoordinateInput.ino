#include <BlendixSerial.h>

BlendixSerial blendix;

const int potPinX = A0; // Analog pin connected to the X-axis potentiometer
const int potPinY = A1; // Analog pin connected to the Y-axis potentiometer
const int potPinZ = A2; // Analog pin connected to the Z-axis potentiometer

void setup() {
  Serial.begin(9600);
  pinMode(potPinX, INPUT);
  pinMode(potPinY, INPUT);
  pinMode(potPinZ, INPUT);
}

void loop() {
  // Read values from the potentiometers
  int x = analogRead(potPinX);
  int y = analogRead(potPinY);
  int z = analogRead(potPinZ);

  // Map the potentiometer values to the desired range (e.g., 0-20)
  int xMapped = map(x, 0, 1023, 0, 20);
  int yMapped = map(y, 0, 1023, 0, 20);
  int zMapped = map(z, 0, 1023, 0, 20);

  // Setting coordinates for set 1
  blendix.setCoordinates(1, xMapped, yMapped, zMapped);

  // Setting text
  blendix.setText("Potentiometer Coordinates");

  // Getting the formatted output
  String Output = blendix.getFormattedOutput();

  // Printing the formatted output
  Serial.println(Output);

  delay(500);
}
