#include "AccessPortal.h"
/* Simple RFID Permissions example using the Access Portal Library. 
  Created on 01/01/2019
  Created by Eddie Atkinson

  API tokens can be created here: https://auth.uwamakers.com/login
  
*/ 
void setup() {
  AccessPortal access("YourSSID", "YourSSIDPassword", "YourAPIToken");
  Serial.begin(115200);
  String uid = ""; // enter your UID
  int byteArray[4]; // byte array that will be used to store the values of the UID hex string after conversion
  String userDetails[6]; // String array that will store a user's details

  bool byteArrayStatus = access.getByteArray(uid, byteArray); // converts uid hex string to integers and stores the results in the byteArray

  if(byteArrayStatus == false) {
    Serial.println("Problem converting UID to byteArray");
    return; 
  }
  bool requestStatus = access.getStudentDetails(byteArray, userDetails);
  if(requestStatus == false) {
    Serial.println("Request failed.");
  }
  String userToken = userDetails[5]; // User's API token is stored in last element of userDetails array.
  bool hasPerm = access.checkPermission("joining.joined", userToken);

  if(hasPerm == true) {
    Serial.println("User has correct permission");
  }
  else {
    Serial.println("User does not possess this permission");
  } 
}

void loop() {
  

}
