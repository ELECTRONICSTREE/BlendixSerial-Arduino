
/*

Thank you for using the BlendixSerial library! This library was created to
  simplify the handling and transmission of multiple coordinate sets along with
  an optional text message over serial communication in Arduino-based projects.
  
  The library supports both sending and receiving data. For transmission, you can
  set up your coordinate sets (either as integers or floats) and an optional text
  message. The getFormattedOutput() method will combine all your transmit data into
  a single formatted string that looks like:
  
      "x,y,z,x,y,z,...;YourText"
  
  For receiving data, the library expects the incoming data string to be in a specific
  format: a comma-separated list of coordinate values that ends with a semicolon. For
  example:
  
      "10,20,30,40,50,60;"
  
  This string will be parsed into coordinate sets (each set comprising an x, y, and z
  value) using the parseReceivedData() function. Once parsed, you can check how many
  sets were received with getReceivedNumSets() and retrieve each set using
  getReceivedCoordinates(index, x, y, z).
  
  Feel free to modify, extend, or contribute to the library. If you discover any bugs or
  have suggestions, please let me know!
  
  Author: Usman
  Maintainer: Usman https://github.com/ELECTRONICSTREE/BlendixSerial-Arduino
  Date: 8-Mar-2025



# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

*/





#ifndef blendixserial_h
#define blendixserial_h

#include <Arduino.h>

// If not already defined, set a default maximum number of coordinate sets
#ifndef BLENDIX_MAX_SETS
#define BLENDIX_MAX_SETS 5
#endif

// If not already defined, set a default text buffer size
#ifndef BLENDIX_TEXT_BUFFER_SIZE
#define BLENDIX_TEXT_BUFFER_SIZE 50
#endif

// String constants for coordinate type selection
#define COORD_TYPE_INT "int"
#define COORD_TYPE_FLOAT "float"

/**
 * @class blendixserial
 * 
 * This class provides functionality to manage and transmit coordinate data (either int or float)
 * and an optional text buffer over serial. It also allows parsing of incoming coordinate data.
 */
class blendixserial {
private:
  /**
   * CoordinateType
   * - Used internally to keep track of whether we're storing coordinates as int or float.
   */
  enum CoordinateType { INT_TYPE, FLOAT_TYPE };

  /**
   * CoordinatesInt
   * - Structure to store one set of integer coordinates (x, y, z).
   */
  struct CoordinatesInt {
    int x;
    int y;
    int z;
  };

  /**
   * CoordinatesFloat
   * - Structure to store one set of floating-point coordinates (x, y, z).
   */
  struct CoordinatesFloat {
    float x;
    float y;
    float z;
  };

  /**
   * ReceivedCoordinates
   * - Structure used to store received coordinates in float format.
   */
  struct ReceivedCoordinates {
    float x;
    float y;
    float z;
  };

  // Pointer to an array of coordinate structures (either int or float), allocated at runtime
  void* coordinates;

  // Indicates whether we're currently storing int or float coordinates
  CoordinateType coordType;

  // How many coordinate sets we are transmitting
  int numSets;

  // Pointer to a text buffer used for storing an optional string
  char* text;

  // The allocated size of the text buffer
  size_t textBufferSize;

  // How many coordinate sets we expect (or are allowed) to receive
  int receiveSets;

  // Pointer to an array of float-based structures for received coordinates
  ReceivedCoordinates* receivedCoordinates;

  // How many sets were actually received
  int receivedSets;

  /**
   * @brief setCoordinateTypeInternal
   * Internal helper to switch between storing int or float coordinates.
   * If the coordinate type changes, it re-allocates the `coordinates` array
   * and resets all coordinates to zero.
   * 
   * @param type The new coordinate type (INT_TYPE or FLOAT_TYPE).
   * @return true if the type was changed, false if the type remains the same.
   */
  bool setCoordinateTypeInternal(CoordinateType type);

  /**
   * @brief validateAndParseData
   * Internal function to parse incoming data (in CSV-like format) into a temporary array
   * of float coordinates. The parsed data is later stored in `receivedCoordinates`.
   * 
   * @param inputData A C-style string containing comma-separated and semicolon-separated values.
   * @param tempCoords Reference to a pointer that will receive the newly allocated array of coordinates.
   * @param tempNumSets Reference to an integer that will receive the number of parsed sets.
   * @return true if parsing was successful, false otherwise.
   */
  bool validateAndParseData(const char* inputData, ReceivedCoordinates*& tempCoords, int& tempNumSets);

public:
  /**
   * @brief Constructor
   * Initializes default values, allocates arrays, and sets up the text buffer.
   */
  blendixserial();

  /**
   * @brief setCoordinateType
   * Public function to switch coordinate storage type via string ("int" or "float").
   * 
   * @param type The string indicating the coordinate type ("int" or "float").
   * @return true if the coordinate type was changed or successfully set, false otherwise.
   */
  bool setCoordinateType(const char* type);

  /**
   * @brief setTxSets
   * Sets how many coordinate sets this library will transmit (Tx).
   * 
   * @param sets The number of sets to transmit.
   * @return true if successfully set, false otherwise.
   */
  bool setTxSets(int sets);

  /**
   * @brief setCoordinates (int version)
   * Stores integer coordinates for a specific set index (1-based).
   * 
   * @param setNum The index of the coordinate set (1-based).
   * @param xVal, yVal, zVal The integer coordinates to store.
   * @return true if successful, false otherwise (e.g., out-of-range setNum or wrong coord type).
   */
  bool setCoordinates(int setNum, int xVal, int yVal, int zVal);

  /**
   * @brief setCoordinates (float version)
   * Stores float coordinates for a specific set index (1-based).
   * 
   * @param setNum The index of the coordinate set (1-based).
   * @param xVal, yVal, zVal The float coordinates to store.
   * @return true if successful, false otherwise.
   */
  bool setCoordinates(int setNum, float xVal, float yVal, float zVal);

  /**
   * @brief resetCoordinates
   * Resets all allocated coordinate sets to zero (int or float).
   */
  void resetCoordinates();

  /**
   * @brief setText
   * Stores a C-style string into the text buffer.
   * 
   * @param inputText The string to copy into the buffer.
   */
  void setText(const char* inputText);

  /**
   * @brief getFormattedOutput
   * Formats the stored coordinates plus the text buffer into a single string.
   * For example: "x,y,z,x,y,z;someText".
   * 
   * @param outputBuffer The buffer to hold the formatted string (cast as uint8_t* for Arduino).
   * @param bufferSize The size of the output buffer.
   */
  void getFormattedOutput(uint8_t* outputBuffer, size_t bufferSize);

  /**
   * @brief setRxSets
   * Sets how many coordinate sets this library will receive (Rx).
   * 
   * @param sets The number of sets to receive.
   * @return true if successfully set, false otherwise.
   */
  bool setRxSets(int sets);

  /**
   * @brief parseReceivedData
   * Takes an Arduino String of data (like "10,20,30;Hello;"), checks format, and stores
   * the coordinates in `receivedCoordinates`. Also updates how many sets were received.
   * 
   * @param inputData The incoming data string.
   * @return true if parsing was successful, false otherwise.
   */
  bool parseReceivedData(const String& inputData);

  /**
   * @brief getReceivedNumSets
   * Returns how many coordinate sets were actually parsed from the last received data.
   * 
   * @return Number of received sets.
   */
  int getReceivedNumSets() const;

  /**
   * @brief getReceivedCoordinates
   * Retrieves one set of received coordinates by index (0-based).
   * 
   * @param index The zero-based index of the received set.
   * @param x, y, z Reference variables to store the retrieved coordinates.
   * @return true if valid index, false otherwise.
   */
  bool getReceivedCoordinates(int index, float& x, float& y, float& z) const;
};

#endif

