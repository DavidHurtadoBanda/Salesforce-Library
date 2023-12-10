#include <ArduinoJson.h>
#include <WiFiClientSecure.h>


#ifndef Salesforce_h
#define Salesforce_h

class Salesforce {

private:
  String  _instanceName;   // Example: "eu18"
  String  _clientId;       // ClientId credential
  String  _clientSecret;   // ClientSecret credential
  String  _userName;       // UserName credential
  String  _password;       // Password credential
  String  _accessToken;    // Internal variable we use to save the Salesforce access token
  char*   _salesforceHost; // The URL to connect to Salesforce
  String  _responseBody;   // The body of the call response
  boolean _isProd;         // Set Production (true) or Sandbox (false)
  boolean _control401;     // Is a control for not make 2 calls if get 401
  String  _apiVersion;     // The version of the API. Default: v58.0
  

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
  String buildCallRequest(String requestBody, String verb, String contentType, String callPath, String token);



  /**
  * Process the Salesforce response and return the JSON part of the response.
  *
  * @param clientSecure WiFi secure client for communication.
  */
  void processResponse(WiFiClientSecure& clientSecure);



  /**
  * Request and store the Salesforce access token and the instance url.
  */
  void requestToken();



public:
  int     _responseCode;   // The call status code

  /**
  * Constructor for the Salesforce class.
  */
  Salesforce();



  /**
  * Set up the minimum required variables for connecting to Salesforce.
  *
  * @param instanceName Salesforce instance name.
  * @param clientId Client identifier for authentication.
  * @param clientSecret Client secret for authentication.
  * @param userName User name for authentication.
  * @param password Password for authentication.
  */
  void setUp(String instanceName, String clientId, String clientSecret, String userName, String password);



  /**
  * Set up the minimum required variables for connecting to Salesforce.
  *
  * @param isProd Salesforce instance name.
  * @param clientId Client identifier for authentication.
  * @param clientSecret Client secret for authentication.
  * @param userName User name for authentication.
  * @param password Password for authentication.
  */
  void setUp(boolean isProd, String clientId, String clientSecret, String userName, String password);



  /**
  * Set up the API Version
  *
  * @param apiVersion Salesforce version of the API.
  */
  void setAPIVersion(String apiVersion);



  /**
  * Make the HTTP call to Salesforce.
  *
  * @param endpoint Call endpoint.
  * @param verb HTTP verb (GET, POST, etc.).
  * @param contentType Content type of the request.
  * @param body Body of the HTTP request.
  * @return String representing the Salesforce JSON response.
  */
  String httpCall(String endpoint, String verb, String contentType, String body);



  /**
  * Returns call status code
  *
  * @return Integer representing the Status code of the call. Example: 200
  */
  int getResponseCode();



  /**
  * Call to the standart API to insert Data
  *
  * @param sObject The name of the sObject we want to delete. Example: "Account"
  * @param Id The Id of the sObject we want to delete
  * @param requestBody The body of the object we want to insert in JSON format
  * @return The Id of the object created
  */
  String createData(String sObject, String requestBody);



  /**
  * Call to the standart API asking for data based on the params received as query
  *
  * @param sObject The name of the sObject we want to delete. Example: "Account"
  * @param Id The Id of the sObject we want to delete
  * @return String representing the Salesforce JSON response.
  */
  String readData(String sObject, String Id);



  /**
  * Call to the standart API to update Data
  *
  * @param sObject The name of the sObject we want to delete. Example: "Account"
  * @param Id The Id of the sObject we want to delete
  * @param requestBody The body of the object we want to insert in JSON format
  * @return Boolean representing if the petition was success
  */
  boolean updateData(String sObject, String Id, String requestBody);



  /**
  * Call to the standart API to delete Data
  *
  * @param sObject The name of the sObject we want to delete. Example: "Account"
  * @param Id The Id of the sObject we want to delete
  * @return Boolean representing if the petition was success
  */
  boolean deleteData(String sObject, String Id);



  /**
  * Call to the standart API asking for data based on the param received as query
  *
  * @param query The query to retreive data from Database in Salesforce.
  * @return String representing the Salesforce JSON response.
  */
  String queryData(String query);
};


#endif

