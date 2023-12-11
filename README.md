# Salesforce-Library
A library to facilitate calls between Arduino and Salesforce

![SalesforcePetFoto](https://www.salesforce.com/content/dam/web/en_us/www/images/resource-center/guide/data-security-governance-it-guide/og-card-itgdsg.png)

## Table of Contents
- [Dependencies](#Dependencies)
- [Set up](#set-up-questions)
- [Call Web Service](#call-web-service)
- [Query Data](#query-data)
- [CRUD](#crud)
- [Contact](#contact)


## Dependencies

This library have dependencies on the following libraries:
- `WiFiClientSecure.h` Is used to establish the connection with Salesforce.
- `ArduinoJson.h` Is used to manage JSON strings as objects in our code.


## Set up

<kbd>SetUp</kbd> method is required. There you will establish  the credentials to make the connection.

There are two ways to set it up:
-   <kbd>salesforce.setUp(INSTANCE_NAME, CLIENT_ID, CLIENT_SECRET, USER_NAME, PASSWORD);</kbd>
    We can send as first param the string: InstanceName (CS81... EU24...).
-   <kbd>salesforce.setUp(IS_PROD, CLIENT_ID, CLIENT_SECRET, USER_NAME, PASSWORD);</kbd>
    We can also just set up without the instance name just passing a boolean IsProd (if is Production true, if is Sandbox false);

By default, for calls to the standar API will use the "**version V58.0**" but optionally you can use the method <kbd>setAPIVersion(String apiVersion)</kbd> to specify the version you need.


## Call Web Service

Once you instantiate the Salesforce class, make an Http call is as simple as that:

<kbd>String response = salesforce.httpCall("/services/apexrest/Test", "POST", "application/json", requestBody);</kbd>
- First param: The path. The sub-url where we want to call to.
- Second param: The verb. (GET, POST, PUT...)
- Third param: The content type. (Usually is application/json)
- Forth param: The body. A string with the body of our request.

It will return a string with just the body response (Filtering the Headers, Status code, etc).


## Query Data

You can easy make queries to the standard API just like that:
<kbd>String response = salesforce.queryData("SELECT Id FROM Account LIMIT 5");</kbd>

This method need to receive a query as string and will return the Salesforce body response.


## CRUD

The library expose another four methods to make a CRUD (Create, read, update, delete);

- <kbd>String createData(String sObject, String requestBody);</kbd>
- <kbd>String readData(String sObject, String Id);</kbd>
- <kbd>boolean updateData(String sObject, String Id, String requestBody);</kbd>
- <kbd>boolean deleteData(String sObject, String Id);</kbd>


## Contact

As expected, you can obvius contact me and will try to answer you as soon as possible at:

[Mail](mailto:hurtado.banda.david@gmail.com)
[LinkedIn](https://www.linkedin.com/in/davidhurtadobanda/)