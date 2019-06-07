#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

void twilio_warn_call(); //trigger call to cleaning person

WiFiClient client;

int calls = 1;   // Execution count, so this doesn't run forever
int maxCalls = 10;   // Maximum number of times the Choreo should be executed
char keypad_value
void setup() {
  Serial.begin(9600);

  int wifiStatus = WL_IDLE_STATUS;

  // determine if the WiFi Shield is present.
  Serial.print("\n\nShield:");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("FAIL");
    
    // if there's no WiFi shield, stop here.
    while(true);
  }
 
  while(wifiStatus != WL_CONNECTED) {
    Serial.print("WiFi:");
    wifiStatus = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    if (wifiStatus == WL_CONNECTED) {
      Serial.println("OK");
    } else {
      Serial.println("FAIL");
    }
    delay(5000);
  }
}

void loop()
{

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
