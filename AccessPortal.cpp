/* Access portal library */
#include "Arduino.h"
#include "AccessPortal.h"

const char* host = "auth.uwamakers.com";
const int httpsPort = 443;
const char* cardURL = "/api/card";
const char* permissionsURL = "/api/user/perms";

AccessPortal::AccessPortal(char* ssid, char* password, String token) {
  Serial.begin(115200);
  _ssid = ssid;
  _password = password;
  _token = token;
}

WiFiClientSecure AccessPortal::connect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _password);
  Serial.println("Connecting");
  while(WiFi.status() !=WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  WiFiClientSecure client;
  if(!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
  }
  return client;
}

void AccessPortal::getByteArray(char* uidInput, int* uidOutput ) {
  std::string uid(uidInput);
  unsigned int x;
  int counter = 0;
  for(int i = 1; i < 8; i += 2) {
      std::string segment = uid.substr(i-1, 2);
      x = strtoul(segment.c_str(), nullptr, 16);
      uidOutput[counter] = x;
      counter++;
  }
}
  String AccessPortal::getStudentDetails(int* byteArray, String* details) {
  char* response;
  WiFiClientSecure client = connect();
  if(client.connected() != 1) {
    Serial.println("Client is not connected.");
    return "ERROR";
  }
  String payload = "uuid=" + String(byteArray[0]) + "&uuid=" + String(byteArray[1]) +"&uuid=" + String(byteArray[2]) + "&uuid=" + String(byteArray[3]) + "&token="+ _token +"&userToken=True";
  client.print("POST ");
  client.print(cardURL);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(payload.length());
  client.println();
  client.println(payload);

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\r');
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  return line;
}
