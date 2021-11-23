#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#define PIN 16
#define NUM 4

bool old[NUM] = {HIGH};
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

volatile int note = 60;
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  for(int i = 0; i<NUM; i++){
    pinMode((PIN + i), INPUT_PULLUP);
  }
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  attachInterrupt(14, upOctave, FALLING);
  attachInterrupt(15, downOctave, FALLING);
  
  // usb_midi.setStringDescriptor("TinyUSB MIDI");
  MIDI.begin(MIDI_CHANNEL_OMNI);
    
  Serial.begin(115200);

  while(!USBDevice.mounted()) delay(1);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  for(int i = 0; i<NUM; i++)
  {
    bool nev = digitalRead(PIN + i);
    if(old[i] != nev)
    {
      if(nev == LOW){
        MIDI.sendNoteOn(note + i, 127, 1+i);
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else{
        MIDI.sendNoteOff(note + i, 0, 1+i);
        digitalWrite(LED_BUILTIN, LOW);
      }
      old[i] = nev;
    }
  }
}
void upOctave(){
  if(note<=(127-12)){
    for(int i = 0; i<NUM; i++)
      MIDI.sendNoteOff(note + i, 0, 1+i);
    note += 12;
  }
}
void downOctave(){
  if(note>=(12)){
    for(int i = 0; i<NUM; i++)
      MIDI.sendNoteOff(note + i, 0, 1+i);
    note -= 12;
  }
}
