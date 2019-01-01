#ifndef accessportal_h
#define accesportal_h
#include "Arduino.h"
#include "stdlib.h"
#include "ESP8266WiFi.h"
#include "WiFiClientSecure.h"
#include "ArduinoJson.h"

class AccessPortal {
public:
  AccessPortal(char* ssid, char* password, String token);
  bool getStudentDetails(int* byteArray, String* detail);
  bool checkPermission(char* permission);
  bool getByteArray(String uidInput, int* uidOutput);
  bool checkPermission(const char* permission, String userToken);
private:
  char* getPermissions(char* userToken);
  WiFiClientSecure connect();
  bool detailsSplitter(String line, String details[6]);
  char* _ssid;
  char* _password;
  String _token;
};

#endif
