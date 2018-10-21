void readinputs(struct inputStates * data)
{
  // A bit of a funny statement below.
  // digitalRead return HIGH or LOW.
  // We want boolean (true/false) indicators of whether the button are
  // pushed.
  // And button inputs are active low - when a button is pushed, it'll read "LOW"
  // The right side of the expression checks if the input is equal to LOW, converting
  // that into a boolean indicator, stored in the array.  
  data->button[0] = (digitalRead(PIN_BTN0) == LOW);
  data->button[1] = (digitalRead(PIN_BTN1) == LOW);
  data->button[2] = (digitalRead(PIN_BTN2) == LOW);

  // Analog inputs have an LSB (out of 10 bits) or so noise, 
  // leading to "chatter" in the change detector logic.
  // Shifting off the 2 LSBs to remove it
  data->pot[0] = analogRead(PIN_POT0) >> 2;
  data->pot[1] = analogRead(PIN_POT1) >> 2;
}

void compareinputs(inputStates * old_p, inputStates * new_p)
{
  uint8_t idx;
//  steps_notesoffset = old_p->pot[1];
  for(idx = 0; idx < 3; idx++)
  {
    if(old_p->button[idx] != new_p->button[idx])
    {
      old_p->button[idx] = new_p->button[idx];
      if (old_p->button[idx] == true){
          if (idx == 0){
            if (currentMode >= 5) {
              currentMode = 0;
            } else {
            currentMode++;
            }
//        currentMode /= idx;
//        MIDI.sendControlChange(idx,old_p->button[idx], 2);
          MIDI.sendNoteOn(idx, 127, currentMode);
        } else if (idx == 1) {
          MIDI.sendNoteOn(idx, 127, currentMode);

        }else if (idx == 2){
          if (currentMode <= 0) {
            currentMode == 5;  
          } else {
            currentMode--;
          }
          MIDI.sendNoteOn(idx, 127, currentMode);

        }
        }
      else if (old_p->button[idx] == false){
        MIDI.sendNoteOn(idx, 0, 2);
      }
      
    }
  }

  for(idx = 0; idx < 2; idx++)
  {
    if(old_p->pot[idx] != new_p->pot[idx])
    {
      old_p->pot[idx] = new_p->pot[idx];
//      Serial.print("Pot #");
//      Serial.print(idx);
//      Serial.print(" changed to ");
//      Serial.println(old_p->pot[idx]);
      MIDI.sendControlChange(idx, old_p->pot[idx],2);
      if (idx == 0) { steps_notesoffset = (old_p->pot[idx]); }
      if (idx == 1) { steps_notesoffset = (old_p->pot[idx]); }
    }
  }
}



//void updateLEDs(inputStates * data)
//{
//  digitalWrite(PIN_LED_GRN, !data->button[0]);
//  digitalWrite(PIN_LED_RED, !data->button[1]);
//}


void getMidiData(){
    //read midishieldbuttondata
  readinputs(& newdata);
  compareinputs(& lastdata, & newdata);
//  updateLEDs(&newdata);
  static uint32_t loops = 0;  
  static uint8_t  ticks = 0;
  static uint8_t  prev_ticks = 0;
  if(  MIDI.read())
  {
    switch(MIDI.getType())
    {
      case midi::Clock :
      { 
        ticks++;
//        ticks++; ///doublespeed!!

//        Serial.print('.');
//        Serial.println(ticks);        
        
        if(ticks < 6)
        {
          digitalWrite(PIN_LED_TEMPO, LOW);
          //Serial.print('#'); 
  
        }
        else if(ticks == 6)
        {
          digitalWrite(PIN_LED_TEMPO, HIGH);
          //Serial.print("6ticks");
          if (playing) {
            next4();
          }
        }
        else if(ticks == 8)
        {
          if (playing) {
            next3();
          }
        }
        else if(ticks == 12)
        {
          if (playing) {
            next2();
            next4();
          }
        }
        else if(ticks == 16)
        {          
          if (playing) {
            next4();
          }
        }
        else if(ticks == 18)
        {
         if (playing) {
            next3();
          }
        }
        else if(ticks == 24)
        {   
          if (playing) {
            next();
            next2();
            next3();
            next4();
          }
          ticks=0;
        }
      }
      break;
      
      case midi::Start :
      {
        digitalWrite(PIN_LED_PLAYING, LOW);
        play_position = 15;
        play_position2 = 15;
        play_position3 = 15;
        play_position4 = 15;
        ticks = 0;
        playing = true;
//        reset();
        
        
//        Serial.println("Starting");
        startmode = 1;
        stopmode = 0;
        continuemode = 0;
        
      }
      break;

      case midi::Stop :
      {
        digitalWrite(PIN_LED_PLAYING, HIGH);
        prev_ticks = ticks;
        playing = false;
//        Serial.println("Stopping");
        startmode = 0;
        stopmode = 1;
        continuemode = 0;
        
      }
      break;
      case midi::Continue :
      {

        digitalWrite(PIN_LED_PLAYING, LOW);
        startmode = 0;
        stopmode = 0;
        continuemode = 1;
        
        // Restore the LED blink counter
        ticks = prev_ticks;
//        Serial.println("continuing");
      }
      break;
      case midi::NoteOn :
      {
        if (MIDI.getChannel() == notes_channel) {
           midiLed(MIDI.getData1(), MIDI.getData2());
        }
            
      }
      break;
      case midi::NoteOff :
      {
        if (MIDI.getChannel() == notes_channel) {
           midiLed(MIDI.getData1(), 0);
        }
            
      }
      break;
      
      default:
      break;
    }
  }

  loops++;
}

