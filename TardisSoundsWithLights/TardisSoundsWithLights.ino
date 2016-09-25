#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h> 
#include <SFEMP3Shield.h>

#include <IRremote.h>

SdFat sd;
SFEMP3Shield MP3player;

//IR vars
int IRpin = A5;
IRrecv irrecv(IRpin);
decode_results result;
//valid remote values
long inputValues[] = {16753245, 16769565, 16720605, 16712445, 16761405, 16769055, 16754775, 16748655, 16738455, 16750695};

//Light vars
int ledPin = 5;
int policeLightPin = A0;

//Sound Tracks
int tardis = 1;
int daleks = 2;
int iMATimeLord = 3;
int cyberman = 4;//not actually cybermen...good man goes to war
int theme = 5;
int timey = 6;
int dontBlink = 7;//need louder volume
int mummy = 8;//are you my mummy...speakers on
int sonic = 9;//sonci screwdriver...speaker off

int speakerPin = A3;
int vol = 0;

void setup() {

  Serial.begin(115200);

  //start the shield
  sd.begin(SD_SEL, SPI_HALF_SPEED);
  MP3player.begin();
  MP3player.setVolume(10,10);
  
  //enable lights for police sign
  pinMode(policeLightPin, OUTPUT);
  
  pinMode(speakerPin, OUTPUT);
  //digitalWrite(speakerPin, HIGH);
  
  irrecv.enableIRIn();
  Serial.println("Finished startup...");
}

void loop() {
  if(irrecv.decode(&result))
  {
    //Serial.println(result.value);
    //testRemote();
    if(inArray())
    {
      remoteChoices(); 
    }
    
//    if(MP3player.isPlaying()){
//      delay(500);
//      forceStop();
//    }	

    irrecv.resume();
  }
}

void testRemote(){
    MP3player.playTrack(iMATimeLord);
}

void forceStop() {
   MP3player.stopTrack(); 
}

void remoteChoices() {
  switch(result.value)
  {
    case 16753245://1st remote action
      Serial.println(result.value);
      turnOnPoliceSign();
      break;
    case 16769565://2nd remote action
      Serial.println(result.value);
      tardisSoundLightShow();
      break;
    case 16720605://3rd remote action
      Serial.println(result.value);
      MP3player.playTrack(daleks);
      break;
    case 16712445://4th remote action
      Serial.println(result.value);
      MP3player.playTrack(iMATimeLord);
      break;
    case 16761405://5th remote action...flick left
      Serial.println(result.value);
      MP3player.playTrack(cyberman);
      break;
    case 16769055://6th remote action
      Serial.println(result.value);
      turnOffPoliceSign();
      break;
     case 16754775://7th remote action...flick right
      Serial.println(result.value);
      MP3player.playTrack(theme);
      break;
     case 16748655://8th remote action...flick down
      Serial.println(result.value);
      MP3player.playTrack(timey);
      break;
     case 16738455://9th remote action
      //Serial.println(result.value);
      Serial.println("getting volume");
      //int vol = 0;
      vol = MP3player.getVolume();
      Serial.println(vol);
      MP3player.playTrack(dontBlink);
      break;
     case 16750695://10th remote action
      Serial.println(result.value);
      toggleSpeakerPower();
      break;
    default:
      Serial.println(result.value);
      MP3player.stopTrack();
      break;
  }
}

boolean inArray(){
   //Serial.println(result.value);
   //need to figure out a way to get the array length instead of hard coding the value
   for (int i = 0; i < 10; i++) {
      //Serial.println(i);
      long test = inputValues[i];
      //Serial.println(test);
      if(result.value == test)
      {
         //Serial.println("found match");
         return true;
      }
  } 
  
  return false;
}

void toggleSpeakerPower(){
  int currentSpeakerReading = digitalRead(speakerPin);
  
  if(currentSpeakerReading == LOW){
    digitalWrite(speakerPin, HIGH);
    delay(500);
    MP3player.playTrack(mummy);//are you my mummy
  }
  else{
    MP3player.playTrack(sonic);//sonic sound
    delay(500);
    digitalWrite(speakerPin, LOW);
  } 
}

void turnOnPoliceSign(){
  digitalWrite(policeLightPin, HIGH);
}

void turnOffPoliceSign(){
  digitalWrite(policeLightPin, LOW);
}

void tardisSoundLightShow(){
  //start tardis sound
  MP3player.playTrack(tardis);
  tardisLightFade();
}

void tardisLightFade(){
  while(MP3player.isPlaying()){
      // fade in from min to max in increments of 5 points:
      for(int fadeValue = 10 ; fadeValue <= 255; fadeValue +=11) { 
        // sets the value (range from 0 to 255):
        analogWrite(ledPin, fadeValue);         
        // wait for 30 milliseconds to see the dimming effect    
        delay(30);                            
      } 
      delay(500);
      // fade out from max to min in increments of 5 points:
      for(int fadeValue = 255 ; fadeValue >= 10; fadeValue -=11) { 
        // sets the value (range from 0 to 255):
        analogWrite(ledPin, fadeValue);         
        // wait for 30 milliseconds to see the dimming effect    
        delay(30);                            
      } 
  } 
  //turn light off when done
  pinMode(ledPin, LOW);
}
