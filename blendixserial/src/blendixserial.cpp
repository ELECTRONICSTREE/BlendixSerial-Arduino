
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



#include "blendixserial.h"

/**
 * @brief Constructor
 * Initializes default values, sets coordinate type to int by default,
 * allocates arrays for coordinates and received data, and sets up the text buffer.
 */
blendixserial::blendixserial()
    : numSets(1),            // Default to 1 transmit set
      receiveSets(0),        // Default to 0 receive sets
      textBufferSize(BLENDIX_TEXT_BUFFER_SIZE),
      coordinates(nullptr),
      coordType(INT_TYPE),
      text(nullptr),
      receivedCoordinates(nullptr),
      receivedSets(0)
{
  // Allocate memory for integer-based coordinates (up to BLENDIX_MAX_SETS)
  coordinates = new CoordinatesInt[BLENDIX_MAX_SETS];

  // Allocate memory for the receivedCoordinates array (always float-based)
  receivedCoordinates = new ReceivedCoordinates[BLENDIX_MAX_SETS];

  // Initialize coordinate values to zero
  resetCoordinates();

  // Allocate memory for the text buffer and set it to an empty string
  text = new char[BLENDIX_TEXT_BUFFER_SIZE];
  text[0] = '\0';
}

/**
 * @brief setCoordinateTypeInternal
 * Internal helper to switch between int or float coordinate storage.
 * If the type changes, we deallocate the old array and allocate a new one,
 * then reset all values to zero.
 *
 * @param type The new coordinate type (INT_TYPE or FLOAT_TYPE).
 * @return true if the type was changed, false if it was already the same.
 */
bool blendixserial::setCoordinateTypeInternal(CoordinateType type) {
  // Only proceed if the new type is different
  if (type != coordType) {
    // Free the memory from the old coordinate array
    delete[] static_cast<CoordinatesInt*>(coordinates);

    // Update the coordinate type
    coordType = type;

    // Allocate a new array based on the updated type
    if (coordType == INT_TYPE) {
      coordinates = new CoordinatesInt[BLENDIX_MAX_SETS];
    } else {
      coordinates = new CoordinatesFloat[BLENDIX_MAX_SETS];
    }

    // Reset all coordinate values to zero
    resetCoordinates();
    return true;
  }
  return false; // No change needed
}

/**
 * @brief setCoordinateType
 * Public-facing method to switch coordinate storage between "int" or "float".
 *
 * @param type A string that should be either "int" or "float".
 * @return true if successful, false otherwise.
 */
bool blendixserial::setCoordinateType(const char* type) {
  if (strcmp(type, COORD_TYPE_INT) == 0) {
    return setCoordinateTypeInternal(INT_TYPE);
  } else if (strcmp(type, COORD_TYPE_FLOAT) == 0) {
    return setCoordinateTypeInternal(FLOAT_TYPE);
  }
  return false; // Invalid string
}

/**
 * @brief setTxSets
 * Sets how many coordinate sets will be transmitted.
 * Ensures total sets (Tx + Rx) don't exceed BLENDIX_MAX_SETS.
 *
 * @param sets Number of transmit sets.
 * @return true if valid and updated, false otherwise.
 */
bool blendixserial::setTxSets(int sets) {
  if (sets < 0) {
    return false;
  }
  // Check combined sets
  if (sets + receiveSets > BLENDIX_MAX_SETS) {
    return false;
  }
  numSets = sets;
  return true;
}

/**
 * @brief setRxSets
 * Sets how many coordinate sets will be received.
 * Ensures total sets (Rx + Tx) don't exceed BLENDIX_MAX_SETS.
 *
 * @param sets Number of receive sets.
 * @return true if valid and updated, false otherwise.
 */
bool blendixserial::setRxSets(int sets) {
  if (sets < 0) {
    return false;
  }
  // Check combined sets
  if (sets + numSets > BLENDIX_MAX_SETS) {
    return false;
  }
  receiveSets = sets;
  return true;
}

/**
 * @brief setCoordinates (int version)
 * Assigns integer x, y, z values to a specific transmit set (1-based index).
 *
 * @param setNum The set index (1-based).
 * @param xVal, yVal, zVal The integer coordinates to store.
 * @return true if successful, false otherwise (e.g. out-of-range or wrong coord type).
 */
bool blendixserial::setCoordinates(int setNum, int xVal, int yVal, int zVal) {
  // Ensure the setNum is within range and we're using int-based coordinates
  if (setNum >= 1 && setNum <= numSets && coordType == INT_TYPE) {
    auto coords = static_cast<CoordinatesInt*>(coordinates);
    coords[setNum - 1].x = xVal;
    coords[setNum - 1].y = yVal;
    coords[setNum - 1].z = zVal;
    return true;
  }
  return false;
}

/**
 * @brief setCoordinates (float version)
 * Assigns floating-point x, y, z values to a specific transmit set (1-based index).
 *
 * @param setNum The set index (1-based).
 * @param xVal, yVal, zVal The float coordinates to store.
 * @return true if successful, false otherwise.
 */
bool blendixserial::setCoordinates(int setNum, float xVal, float yVal, float zVal) {
  // Ensure the setNum is within range and we're using float-based coordinates
  if (setNum >= 1 && setNum <= numSets && coordType == FLOAT_TYPE) {
    auto coords = static_cast<CoordinatesFloat*>(coordinates);
    coords[setNum - 1].x = xVal;
    coords[setNum - 1].y = yVal;
    coords[setNum - 1].z = zVal;
    return true;
  }
  return false;
}

/**
 * @brief resetCoordinates
 * Resets all transmit coordinates (up to numSets) to 0 or 0.0 depending on the type.
 */
void blendixserial::resetCoordinates() {
  if (coordType == INT_TYPE) {
    // Cast to int array
    auto coords = static_cast<CoordinatesInt*>(coordinates);
    for (int i = 0; i < numSets; i++) {
      coords[i].x = 0;
      coords[i].y = 0;
      coords[i].z = 0;
    }
  } else {
    // Cast to float array
    auto coords = static_cast<CoordinatesFloat*>(coordinates);
    for (int i = 0; i < numSets; i++) {
      coords[i].x = 0.0f;
      coords[i].y = 0.0f;
      coords[i].z = 0.0f;
    }
  }
}

/**
 * @brief setText
 * Copies a user-supplied string into the text buffer for optional usage in formatted output.
 *
 * @param inputText Pointer to a C-style string.
 */
void blendixserial::setText(const char* inputText) {
  if (text && inputText) {
    strncpy(text, inputText, textBufferSize - 1);
    text[textBufferSize - 1] = '\0'; // Ensure null termination
  }
}

/**
 * @brief getFormattedOutput
 * Creates a single output string containing the transmit coordinates followed by
 * a semicolon and the optional text. Example: "10,20,30,40,50,60;Hello".
 *
 * @param outputBuffer Pointer to the buffer that will hold the result string.
 * @param bufferSize The capacity of outputBuffer.
 */
void blendixserial::getFormattedOutput(uint8_t* outputBuffer, size_t bufferSize) {
  // If no valid buffer or size is zero, do nothing
  if (!outputBuffer || bufferSize == 0) return;

  // Keep track of our position in outputBuffer
  size_t offset = 0;

  // Temporary buffer for string conversions (size depends on int vs float)
  size_t tempBufferSize = (coordType == INT_TYPE) ? 12 : 16;
  char* tempBuffer = new char[tempBufferSize];

  // Format coordinates
  if (coordType == INT_TYPE) {
    // Cast the coordinates pointer to int-based structure
    auto coords = static_cast<CoordinatesInt*>(coordinates);
    for (int i = 0; i < numSets; i++) {
      // Convert X to string and append
      snprintf(tempBuffer, tempBufferSize, "%d", coords[i].x);
      offset += snprintf((char*)outputBuffer + offset, bufferSize - offset, "%s,", tempBuffer);

      // Convert Y to string and append
      snprintf(tempBuffer, tempBufferSize, "%d", coords[i].y);
      offset += snprintf((char*)outputBuffer + offset, bufferSize - offset, "%s,", tempBuffer);

      // Convert Z to string and append
      snprintf(tempBuffer, tempBufferSize, "%d", coords[i].z);
      offset += snprintf((char*)outputBuffer + offset, bufferSize - offset, "%s", tempBuffer);

      // Add comma separator if not the last set
      if (i < numSets - 1 && offset < bufferSize - 1) {
        outputBuffer[offset++] = ',';
      }
    }
  } else {
    // Cast the coordinates pointer to float-based structure
    auto coords = static_cast<CoordinatesFloat*>(coordinates);
    for (int i = 0; i < numSets; i++) {
      // Convert X to string with 2 decimal places
      dtostrf(coords[i].x, 0, 2, tempBuffer);
      offset += snprintf((char*)outputBuffer + offset, bufferSize - offset, "%s,", tempBuffer);

      // Convert Y to string with 2 decimal places
      dtostrf(coords[i].y, 0, 2, tempBuffer);
      offset += snprintf((char*)outputBuffer + offset, bufferSize - offset, "%s,", tempBuffer);

      // Convert Z to string with 2 decimal places
      dtostrf(coords[i].z, 0, 2, tempBuffer);
      offset += snprintf((char*)outputBuffer + offset, bufferSize - offset, "%s", tempBuffer);

      // Add comma separator if not the last set
      if (i < numSets - 1 && offset < bufferSize - 1) {
        outputBuffer[offset++] = ',';
      }
    }
  }

  // Append semicolon and text at the end, if there's space
  if (offset < bufferSize - 1) {
    snprintf((char*)outputBuffer + offset, bufferSize - offset, ";%s", text ? text : "");
  }

  // Clean up
  delete[] tempBuffer;
  // Ensure the output is null-terminated
  outputBuffer[bufferSize - 1] = '\0';
}

/**
 * @brief validateAndParseData
 * Internal helper to tokenize the incoming data string by commas and semicolons,
 * convert them to floats, and allocate a temporary array of ReceivedCoordinates.
 *
 * @param inputData The incoming string data (e.g. "10,20,30,40,50,60;SomeText").
 * @param tempCoords A reference to a pointer that will store the newly allocated ReceivedCoordinates array.
 * @param tempNumSets An integer reference that will store how many sets were parsed.
 * @return true if parsing is valid and successful, false otherwise.
 */
bool blendixserial::validateAndParseData(const char* inputData, ReceivedCoordinates*& tempCoords, int& tempNumSets) {
  if (!inputData) return false;

  // Make a copy for safe tokenization
  char* dataCopy = strdup(inputData);
  // Tokenize by commas and semicolons
  char* token = strtok(dataCopy, ",;");

  // Allocate an array for float values (up to receiveSets * 3, since each set has 3 coords)
  float* values = new float[receiveSets * 3];
  int valueIndex = 0;

  // Parse tokens
  while (token) {
    // If we exceed our expected max, stop
    if (valueIndex >= receiveSets * 3) {
      break;
    }
    // Convert token to float
    values[valueIndex++] = atof(token);
    // Move to the next token
    token = strtok(nullptr, ",;");
  }

  // Clean up temporary copy
  free(dataCopy);

  // Check if the total number of floats is a multiple of 3 (x, y, z)
  if (valueIndex % 3 != 0) {
    delete[] values;
    return false;
  }

  // Calculate how many sets were found
  tempNumSets = valueIndex / 3;

  // If the data has more sets than we can store, limit to receiveSets
  if (tempNumSets > receiveSets) {
    tempNumSets = receiveSets;
  }

  // Allocate a new array of ReceivedCoordinates
  tempCoords = new ReceivedCoordinates[tempNumSets];

  // Fill the temporary coordinate array
  for (int i = 0, coordIndex = 0; i < valueIndex && coordIndex < tempNumSets; i += 3, coordIndex++) {
    tempCoords[coordIndex].x = values[i];
    tempCoords[coordIndex].y = values[i + 1];
    tempCoords[coordIndex].z = values[i + 2];
  }

  // Free the float array
  delete[] values;

  return true;
}

/**
 * @brief parseReceivedData
 * Public function to parse a String (Arduino String) of incoming data.
 * Checks that the data ends with a semicolon, then calls validateAndParseData.
 * If successful, updates the receivedCoordinates array and the count of received sets.
 *
 * @param inputData The incoming data string.
 * @return true if parsing was successful, false otherwise.
 */
bool blendixserial::parseReceivedData(const String& inputData) {
  // Convert Arduino String to C-style string
  const char* inputCStr = inputData.c_str();

  // We expect the data to end with a semicolon
  if (inputCStr == nullptr || inputCStr[strlen(inputCStr) - 1] != ';') {
    return false;
  }

  // Temporary pointers for parsed data
  ReceivedCoordinates* tempCoords = nullptr;
  int tempNumSets = 0;

  // Attempt to parse
  if (validateAndParseData(inputCStr, tempCoords, tempNumSets)) {
    // If successful, free the old array and store the new one
    delete[] receivedCoordinates;
    receivedCoordinates = tempCoords;
    receivedSets = tempNumSets;
    return true;
  }
  return false;
}

/**
 * @brief getReceivedNumSets
 * Returns how many sets were parsed from the last call to parseReceivedData.
 *
 * @return The number of received sets.
 */
int blendixserial::getReceivedNumSets() const {
  return receivedSets;
}

/**
 * @brief getReceivedCoordinates
 * Retrieves the coordinates of a given set (by 0-based index) from the receivedCoordinates array.
 *
 * @param index Zero-based index of the set to retrieve.
 * @param x, y, z References to floats that will store the coordinates.
 * @return true if index is valid, false otherwise.
 */
bool blendixserial::getReceivedCoordinates(int index, float& x, float& y, float& z) const {
  // Check if the index is in range and we have a valid array
  if (index < 0 || index >= receivedSets || !receivedCoordinates) {
    return false;
  }

  // Copy the stored coordinates into the provided references
  x = receivedCoordinates[index].x;
  y = receivedCoordinates[index].y;
  z = receivedCoordinates[index].z;
  return true;
}
