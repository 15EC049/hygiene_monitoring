
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

 char keypad_value;//keypad entry from call

int n1,n2,n3;// node values
String st;
String name="SRITHI";
String time;
float percent;
String cln;
int men;

//m2x initialisation
char deviceId[] = "6d85d4c7dd2ea06749e04d1bb6f2cab3"; // Device you want to receive values
char streamName1[]= "node1"; // Stream you want to receive values
char streamName2[]= "node2";
char streamName3[]= "node3";
//char streamName4[]= "node4";
char streamName5[]= "uid";
char streamName6[]= "count";
//char streamName[]7 = "water";
char m2xKey[] = "5bcb99f9bff9c2364a7ffad3a44ceb0a"; // Your M2X access key

char auth[] = "5eff5785841c4f83b9437b5dea41d4b8";//blynk auth

//object
M2XStreamClient m2xClient(&client, m2xKey);
WiFiClient client;
LiquidCrystal lcd(2,9,10,5,6,8);

void clockDisplay()
{
  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
 time=currentTime;
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();
}

BLYNK_CONNECTED()
{
  // Synchronize time on connection
  rtc.begin();
}
BLYNK_READ(V0)//node1
{ 
  Blynk.virtualwrite(V0,st);//status
}
BLYNK_READ(V1)//cleaner+date and time
{ 
 cln=name+"    "+time;
  Blynk.virtualwrite(V1,cln);
}
BLYNK_READ(V3)//people count
{ 
  Blynk.virtualwrite(V5,people_cnt);
}
BLYNK_WRITE(V4)//node 1 hit
{ 
  BLYNK_LOG("Got a value: %s", param.asStr());
  f1 = param.asInt(); 
}
BLYNK_WRITE(V5)//node 2 hit
{ 
  BLYNK_LOG("Got a value: %s", param.asStr());
  f2 = param.asInt(); 
}
BLYNK_WRITE(V6)//cleaner hit
{ 
  BLYNK_LOG("Got a value: %s", param.asStr());
  f3 = param.asInt(); 
}
BLYNK_WRITE(V)//cleaner hit
{ 
  BLYNK_LOG("Got a value: %s", param.asStr());
  men = param.asInt(); 
}




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
  count_people();
 
feedback = digitalRead(buttonPin);//value of feed back button
    n1=(f1==1)?random(700,900):random(100);
    n2=(f2==1)?random(700,900):random(100);
    n3=random(200,300);
  float avg=((n1+n2+n3)/3)
  float p=(avg/1000)*100;
  percent=100-p;
  
 Serial.print("node 1:");
 Serial.println(n1);
   Serial.print("node 2:");
 Serial.println(n2);
 Serial.print("node 3:");
 Serial.println(n3);
 // Serial.print("node 4:");
 //Serial.println(n4);
 Serial.print("Toilet Status: ");
if(percent<=25||feedback == HIGH)//vb

{
 Serial.println("very bad");
 lcd.setCursor(0, 2);
 lcd.print(" V.BAD  ");
 lcd.setCursor(0, 3);
 lcd.print(percent);
 lcd.print("%");
}

else if(percent>25&&percent<=35)//b
{
 Serial.println("bad");
 lcd.setCursor(0, 2);
 lcd.print(" BAD ");
 lcd.setCursor(0, 3);
 lcd.print(percent);
 lcd.print("%");
}

else if(percent>35&&percent<=50)//m
{
 Serial.println("medium");
 lcd.setCursor(0, 2);
 lcd.print(" MEDIUM");
 lcd.setCursor(0, 3);
 lcd.print(percent);
 lcd.print("%");
}

else if(percent>50&&percent<=75)//g
{
 Serial.println("good");
 lcd.setCursor(0, 2);
 lcd.print("GOOD ");
 lcd.setCursor(0, 3);
 lcd.print(percent);
 lcd.print("%");
}
else if(percent>75&&percent<=100)//vg
{
 Serial.println("vg");
 lcd.setCursor(0, 2);
 lcd.print(" V.GOOD ");
 lcd.setCursor(0, 3);
 lcd.print(percent);
 lcd.print("%");
}

  
  if (percent<=25||feedback == HIGH)
  {
    Serial.println("\n UNHYGEINE! Making phone call...\n");
       twilio_warn_call();
    // if the user chooses option 1 when they receive the phone call
    if ( keypad_value == '1') 
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
  //int response4 = m2xClient.updateStreamValue(deviceId, streamName4, n4);
  int response6 = m2xClient.updateStreamValue(deviceId, streamName6, people_cnt);
  int response5 = m2xClient.updateStreamValue(deviceId, streamName5, name);
    Blynk.run();
}


void count_people()
{
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
 
}

 void twilio_warn_call()
 {
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
