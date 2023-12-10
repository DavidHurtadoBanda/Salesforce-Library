/**
* Wifi Credentials
*/
#define SSID_NAME ""
#define SSID_PASSWORD ""
#include "WifiModule.h"



/**
* Salesforce Credentials
*/
String CLIENT_ID = "";
String CLIENT_SECRET = "";
String USER_NAME = "";
String PASSWORD = "";
String INSTANCE_NAME = ""; // SELECT InstanceName FROM Organization
#include <Salesforce.h>

Salesforce salesforce;

void setup() {
  Serial.begin(115200);

  SetUpWifi();

  salesforce.setUp(INSTANCE_NAME, CLIENT_ID, CLIENT_SECRET, USER_NAME, PASSWORD);

  // Also can set it up just passing as first param the boolean: "isProd"
  //salesforce.setUp(true, CLIENT_ID, CLIENT_SECRET, USER_NAME, PASSWORD);

  // We create a JSON 
  String requestBody = createRequestBody();

  String objId = salesforce.createData("Account", requestBody);

  Serial.print("The Id of the Account created: ");
  Serial.println(objId);

}

void loop() {
  CheckInternetConexion();

  delay(5000);
}

String createRequestBody() {
  DynamicJsonDocument jsonDoc(1024);
  jsonDoc["Name"] = "Test Account";
  String body;
  serializeJson(jsonDoc, body);
  return body;
}