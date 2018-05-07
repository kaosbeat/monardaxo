void stepsKey(byte x,byte y,byte z){  
  // toggle steps
  if(z == 1 && y < 6) {
    step[y][x] ^= 1;
    dirty = true; 
  }
  else if(y == 7) {
    // track number of keys held
    keys_held = keys_held + (z*2) - 1;
      
    // cut
    if(z == 1 && keys_held == 1) {
      cutting = true;
      next_position = x;
      key_last = x;
    }
    // set loop points
    else if(z == 1 && keys_held == 2) {
      loop_start = key_last;
      loop_end = x;
    }
  }
 }

 

// REDRAW ////////////////// 
void stepsRedraw() {
  monome.led_clear();
  
  // draw toggles with play bar
  byte highlight;
  for(byte x=0;x<16;x++) {
    if(x == play_position)
      highlight = 4;
    else
      highlight = 0;
      
    for(byte y=0;y<6;y++)
      monome.led_set(x,y,step[y][x] * 11 + highlight);
  }
  
  // draw trigger row and on-triggers
  for(byte x=0;x<16;x++)
    monome.led_set(x,6,4);
  for(byte y=0;y<6;y++)
    if(step[y][play_position] == 1)
      monome.led_set(y,6,15);
      
  // draw playback position
  monome.led_set(play_position,7,15);
}


// TRIGGER ////////////////// 
void stepsTrigger(byte i) {
   MIDI.sendNoteOn(i, 127, 1);
//   Serial.print(i);
   notes[1][i] = 1;  ///set notes playing on channel/note

}




/////util for this program

void killNotes(){
    for(byte ch=0;ch<16;ch++){
      for(byte nt=0;nt<127;nt++){
        if(notes[ch][nt] == 1){
          MIDI.sendNoteOn(nt, 0, ch);
          notes[ch][nt] = 0;
        }
      }
    }
  }


