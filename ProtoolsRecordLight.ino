#include <MIDI.h>

//Flash arduino 8u2 or 16u2 chip with HIDUINO_MIDI.hex
//Set ProTools or other DAW to use MIDI controller / HUI protocol / arduino_midi (default hiduino name)
//The two sets of messages that we are looking for are:
// RECORD ON
// 1) channel 1,  controller 12, value 14 
// 2) channel 1,  controller 44, value 69
// RECORD OFF
// 1) channel 1,  controller 12, value 14 
// 2) channel 1,  controller 44, value 5

int firstMsg=0;  // set int for the first part of the two messages

MIDI_CREATE_DEFAULT_INSTANCE(); 

void setup(){
//Use LED for reference
pinMode(13,OUTPUT); 
// Listen for MIDI on channel 1 only
MIDI.begin(1); 
//Listen for incoming ControlChange MIDI messages and pass bytes to function if received
MIDI.setHandleControlChange(CCSequence); 
}

void loop() {
MIDI.read(); // Read all incoming MIDI data
}

//Function that will receive the CC MIDI Bytes
void CCSequence(byte channel, byte controller, byte value  ) { 
  //Check if CC message is equal to the first part of our two sequences:
  //1) channel 1,  controller 12, value 14 
  if((channel==1)&&(controller=12)&&(value==14))
    {
      //Mark first part of the sequence as received
      firstMsg=1; 
    }
    
  //If first part is received look for the second part of our two sequences
  if(firstMsg==1)
      {
        //Check if CC message is the second part of RECORD ON
        //2) channel 1,  controller 44, value 69
        if((channel==1)&&(controller=44)&&(value==69))
          {
            //Turn LED ON
            digitalWrite(13, HIGH);
            //Reset Marker. We start waiting for the first part of the sequence again
            firstMsg=0;
          } 
        //Check if CC message is the second part of RECORD OFF
        //2) channel 1,  controller 44, value 5
        if((channel==1)&&(controller=44)&&(value==5))
          {
             //Turn LED OFF
             digitalWrite(13, LOW);
             //Reset Marker. We start waiting for the first part of the sequence again
             firstMsg=0;
          }
      }
}