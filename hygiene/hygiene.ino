
//library files
#include <aJSON.h>  
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Temboo.h>
#include <WiFiClient.h>
#include <LiquidCrystal.h>
#include "TembooAccount.h" 
#include "M2XStreamClient.h"
#include <BlynkSimpleEnergiaWiFi.h> 

//pin and variable initialisation   
const int buttonPin = PUSH2; //feedback button 
int feedback = 0; //feedback value

int  pirpin = 19; // pir pin
int people_cnt=0;
int State = 0;         // current state of the pir sensor
int lastState = 0;     // previous state of the pir sensor
int keyIndex = 0;

int n1,n2,n3,n4;// node values

//m2x initialisation
char deviceId[] = "6d85d4c7dd2ea06749e04d1bb6f2cab3"; // Device you want to receive values
char streamName1[]= "node1"; // Stream you want to receive values
char streamName2[]= "node2";
char streamName3[]= "node3";
char streamName4[]= "node4";
char streamName5[]= "uid";
char streamName6[]= "count";
//char streamName[]7 = "water";
char m2xKey[] = "5bcb99f9bff9c2364a7ffad3a44ceb0a"; // Your M2X access key

char auth[] = "5eff5785841c4f83b9437b5dea41d4b8";//blynk auth

//object
M2XStreamClient m2xClient(&client, m2xKey);
WiFiClient client;
LiquidCrystal lcd(2,9,10,5,6,8);


BLYNK_WRITE(V0)//node1
{ 
  BLYNK_LOG("Got a value: %s", param.asStr());
  n1 = param.asInt(); 
}
BLYNK_WRITE(V1)//node1
{ 
  BLYNK_LOG("Got a value: %s", param.asStr());
  n2 = param.asInt(); 
}
BLYNK_WRITE(V2)//node1
{ 
  BLYNK_LOG("Got a value: %s", param.asStr());
  n3 = param.asInt(); 
}
BLYNK_WRITE(V3)//node1
{ 
  BLYNK_LOG("Got a value: %s", param.asStr());
  n4 = param.asInt(); 
}

//wifi connection
char ssid[] = "Akshaya";
char password[] = "12345678";



void setup()
{
  Serial.begin(115200);
    lcd.begin(16, 4);
pinMode(buttonPin, INPUT_PULLUP);//feedback
pinMode(pirpin, INPUT);//people counter
Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    WiFi.begin(WIFI_SSID,WPA_PASSWORD);
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  
  // you're connected now, so print out the status  
  printWifiStatus();
  
  Blynk.begin(auth,WIFI_SSID  , WPA_PASSWORD );//blynk begin
    lcd.print("HYGIENE MONITOR");
}

void loop() 
{
  
   /*if(i==HIGH)//for rfid detection
   {
     lcd.setCursor(0, 1);
    lcd.print("CARD DETECTED");
   }*/
    lcd.setCursor(0, 1);
  lcd.print("TOILET STATUS");
  State = digitalRead(pirpin);
   if (State != lastState)
{
  if (State == HIGH) 

    {
      people_cnt++;
      Serial.print("number of people:  ");
      Serial.println(people_cnt);
    }
 lastState = State;
}
  
feedback = digitalRead(buttonPin);//value of feed back button
  
 Serial.print("node 1:");
 Serial.println(n1);
   Serial.print("node 2:");
 Serial.println(n2);
 Serial.print("node 3:");
 Serial.println(n3);
  Serial.print("node 4:");
 Serial.println(n4);
 if(n1==1)
 {
 x=x-25;
 
 }

  if(n2==1)

  {
 x=x-25;
  }
  if(n3==1)
  {
 x=x-25;
  } 
  if(n4==1)
  {
 x=x-25;
  }
 if(feedback==HIGH)
      x=0;
 if(x==0)//vb
 {
  Serial.println("vb");
  lcd.setCursor(0, 2);
  lcd.print("V.BAD");
    lcd.setCursor(0, 3);
  lcd.print("8%");
 }
else if(x==25)//b
 {
  Serial.println("b");
  lcd.setCursor(0, 2);
  lcd.print("BAD");
    lcd.setCursor(0, 3);
  lcd.print("24%");
 }
 else if(x==50)//m
 {
  Serial.println("m");
  lcd.setCursor(0, 2);
  lcd.print("MEDIUM");
    lcd.setCursor(0, 3);
  lcd.print("50%");
 }
 else if(x==75)//g
 {
  Serial.println("g");
  lcd.setCursor(0, 2);
  lcd.print("GOOD");
    lcd.setCursor(0, 3);
  lcd.print("78%");
 }
 else if(x==100)//vg
 {
  Serial.println("vg");
  lcd.setCursor(0, 2);
  lcd.print("V.GOOD");
    lcd.setCursor(0, 3);
  lcd.print("98%");
 }
  if (x==0||feedback == HIGH)
  {
    Serial.println("\n UNHYGEINE! Making phone call...\n");

    // if the user chooses option 1 when they receive the phone call
    if (makeNexmoCall() == "1") 
    {
      Serial.println("The user pressed 1.\n");
    } 
    else 
    {
      Serial.println("The user did not press 1.");
    }
  }
  
  int response1 = m2xClient.updateStreamValue(deviceId, streamName1, n1);
  int response2 = m2xClient.updateStreamValue(deviceId, streamName2, n2);
  int response3 = m2xClient.updateStreamValue(deviceId, streamName3, n3);
  int response4 = m2xClient.updateStreamValue(deviceId, streamName4, n4);
  int response6 = m2xClient.updateStreamValue(deviceId, streamName6, people_cnt);
  //int response5 = m2xClient.updateStreamValue(deviceId, streamName5, nam);
    Blynk.run();
}

 void twilio_warn_call()
 {
 if (calls <= maxCalls) {
    Serial.println("Running CaptureKeyPadEntry - Run #" + String(calls++));

    TembooChoreoSSL CaptureKeyPadEntryChoreo(client);

    // Invoke the Temboo client
    CaptureKeyPadEntryChoreo.begin();

    // Set Temboo account credentials
    CaptureKeyPadEntryChoreo.setAccountName(TEMBOO_ACCOUNT);
    CaptureKeyPadEntryChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    CaptureKeyPadEntryChoreo.setAppKey(TEMBOO_APP_KEY);
    CaptureKeyPadEntryChoreo.setDeviceType(TEMBOO_DEVICE_TYPE);

    // Set Choreo inputs
    String GoodbyeURLValue = "https://handler.twilio.com/twiml/EHae5b628d1f8cf2bad87743f16403ec4a";
    CaptureKeyPadEntryChoreo.addInput("GoodbyeURL", GoodbyeURLValue);
    String AnswerURLValue = "https://handler.twilio.com/twiml/EH18432b900451fc956e7f90136aae8878";
    CaptureKeyPadEntryChoreo.addInput("AnswerURL", AnswerURLValue);
    String AuthTokenValue = "f7b933dd8977c183daf329239a4e98c0";
    CaptureKeyPadEntryChoreo.addInput("AuthToken", AuthTokenValue);
    String ToValue = "91-7034-221-248";
    CaptureKeyPadEntryChoreo.addInput("To", ToValue);
    String FromValue = "1-717-347-0498";
    CaptureKeyPadEntryChoreo.addInput("From", FromValue);
    String AccountSIDValue = "AC84462a26b443f3f5bd41ff3c68a554a9";
    CaptureKeyPadEntryChoreo.addInput("AccountSID", AccountSIDValue);

    // Identify the Choreo to run
    CaptureKeyPadEntryChoreo.setChoreo("/Library/Twilio/Calls/CaptureKeyPadEntry");

    // Run the Choreo; when results are available, print them to serial
    CaptureKeyPadEntryChoreo.run();

    while(CaptureKeyPadEntryChoreo.available()) {
      char keypad_value = CaptureKeyPadEntryChoreo.read();
      Serial.print(c);
    }
    CaptureKeyPadEntryChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between CaptureKeyPadEntry calls
}


void printWifiStatus() 
{
  // print the SSID of the network you're attached to:
  Serial.print("Network Name: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
