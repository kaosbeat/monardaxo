////regullar notes sending app

byte notes_state[16][8];



void notesKey(byte x,byte y,byte z){
    midiLed(gridXYtoNoteNumber(x, y),z);
    dirty = true;
}


void midiLed(byte note,byte velocity) {
  if (velocity > 0) {
    notes_state[noteNumberToGridX(note)][noteNumberToGridY(note)] = 1;
    MIDI.sendNoteOn(note, 100, notes_channel);
  }
  else {
    notes_state[noteNumberToGridX(note)][noteNumberToGridY(note)] = 0;
//    MIDI.sendNoteOn(note, 0, notes_channel);
    MIDI.sendNoteOff(note, 100, notes_channel);
  }
}

void notesRedraw() {
  monome.led_clear();
   for(byte x=0;x<16;x++){
      for(byte y=0;y<8;y++){
        if(notes_state[x][y] == 1){
          monome.led_set(x,y,15);
        }
      }
   }
}
