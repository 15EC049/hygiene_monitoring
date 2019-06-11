const int watermeterPin = 7;
int blynkButton=2;
int waterstatus_to_blynk=3;
int read_water_status_pin=4;
int motr_connected_pin=5;
volatile int  pulse_frequency;
unsigned int  literperhour;
unsigned long currentTime, loopTime;
byte sensorInterrupt = 0;

void setup()
{ 
   pinMode(watermeterPin, INPUT);
   Serial.begin(9600); 
   attachInterrupt(sensorInterrupt, getFlow, FALLING);
                                     
   currentTime = millis();
   loopTime = currentTime;
} 

void loop ()    
{
  int st=digitalRead(read_water_status_pin);//water level to blynk
  if(st==HIGH)
  digitalWrite(waterstatus_to_blynk,HIGH);
  else
  digitalWrite(waterstatus_to_blynk,LOW);

  
  if(digitalRead(blynkButton)==HIGH)// motor on or off from blynk
    digitalWrite(motr_connected_pin,HIGH);
else
  digitalWrite(motr_connected_pin,LOW);

   currentTime = millis();
   if(currentTime >= (loopTime + 1000))
   {
      loopTime = currentTime;
      literperhour = (pulse_frequency * 60 / 7.5);
      pulse_frequency = 0;
      Serial.print(literperhour, DEC);
      Serial.println(" Liter/hour");
   }
}
void getFlow ()
{ 
   pulse_frequency++;
}
