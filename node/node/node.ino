#include <ESP8266WiFi.h>
#define server  "smartsanitation.tech"
//Default WIFI Credential
const char* def_ssid =  "Umar";
const char* def_pass =  "umarahammed"; 

//ADC pin Declaration
const int analogInPin = A0;
//Store Sen Value
int sensorValue = 0;
//User Wifi Credential
String wifi_ap ="sanitation";
String wifi_pass ="Smart";

//Unique Sensor ID
String sen_id="sen2";
String get_wifi_page="/get-wifi-details.php";
String send_sen_val_page="/upload-data.php";

String recstring="";
WiFiClient client;

void setup()
{
       Serial.begin(115200);
       delay(10);
       pinMode(LED_BUILTIN, OUTPUT);
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
      if(wifi_ap!=def_ssid)
      {
        Serial.print("Connecting to ");
        Serial.println(wifi_ap);
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
  sensorValue = analogRead(analogInPin);
  send_sen_data();
  Serial.print("Value=");
  Serial.print(sensorValue);
  delay(2000);
  led_blink();
  delay(300000);
}

void led_blink()
{
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);   // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  delay(1000);   
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

void send_sen_data()
{
       if (client.connect(server,80)) 
      {
        String v=String(sensorValue);
        String str=send_sen_val_page+"?id="+sen_id+"&val="+v+" HTTP/1.1";
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
      }
  
}
void get_new_wifi_details()
{
     if (client.connect(server,80)) 
      {
        String str=get_wifi_page+"?id="+sen_id+" HTTP/1.1";
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
           String st="";
           if (recstring != "")
            {
              int len=recstring.length();
              int i=0;
              for (i=0;i<len;i++)
              {
                if(recstring.charAt(i)!=':')
                {
                  st=st+recstring.charAt(i);
                }
                else
                {
                  wifi_ap=st;
                  st="";
                }
                
              }
              wifi_pass=st;
              Serial.println("");
              Serial.print("Got New WIFI details from server AP:");
              Serial.print(wifi_ap);
              Serial.print("   Password:");
              Serial.println(wifi_pass);
            }

}
}
