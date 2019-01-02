# Access Portal Library
![](logo.png)
> An Arduino library for interacting with the UWA Makers Access Portal using RFID strings from student cards to access student information and tool permissions. 


UWA Makers uses its own home-cooked Access Portal to manage user sign-ups, tool permissions and room access. Until now there hasn't been an easy way to interact with the Access Portal to get user information. This has limited the potential for automation and utility projects within UWA Makers. 

This library attempts to improve this situation by making it easy for members to use RFID readers in a variety of projects and use  them to get user information from the Access Portal. 

This project is designed to work with the ESP8266 microcontroller. For more information on how to install the ESP8266 board manager and libraries see the [official github page](https://github.com/esp8266/Arduino) for the project. 


## Installation

Simply download the library and create a file called AccessPortal in:

```sh
Arduino/libraries/
```
In this file extract AccessPortal.cpp and AccessPortal.h

Update your Arduino libraries by clicking on Sketch>Include Library>Manage Libraries and allowing it to update. 

## Dependencies

This library has the following the dependencies: 

* ESP8266WiFi.h
* WiFiClientSecure.h
* ArduinoJson.h

The ESP8266WiFi.h and WiFiClientSecure.h libraries are automatically installed when the ESP8266 board manager is installed. 

The ArduinoJson library can be installed from the Arduino IDE library manager, or from the projects github [here](https://github.com/bblanchon/ArduinoJson).

## Usage example

For an example of how to use this library see the Simple RFID Permissions [example](https://github.com/e-spatula/AccessPortal/blob/master/Examples/RFID_Permissions_Example.ino)


## Release History

* 0.0.1
    * initial release

## Meta

Eddie Atkinson - safety@makeuwa.com

Distributed under the XYZ license. See ``LICENSE`` for more information.

[https://github.com/e-spatula/AccessPortal](https://github.com/e-spatula/AccessPortal)

