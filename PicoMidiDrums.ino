#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#define PIN 16
#define NOTE 62
#define NUM 4

bool old[NUM] = {HIGH};
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  for(int i = 0; i<NUM; i++){
    pinMode((PIN + i), INPUT_PULLUP);
  }
  
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
        MIDI.sendNoteOn(NOTE + i, 127, 1+i);
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else{
        MIDI.sendNoteOff(NOTE + i, 0, 1+i);
        digitalWrite(LED_BUILTIN, LOW);
      }
      old[i] = nev;
    }
  }
}
