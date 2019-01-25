/* 
Access Portal Library

Simple library for interacting with the UWA Makers Access portal to get user information and permissions using RFID strings. 

Created 01/01/2019
By Eddie Atkinson

TODO: Include link to example docs
Access Portal API Documentation: https://uwasystemhealth.github.io/pheme-auth/

This libary requires users to have a valid API token.
 API Tokens can be obtained using UWA Pheme details from the following URL: https://auth.uwamakers.com/login
 
*/
#include "Arduino.h"
#include "AccessPortal.h"
#include "ArduinoJson.h"

const char* host = "auth.uwamakers.com"; //UWA Makers Access Portal host URL
const int httpsPort = 443; 
const char* cardURL = "/api/card"; // URL where requests for user information are made using users' card UIDs
const char* permissionsURL = "/api/user/perms"; // URL where requests for user permissions are made

AccessPortal::AccessPortal(char* ssid, char* password, String token) {
  _ssid = ssid; // WiFi network name
  _password = password; // WiFI network password
  _token = token; // Access Portal API Token
}

// Private method that connects the Arduino to WiFi and returns a WiFi client to the caller 

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

// Private method which splits the JSON String returned from the Access Portal containing user details and places the details in an array of Strings

bool AccessPortal::detailsSplitter(String line, String details[6]) {
  bool successStatus = false; // status of the request made to the Access Portal endpoint 
  DynamicJsonBuffer jsonBuffer;
  DynamicJsonBuffer detailsBuffer;
  JsonObject& root = jsonBuffer.parseObject(line);
  String userDetails = root ["user"]; // breaks the JSON string into a smaller JSON string containing user details
  String userToken = root["userToken"]; // user API token that is used to make a GET request for user permissions
  String status = root["success"]; // status of the request from the JSON string returned by the endpoint

  if(status == "true") {
    successStatus = true;
  }

  JsonObject& container = jsonBuffer.parseObject(userDetails); 
  String username = container["username"]; // Student / Staff number
  String email = container["email"]; // UWA email address
  String fullname = container["fullname"]; // first name and last name of user
  String lastname = container["lastname"]; // surname
  String firstname = container["firstname"]; // first name 

  details[0] = username;
  details[1] = email;
  details[2] = fullname;
  details[3] = lastname;
  details[4] = firstname;
  details[5] = userToken;

  return successStatus; 
}

// Public method to take an 8 character UID string and convert it from hexadecimal to integers,
// placing the result in an array of integers provided as a formal parameter

bool AccessPortal::getByteArray(String uidInput, int uidOutput[4] ) {
  if(uidInput.length() != 8) {
    return false;
  }
  unsigned int x;
  int counter = 0;
  for(int i = 1; i < 8; i += 2) {
      String segment = uidInput.substring(i-1, i + 1);
      x = strtoul(segment.c_str(), nullptr, 16);
      uidOutput[counter] = x;
      counter++;
  }
  return true;
}
// Public method that makes a POST request to the endpoint using the UID byte array and the endpoint API token,
// makes a call to detailsSplitter to split the JSON string and stores the result in an array of Strings.
// Returns the status of the POST request as a boolean value.

  bool AccessPortal::getStudentDetails(int* byteArray, String details[6]) {
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
  String line;
  boolean responseStart = false; 
  Serial.println("");
  while (client.connected()) {
    char c = client.read();
    if(c == '{') {
      responseStart = true;
    }
    if(responseStart == true) {
      if(line.length() > 1000) {
        break;
      }
      line += c;
    }
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  
  bool successStatus = detailsSplitter(line, details); 
  return successStatus;
}

// Public method that makes a GET request to the endpoint containing the User's API Token and checks whether a user-specified permission 
// is present in the list of permissions returned by the endpoint.

bool AccessPortal::checkPermission(const char* permission, String userToken) {
  bool hasPermission = false; // boolean determining whether the user has a permission
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
  String line; 
  boolean responseStart = false;
  while (client.connected()) {
    char c = client.read();
    if(responseStart == true) {
      line += c;
    }
    if(c == ']') {
      break; // read string returned by endpoint until terminating ]
    }
    if(c == '[') {
      responseStart = true;
    }
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  
  const char* lineToSearch = line.c_str(); // convert the line to a c string
  if(strstr(lineToSearch, permission) != NULL) { // search through the line and see if the permission is present
    hasPermission = true; // if the permission is present change hasPermission to true
  }
  return hasPermission;
} 


