#ifndef BlendixSerial_h
#define BlendixSerial_h

#include <Arduino.h>

class BlendixSerial{
private:
  struct Coordinates {
    int x;
    int y;
    int z;
  };

  Coordinates coordinates[3];
  String text;

public:
  BlendixSerial();
  void resetCoordinates();
  void setCoordinates(int setNum, int xVal, int yVal, int zVal);
  void setText(String inputText);
  String getFormattedOutput();
};

#endif
