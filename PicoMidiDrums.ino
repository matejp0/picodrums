#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#define SW1 16
#define SW2 17

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  
  usb_midi.setStringDescriptor("TinyUSB MIDI");
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);

  while( !USBDevice.mounted() ) delay(1); // wait until device mounted
}

void loop()
{
  if(digitalRead(SW1) == HIGH)  MIDI.sendNoteOn(69, 127, 1);
  else if(digitalRead(SW1) == LOW)  MIDI.sendNoteOff(69, 0, 1);

  if(digitalRead(SW2) == HIGH)  MIDI.sendNoteOn(76, 127, 2);
  else if (digitalRead(SW2) == LOW) MIDI.sendNoteOff(76, 0, 2);

  digitalWrite(LED_BUILTIN, digitalRead(SW1)); // blink with the built-in LED when SW1 is pressed
}
