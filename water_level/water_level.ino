const int watermeterPin = 2;

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
