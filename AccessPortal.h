#ifndef accessportal_h
#define accesportal_h
#include "Arduino.h"
#include "stdlib.h"
#include "ESP8266WiFi.h"
#include "WiFiClientSecure.h"

class AccessPortal {
public:
  AccessPortal(char* ssid, char* password, String token);
  String getStudentDetails(int* byteArray, char* detail);
  bool checkPermission(char* permission);
  WiFiClientSecure connect();
  void getByteArray(char* uidInput, int* uidOutput);
private:
  char* getPermissions(char* userToken);
  char* _ssid;
  char* _password;
  String _token;
};

#endif
