#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <stdlib.h>

#define SENSORPIN A2
#define STEP 12
#define CALBUTTON 16

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

volatile int note = 60; // middle C

int lowestValue;
int highestValue;
float f;
int sensorValue = 0;
bool notestate = false;
void setup()
{
    
  MIDI.begin(MIDI_CHANNEL_OMNI);
  // Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  pinMode(14, INPUT_PULLUP);  // octave changing
  pinMode(15, INPUT_PULLUP);  // buttons
  attachInterrupt(14, downOctave, FALLING);
  attachInterrupt(15, upOctave, FALLING);

  pinMode(CALBUTTON, INPUT_PULLUP); // highestValue button

  lowestValue = analogRead(SENSORPIN);

  while(digitalRead(CALBUTTON) == 1) delay(1);
  if(digitalRead(CALBUTTON) == 0) highestValue = analogRead(SENSORPIN);
  
  f = ((highestValue-lowestValue)/127);

  while(!USBDevice.mounted()) delay(1);
  digitalWrite(LED_BUILTIN, LOW);
      
}
int getMIDIVal(int pin){
  int a = (analogRead(pin)-lowestValue)/f;
  return ((a>127) ? 127 : a);
}
void loop()
{
  sensorValue = analogRead(SENSORPIN);
  if(sensorValue >= lowestValue + 50)
  {
    if(!notestate)  MIDI.sendNoteOn(note, getMIDIVal(SENSORPIN), 1);
    else  MIDI.sendAfterTouch(note, getMIDIVal(SENSORPIN), 1);
    
    digitalWrite(LED_BUILTIN, HIGH);
    notestate = true;
  }
  
  else if(sensorValue <= lowestValue - 30){
    MIDI.sendNoteOff(note, 0, 1);
    digitalWrite(LED_BUILTIN, LOW);
    notestate = false;
    
  }
  
}
void upOctave(){
  for(int i = 0; i<1; i++)
   MIDI.sendNoteOff(note + i, 0, 1+i);
  note += (note<=(127-STEP)) ? STEP : 0;
}
void downOctave(){
  for(int i = 0; i<1; i++)
    MIDI.sendNoteOff(note + i, 0, 1+i);
  note -= (note>=(STEP)) ? STEP : 0;
}

/*  TODO:
 *  
 *  abs(ceil(sensorValue/f)-127)
 */
