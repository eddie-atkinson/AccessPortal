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
  String getStudentDetails(int* byteArray, String* detail);
  bool checkPermission(char* permission);
  void getByteArray(char* uidInput, int* uidOutput);
private:
  char* getPermissions(char* userToken);
  WiFiClientSecure connect();
  String detailsSplitter(String line, String* details);
  char* _ssid;
  char* _password;
  String _token;
};

#endif
