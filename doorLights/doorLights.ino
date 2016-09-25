#include <IRremote.h>
#include <SPI.h>
#include <SimpleTimer.h>

//pin 13 used to turn lights on/off
int lightsPin = 13;

//pin 4 is the input for the ir sensor
int IRpin = 4;

//power check pin
int powerCheckPin = 5;

//initialize the ir sensor
IRrecv irrecv(IRpin);
decode_results result;

//initialize the timer
SimpleTimer timer;

//valid ir input values
long inputValues[] = {16753245,16769055};

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing..."); 
  Serial.println("Starting the ir sensor..."); 
  irrecv.enableIRIn();
  pinMode(lightsPin, OUTPUT);
  pinMode(powerCheckPin, OUTPUT);
  timer.setInterval(40000, powerCheck);
}

void loop()
{
  if(irrecv.decode(&result))
  {
    if(inArray())
    {
      remoteChoices(); 
    }
    
    irrecv.resume();
  }
  
  timer.run();
}

void remoteChoices(){
    Serial.println(result.value);
    switch(result.value)
    {
      case 16753245: //1st remote action
        Serial.println(result.value);
        Serial.println("Turning lights on");
        digitalWrite(lightsPin, HIGH);     // sets the Red LED on
        break;
      case 16769055://6th remote action
        Serial.println(result.value);
        Serial.println("Turning lights off");
        digitalWrite(lightsPin, LOW);
        break;
      default:
        Serial.println("Not a valid input");
        break;
    }
}

void powerCheck(){
    int check = digitalRead(lightsPin);
    Serial.println("power check running");
    if(check == LOW){
        Serial.println("toggling power on pin 5");        
        digitalWrite(powerCheckPin, HIGH);
        delay(500);
        digitalWrite(powerCheckPin, LOW);
    }
}

boolean inArray(){
   for (int i = 0; i < 5; i++) {
      long test = inputValues[i];
      if(result.value == test)
      {
         return true;
      }
  } 
  Serial.println("no value found");
  return false;
}
