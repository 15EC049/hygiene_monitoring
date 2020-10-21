#include <ESP8266WiFi.h>
#define server  "smartsanitation.tech"
//Default WIFI Credential
const char* def_ssid =  "sanitation";
const char* def_pass =  "Smart"; 

//ADC pin Declaration
const int analogInPin = A0;

//User Wifi Credential
String wif_ap ="sanitation";
String wifi_pass ="Smart";

//Unique Sensor ID
String sen_id="ss_7034221248";
String get_wifi_page="/get-wifi-details.php";

String recstring="";
WiFiClient client;

void setup()
{
       Serial.begin(115200);
       delay(10);
 
       Serial.print("Connecting to ");
       Serial.println(def_ssid);
       WiFi.begin(def_ssid, def_pass);
       while (WiFi.status() != WL_CONNECTED) 
        {
            delay(100);
            Serial.print("*");
        }
      Serial.println("");
      Serial.println("WiFi connected to default credential");
      printWifiStatus();
      Serial.println("Connecting to WebSite");
      while (0 == client.connect(server, 80))
        {
          Serial.println("Re-Connecting to WebSite");
          delay(1000);
        }
      get_new_wifi_details();
      if(wifi_ap!="sanitation")
      {
        Serial.print("Connecting to ");
        Serial.println(wif_ap);
        WiFi.begin(wifi_ap, wifi_pass);
        while (WiFi.status() != WL_CONNECTED) 
          {
            delay(100);
            Serial.print("*");
          }
        Serial.println("");
        Serial.println("WiFi connected to user credential");
        printWifiStatus();
      }
      else
      {
        Serial.println("No New wifi found. Please visit smartsanitation.tech/wifi_details.php to give wifi details");
      }
}
void loop()
{
  
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
void get_new_wifi_details()
{
     if (client.connect(server,80)) 
      {
        String str=get_wifi_page+"?senid="+sen_id;
        client.println("GET  "+ str);
        client.println("Host: " server);     
        client.println("Connection: close");
        client.println();
        while (!client.available())
          {
            delay(100);
          }
        recstring="";
        while (client)
          {
            int v = client.read();
            if (v != -1)
             {
              recstring = recstring+(char)v;
              //Serial.print((char)v);
             }
            else
             {
              //Serial.println("no more content, disconnect");
              client.stop();
              while (1)
                {
                  delay(1);
                }
              }  
            } 
           Serial.print(recstring);
           if (recstring != "")
            {
              int firstletter = recstring.indexOf('AP:');
              int secondletter = recstring.indexOf(firstletter,'pass:');
              int thirdletter = recstring.indexOf(secondletter,'end');
              wifi_ap=recstring.substring(firstletter, secondletter);
              wifi_pass=recstring.substring(secondletter,thirdletter);
     
            }
}
}
