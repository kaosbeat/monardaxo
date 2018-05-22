///multiprogramutils

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


byte gridXYtoNoteNumber(byte x, byte y) {
  byte note = y*16 + x;
  return note;
}

byte noteNumberToGridX(byte note) {
  byte x = note % 16;
  return x;
}

byte noteNumberToGridY(byte note) {
  byte x = note % 16;
  byte y = (note - x) / 16 ;
  return y;
}
