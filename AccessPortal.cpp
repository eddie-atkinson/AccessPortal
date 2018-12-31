/* Access portal library */
#include "Arduino.h"
#include "AccessPortal.h"
#include "ArduinoJson.h"

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

bool AccessPortal::detailsSplitter(String line, String details[6]) {
  bool successStatus = false;
  DynamicJsonBuffer jsonBuffer;
  DynamicJsonBuffer detailsBuffer;
  JsonObject& root = jsonBuffer.parseObject(line);
  String userDetails = root ["user"];
  String userToken = root["userToken"];
  String status = root["success"];
  JsonObject& container = jsonBuffer.parseObject(userDetails);
  
  String username = container["username"];
  String email = container["email"];
  String fullname = container["fullname"];
  String lastname = container["lastname"];
  String firstname = container["firstname"];

  details[0] = username;
  details[1] = email;
  details[2] = fullname;
  details[3] = lastname;
  details[4] = firstname;
  details[5] = userToken;

  if(status == "true") {
    successStatus = true;
  }

  return successStatus; 
}
void AccessPortal::getByteArray(String uidInput, int uidOutput[4] ) {
  unsigned int x;
  int counter = 0;
  for(int i = 1; i < 8; i += 2) {
      String segment = uidInput.substring(i-1, i + 1);
      x = strtoul(segment.c_str(), nullptr, 16);
      uidOutput[counter] = x;
      counter++;
  }
}
  bool AccessPortal::getStudentDetails(int* byteArray, String details[6]) {
  char* response;
  WiFiClientSecure client = connect();
  if(client.connected() != 1) {
    Serial.println("Client is not connected.");
    return false;
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
  bool successStatus = detailsSplitter(line, details);
  return successStatus;
}

bool AccessPortal::checkPermission(const char* permission, String userToken) {
  bool hasPermission = false;
  String payload = "token=" + userToken;
  WiFiClientSecure client = connect();
  if(client.connected() != 1) {
    Serial.println("Client is not connected.");
    return false;
  }
  client.print("GET ");
  client.print (permissionsURL);
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
  String line = client.readStringUntil(']');
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  
  const char* lineToSearch = line.c_str();
  if(strstr(lineToSearch, permission) != NULL) {
    hasPermission = true;
  }
  return hasPermission;
} 


