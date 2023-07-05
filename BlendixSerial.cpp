#include "BlendixSerial.h"

BlendixSerial::BlendixSerial() {
  resetCoordinates();
}

void BlendixSerial::resetCoordinates() {
  for (int i = 0; i < 3; i++) {
    coordinates[i].x = 0;
    coordinates[i].y = 0;
    coordinates[i].z = 0;
  }
}

void BlendixSerial::setCoordinates(int setNum, int xVal, int yVal, int zVal) {
  if (setNum >= 1 && setNum <= 3) {
    coordinates[setNum - 1].x = xVal;
    coordinates[setNum - 1].y = yVal;
    coordinates[setNum - 1].z = zVal;
  }
}

void BlendixSerial::setText(String inputText) {
  text = inputText;
}

String BlendixSerial::getFormattedOutput() {
  String output = "";
  for (int i = 0; i < 3; i++) {
    output += String(coordinates[i].x);
    output += ",";
    output += String(coordinates[i].y);
    output += ",";
    output += String(coordinates[i].z);
    if (i < 2) {
      output += ",";
    }
  }
  output += ";" + text;
  return output;
}
