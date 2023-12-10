#include "Salesforce.h"

/**
* Constructor for the Salesforce class.
*/
Salesforce::Salesforce() {
  _control401 = false;
  _apiVersion = "v58.0";
}



/**
* Set up the minimum required variables for connecting to Salesforce.
*
* @param instanceName Salesforce instance name.
* @param clientId Client identifier for authentication.
* @param clientSecret Client secret for authentication.
* @param userName User name for authentication.
* @param password Password for authentication.
*/
void Salesforce::setUp(String instanceName, String clientId, String clientSecret, String userName, String password) {
  String concatenatedHost = instanceName + ".my.salesforce.com";
  _salesforceHost = strdup(concatenatedHost.c_str());
  _instanceName = instanceName;
  _clientId = clientId;
  _clientSecret = clientSecret;
  _userName = userName;
  _password = password;
}



/**
* Set up the minimum required variables for connecting to Salesforce.
*
* @param isProd Salesforce instance name.
* @param clientId Client identifier for authentication.
* @param clientSecret Client secret for authentication.
* @param userName User name for authentication.
* @param password Password for authentication.
*/
void Salesforce::setUp(boolean isProd, String clientId, String clientSecret, String userName, String password) {
  _isProd = isProd;
  if (_isProd) {
    _salesforceHost = strdup("login.salesforce.com");
  } else {
    _salesforceHost = strdup("test.salesforce.com");
  }
  _clientId = clientId;
  _clientSecret = clientSecret;
  _userName = userName;
  _password = password;
}



/**
* Set up the API Version
* This method is optional because be default will setted "v58.0"
*
* @param apiVersion Salesforce version of the API.
*/
void Salesforce::setAPIVersion(String apiVersion){
  _apiVersion = apiVersion;
}



/**
* Returns call status code
*
* @return Integer representing the Status code of the call. Example: 200
*/
int Salesforce::getResponseCode(){
  return _responseCode;
}



/**
* Build the HTTP request for the Salesforce call.
*
* @param requestBody Body of the HTTP request.
* @param verb HTTP verb (GET, POST, etc.).
* @param contentType Content type of the request.
* @param callPath Path of the call.
* @param token Access token for authorization.
* @return String representing the constructed HTTP request.
*/
String Salesforce::buildCallRequest(String requestBody, String verb, String contentType, String callPath, String token) {
  String callString = verb + " " + callPath + " HTTP/1.1\r\n" + "Host: " + _salesforceHost + "\r\n";
  if (!token.isEmpty()) {
    callString += "Authorization: Bearer " + token + "\r\n";
  }
  callString += "Content-Type: " + contentType + "\r\n" + "Content-Length: " + requestBody.length() + "\r\n\r\n" + requestBody;
  return callString;
}



/**
* Process the Salesforce response and return the JSON part of the response.
*
* @param clientSecure WiFi secure client for communication.
*/
void Salesforce::processResponse(WiFiClientSecure &clientSecure) {
  String response = clientSecure.readString();

  //Serial.println("Respuesta completa de SF:");
  //Serial.println(response);

  String firstLine = clientSecure.readStringUntil('\r');

  _responseCode = response.substring(9, 12).toInt();
  _responseBody = "";

  int bodypos = response.indexOf("{");
  char previus = response[bodypos - 1];
  bool isList = (previus == '[');

  if (isList) {
    int closingBracketPos = response.indexOf("]", bodypos) + 1;
    _responseBody = response.substring(bodypos - 2, closingBracketPos);
  } else {
    _responseBody = response.substring(bodypos);
  }
}




/**
* Make the HTTP call to Salesforce.
*
* @param endpoint Call endpoint.
* @param verb HTTP verb (GET, POST, etc.).
* @param contentType Content type of the request.
* @param body Body of the HTTP request.
* @return String representing the Salesforce JSON response.
*/
String Salesforce::httpCall(String endpoint, String verb, String contentType, String body) {

  // Checks if already have an access token
  // If not, try to get one, If fails, stop the process
  if (_accessToken == "") {
    requestToken();
    if (_responseCode != 200) {
      return "";
    }
  }

  WiFiClientSecure clientSecure;

  // Skip verification
  clientSecure.setInsecure();

  if (clientSecure.connect(_salesforceHost, 443)) {

    String request = buildCallRequest(body, verb, contentType, endpoint, _accessToken);

    clientSecure.print(request);

    delay(100);

    processResponse(clientSecure);


    clientSecure.stop();

    boolean control401 = false;
    // Modulo de reintento en caso de haber caducado el token
    if (_responseCode == 401 && !control401) {
      control401 = true;
      requestToken();
      if (_responseCode != 200) {
        return "";
      }
      httpCall(endpoint, verb, contentType, body);
    }


    return _responseBody;
  } else {
    Serial.println("");
    Serial.println("Error connecting to Salesforce host to obtain the token");
    return "";
  }
}




/**
* Request and store the Salesforce access token and the instance url.
*/
void Salesforce::requestToken() {
  _accessToken = "fakeToken";

  // Build the body
  String tokenBody = "grant_type=password&client_id=" + _clientId + "&client_secret=" + _clientSecret + "&username=" + _userName + "&password=" + _password;

  // Do the call
  httpCall("/services/oauth2/token", "POST", "application/x-www-form-urlencoded", tokenBody);

  // Store the response
  if (_responseCode == 200) {
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, _responseBody);

    // Store the access token
    _accessToken = doc["access_token"].as<String>();

    // In case the instance name was not setted we take it from the response
    if (_instanceName == "") {
      _salesforceHost = strdup(doc["instance_url"].as<String>().c_str());
    }
  } else {
  }
}



/**
* Call to the standart API to insert Data
*
* @param sObject The name of the sObject we want to delete. Example: "Account"
* @param Id The Id of the sObject we want to delete
* @param requestBody The body of the object we want to insert in JSON format
* @return The Id of the object created
*/
String Salesforce::createData(String sObject, String requestBody) {

  String path = "/services/data/" + _apiVersion + "/sobjects/" + sObject;

  String _responseBody = httpCall(path, "POST", "application/json", requestBody);

  if (_responseCode == 201) {
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, _responseBody);

    return doc["id"].as<String>();
  } else {
    return "";
  }

}



/**
* Call to the standart API asking for data based on the params received as query
*
* @param sObject The name of the sObject we want to delete. Example: "Account"
* @param Id The Id of the sObject we want to delete
* @return String representing the Salesforce JSON response.
*/
String Salesforce::readData(String sObject, String Id) {

  String path = "/services/data/" + _apiVersion + "/sobjects/" + sObject + "/" + Id;

  String response = httpCall(path, "GET", "application/json", "");

  return response;
}




/**
* Call to the standart API to update Data
*
* @param sObject The name of the sObject we want to delete. Example: "Account"
* @param Id The Id of the sObject we want to delete
* @param requestBody The body of the object we want to insert in JSON format
* @return Boolean representing if the petition was success
*/
boolean Salesforce::updateData(String sObject, String Id, String requestBody) {

  String path = "/services/data/" + _apiVersion + "/sobjects/" + sObject + "/" + Id;

  String response = httpCall(path, "PATCH", "application/json", requestBody);

  return (_responseCode == 0) ? true : false;
}



/**
* Call to the standart API to delete Data
*
* @param sObject The name of the sObject we want to delete. Example: "Account"
* @param Id The Id of the sObject we want to delete
* @return Boolean representing if the petition was success
*/
boolean Salesforce::deleteData(String sObject, String Id) {

  String path = "/services/data/" + _apiVersion + "/sobjects/" + sObject + "/" + Id;

  String response = httpCall(path, "DELETE", "application/json", "");

  return (_responseCode == 204) ? true : false;
}



/**
* Call to the standart API asking for data based on the param received as query
*
* @param query The query to retreive data from Database in Salesforce.
* @return String representing the Salesforce JSON response.
*/
String Salesforce::queryData(String query) {

  query.replace(" ", "%20");
  String path = "/services/data/" + _apiVersion + "/query/?q=" + query;

  String response = httpCall(path, "GET", "application/json", "");

  return response;
}
