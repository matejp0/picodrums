#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <stdlib.h>

#define SENSORPIN A2
#define STEP 12

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

volatile int note = 60; // middle C

float calibration;

int sensorValue = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  attachInterrupt(14, downOctave, FALLING);
  attachInterrupt(15, upOctave, FALLING);

  calibration = analogRead(SENSORPIN) - 100;
  
  MIDI.begin(MIDI_CHANNEL_OMNI);
    
  Serial.begin(115200);
  
  while(!USBDevice.mounted()) delay(1);
  
  digitalWrite(LED_BUILTIN, LOW);
      
}

void loop()
{
  sensorValue = analogRead(SENSORPIN);
  
  if(sensorValue >= calibration+50)
  {
    MIDI.sendNoteOff(note, 0, 1);
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  else
  {
    MIDI.sendNoteOn(note, 127, 1);
    digitalWrite(LED_BUILTIN, HIGH);
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
 *  float f;
 *  f = calibration/127;
 *  abs(ceil(sensorValue/f)-127)
 */
