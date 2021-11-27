#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <stdlib.h>

#define PIN 16
#define NUM 4
#define STEP 12

bool old[NUM] = {HIGH};
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

volatile int note = 60;
int sensorPin = A2;
float calibration;
int sensorValue = 0;
float f;
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  attachInterrupt(14, upOctave, FALLING);
  attachInterrupt(15, downOctave, FALLING);

  calibration = analogRead(A2);
  // usb_midi.setStringDescriptor("TinyUSB MIDI");
  MIDI.begin(MIDI_CHANNEL_OMNI);
    
  Serial.begin(115200);

  while(!USBDevice.mounted()) delay(1);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  
  f = calibration/127;
}

void loop()
{
  sensorValue = analogRead(sensorPin);
  if(sensorValue >= calibration)
    MIDI.sendNoteOff(note, 0, 1);
  else {
    MIDI.sendNoteOn(note, 127, 1);
    // abs(ceil(sensorValue/f)-127)
  }
  
}
void upOctave(){
  for(int i = 0; i<NUM; i++)
   MIDI.sendNoteOff(note + i, 0, 1+i);
  note += (note<=(127-STEP)) ? STEP : 0;
}
void downOctave(){
  for(int i = 0; i<NUM; i++)
    MIDI.sendNoteOff(note + i, 0, 1+i);
  note -= (note>=(STEP)) ? STEP : 0;
}
