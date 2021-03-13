/*MSE Sound sketch
13th March 2021

MrBaddeley printed droids - www.patreon.com/mrbaddeley www.facebook.com/groups/mrbaddeley
This sketch is to control the sound on the full scale MSE-6 Mouse Droid
This code is using a Nano, AltSoftserial on pins 10 and 11, and a DF Player. 

AltSoftSerial has sorted a few issues with interupts out for me on the code, allowing me to accurately read all the inputs. 

I'm using channel 5 linked to the VRA on the Flysky FS-I6X as the volume control on pin 3
Pins 4 through to 7 are linked to SWA, SWB, SWC and SWD on the Flysky (sw7-A sw8-B sw9-C sw10-D), importantly sw9 is the multi position switch.

The code plays the first MP3 when you change SWA, the second on SWB and the third on SWD. 

SWC (the multi position switch) shift mode, at the top, it kills the volume, at the middle it ENABLES the other switches, at the bottom it plays the fourth MP3 file on the sdcard (I used the long loop for this). 
This means at the bottom position it just plays the long loop, push it up to kill the sound and in the middle to use the switches. 

I've also used VRA (on channel 5) for volume control. 

Libraries used:

https://github.com/DFRobot/DFRobotDFPlayerMini
https://www.arduino.cc/reference/en/libraries/altsoftserial/

Not perfect, but should get people started. 

*/
#include "Arduino.h" //   libraries for DFplayer and SerialSoftware//
#include <AltSoftSerial.h> //load the AltSoftSerial library
#include "DFRobotDFPlayerMini.h"

AltSoftSerial altSerial;
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

int sw7; // to store data from the RC switchs (channels 7-10 & 5 for the volume). 
int sw8;
int sw9;
int sw10;
int readvol5;
int DFvol;

int sw10pin = 7; // set the Arduino pins for reading the RC
int sw9pin = 6;
int sw8pin = 5;
int sw7pin = 4;
int readvol5pin = 3;


int soundstate7 =1; //set variables for the states of the sound
int prevsoundstate7 =1;
int soundstate8 =1; 
int prevsoundstate8 =1;
int soundstate10 =1; 
int prevsoundstate10 =1;
int soundstate9;
int prevsoundstate9;



void setup() {
  // put your setup code here, to run once:

    altSerial.begin(9600);
  Serial.begin(115200); // Starts HW serial  @ 115200, used for Serial monitor to diagnose
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)")); // prints some starting messages. 
  
  if (!myDFPlayer.begin(altSerial)) {  //Use softwareSerial to communicate with mp3 and checks link.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3 plays the first MP3 to indicate it's all working. 

}

void loop() {
  // put your main code here, to run repeatedly:
  


sw7 = pulseIn (sw7pin, HIGH); //These read the status of the 5 switches
sw8 = pulseIn (sw8pin, HIGH);
sw9 = pulseIn (sw9pin,HIGH);
sw10 = pulseIn (sw10pin,HIGH);
readvol5 = pulseIn (readvol5pin,HIGH);

Serial.println (soundstate9);


//This first section checks the 3 position switch. if it's at the top (state 0) 
//no sound will be played, middle position, the other three switches trigger three different sounds, at the bottom it plays the long loop. 

soundstate9 = 0;

if (sw9 > 1300) {
soundstate9 = 1;  
}

if (sw9 > 1800) {
  soundstate9 = 2;
}

Serial.print (sw7); //diagnostic serial prints
Serial.print (" ");
Serial.print (sw8);
Serial.print (" ");
Serial.print (sw9);
Serial.print (" ");
Serial.print (sw10);
Serial.print (" ");
Serial.print (readvol5);
Serial.print (" ");
Serial.println (soundstate9);


if (soundstate9 == 2) { //This triggers the long loop. 

if (soundstate9 != prevsoundstate9){
    prevsoundstate9 = soundstate9;
    myDFPlayer.play(4);  //Play the long loop
}
}

if (soundstate9==0){ //THis kills the volume
  prevsoundstate9 = soundstate9;
  readvol5=1000;
    
   
}

if (soundstate9==1){ //This enables the other switch sounds.
  if (soundstate9 != prevsoundstate9){
    prevsoundstate9 = soundstate9;
    myDFPlayer.pause();
}
}

if (sw7< 1500 && soundstate9==1){    //If switch 7 changes state, play the first sound
    soundstate7=1;
  }
   if (sw7> 1500 && soundstate9==1){
    soundstate7=2;
  }
  if (soundstate7 != prevsoundstate7){
    prevsoundstate7 = soundstate7;
    myDFPlayer.pause();
    myDFPlayer.play(1);  //Play the first mp3
  }

if (sw8< 1500 && soundstate9==1){   //if switch 8 changes state, play the second sound
    soundstate8=1;
  }
   if (sw8> 1500 && soundstate9==1){
    soundstate8=2;
  }
  if (soundstate8 != prevsoundstate8){
    prevsoundstate8 = soundstate8;
    myDFPlayer.pause();
    myDFPlayer.play(2);  //Play the first mp3
  }

if (sw10< 1500 && soundstate9==1){ //if switch 10 changes state, play the third sound
    soundstate10=1;
  }
   if (sw10> 1500 && soundstate9==1){
    soundstate10=2;
  }
  if (soundstate10 != prevsoundstate10){
    prevsoundstate10 = soundstate10;
    myDFPlayer.pause();
    myDFPlayer.play(3);  //Play the first mp3
  }


DFvol = map(readvol5,950,2050,0,30); //This maps the volume from channel 5

myDFPlayer.volume(DFvol);  //Set volume value from the channel 5 (potentimeter)
    
  

}
