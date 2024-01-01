

# BlendixSerial Arduino Library

The `BlendixSerial` library is designed to work in conjunction with the blendixserial addon in Blender. It allows for controlling objects in Blender using serial communication with an Arduino or other microcontrollers. The library provides functions to set coordinates and text for the objects in Blender, as well as retrieve a formatted output string. It provides a convenient way to manage coordinate data and text, making it easier to send structured information over a serial connection.

![Logo](https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEiGNnVIOoxRX6aHYeYyJ0QT1i-QVphdHVB9fmAdPQVGwlD4HLYs93XxYV8hMtnX7M0Fbh6QGxYiTzH3nqEpBvtBv-oQIS1FbCINqn-kJT9jQJuKrZRC8IeuqhR9G8-Qub3etiKLiZpXQWW7nBNOSh9DgJjOKsocKliayJhjwk3nYN6wXpbzSjopdccC/s16000/blendixserial.png)


## Installation

To use the `BlendixSerial` library, follow these steps:

1. Download the library files.
2. Extract the downloaded files.
3. Copy the entire extracted folder into the Arduino libraries folder. The default location for this folder is:
   - Windows: `Documents\Arduino\libraries`
   - macOS: `Documents/Arduino/libraries`
   - Linux: `Documents/Arduino/libraries`
4. Restart the Arduino IDE if it was already open.

## Usage

To use the `BlendixSerial` library in your Arduino sketch, follow these steps:

1. Include the library at the top of your sketch:

   ```cpp
   #include <BlendixSerial.h>
   ```

2. Create an instance of the `BlendixSerial` class:

   ```cpp
   BlendixSerial blendix;
   ```

3. Set the coordinates and text using the provided member functions:

   ```cpp
   blendix.setCoordinates(1, 10, 20, 30);
   blendix.setText("Hello, Blendix!");
   ```

4. Retrieve the formatted output using the `getFormattedOutput()` function:

   ```cpp
   String output = blendix.getFormattedOutput();
   ```

   The `output` string will contain the formatted coordinates and text data.

5. Use the `output` string as needed. For example, you can send it over a serial connection:

   ```cpp
   Serial.println(output);
   ```



### Additional Resources
For more information and examples, you can visit the [BlendixSerial Arduino Control documentation](https://electronicstree.com/arduino-library-for-blendixserial-addon/).


### `License`
The BlendixSerial Arduino Labrary is distributed under the GNU General Public License. Please refer to the license text for more details.
