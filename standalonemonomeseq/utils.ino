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


