/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/* This sketch is enumerated as USB MIDI device. 
 * Following library is required
 * - MIDI Library by Forty Seven Effects
 *   https://github.com/FortySevenEffects/arduino_midi_library
 */

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
// read any new MIDI message
#define SW1 16
#define SW2 17



// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

// Create a new instance of the Arduino MIDI Library,
// and attach usb_midi as the transport.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  
  usb_midi.setStringDescriptor("TinyUSB MIDI");

  // Initialize MIDI, and listen to all MIDI channels
  // This will also call usb_midi's begin()
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // Attach the handleNoteOn function to the MIDI Library. It will
  // be called whenever the Bluefruit receives MIDI Note On messages.
  MIDI.setHandleNoteOn(handleNoteOn);

  // Do the same for MIDI Note Off messages.
  MIDI.setHandleNoteOff(handleNoteOff);

  Serial.begin(115200);

  // wait until device mounted
  while( !USBDevice.mounted() ) delay(1);
}

void loop()
{
  if(digitalRead(SW1) == HIGH)
    MIDI.sendNoteOn(69, 127, 1);
  else if(digitalRead(SW1) == LOW)
    MIDI.sendNoteOff(69, 0, 1);

  if(digitalRead(SW2) == HIGH)
    MIDI.sendNoteOn(76, 127, 2);
  else if (digitalRead(SW2) == LOW)
    MIDI.sendNoteOff(76, 0, 2);

  digitalWrite(LED_BUILTIN, digitalRead(SW1)); // blink with the built-in LED when SW1 is pressed
   
  // read any new MIDI messages
  MIDI.read();  
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  // Log when a note is pressed.
  Serial.printf("Note on: channel = %d, pitch = %d, velocity - %d", channel, pitch, velocity);
  Serial.println();
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  // Log when a note is released.
  Serial.printf("Note off: channel = %d, pitch = %d, velocity - %d", channel, pitch, velocity);
  Serial.println();
}
