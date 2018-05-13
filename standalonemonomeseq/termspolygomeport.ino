const int seq1size = 10;
float seq1[seq1size]={8,16,12,0,15,5,12,8,-16,20}; ///sequence of offsets
bool seq1play = false;
bool freezekeys = false;
int seq1from = 0;
int seq1till = 0;
int seq1step = 0;
int seq1offset = 0;
byte heldkeys[16][8];
//byte seq1pos[


///when keydown get seq1
//log startstep / seq1play = true
//play note seq[step - startstep]
//on next()
//play note seq[step - startstep]
//[...]
//if keyup
//seq1play = false


//void playSeqNote(step, seq){}

//void polygomeInit(){}

void polygomeKey(byte x,byte y,byte z, int play_position){  
  if(z == 1 && x==0 && y==0) {
    if (freezekeys == false) {
      freezekeys = true;
      monome.led_set(0,0,15);
    } else {
      freezekeys = false;
      monome.led_set(0,0,0);
    }
    
  }
  if (freezekeys == false) { ////start updating shizzle
    if(z == 1 && y > 0) {
      heldkeys[x][y] = 1+play_position;
      seq1play = true;
      dirty = true; 
    }
    else if (z == 0 &&  y > 0) {
      heldkeys[x][y] = 0;
      seq1play = false;
      dirty = true; 
    }
  }
 }


void polygomeRedraw() {
  monome.led_clear();
//  byte x = seq1offset % 16;
//  byte y = seq1offset - (16 * x) ;
  //from current step draw with overflow currently held keys
  
  //draw current held keys, unfrozen state

//  currentseqstep = play_position (heldkeys[x][y] - 1 + seq1.size())
  for(byte x=0;x<16;x++){
      for(byte y=0;y<8;y++){
        if(heldkeys[x][y] > 0){
          //vars needed
          //play_position 
          seq1from = heldkeys[x][y] - 1;
          seq1till = (seq1from + seq1size)%16;
          if ((play_position >= seq1from && play_position < seq1till) && (seq1from < seq1till)) ///case without overflow
            {
             monome.led_set(x,y,15);
             seq1step = play_position - seq1from;
             for (int i=0; i< seq1step; i++){
                byte note = seq1[i]+gridXYtoNoteNumber(x,y);
                monome.led_set(noteNumberToGridX(note),noteNumberToGridY(note),15);
                if (i == seq1step-1){
                  MIDI.sendNoteOn(note, 127, 2);
                  notes[2][note] = 1;  /// set on channel two for polygome
                }
             }
            }
          else if ((play_position >= seq1from || play_position < seq1till) && (seq1from > seq1till)) ///case with overflow
            {
             monome.led_set(x,y,15);
             if (play_position >= seq1from) {
              seq1step = play_position - seq1from;
             }
             if (play_position < seq1till) {
              seq1step = 16 - seq1from + play_position;
             }
             for (int i=0; i< seq1step; i++){
                byte note = seq1[i]+gridXYtoNoteNumber(x,y);
                monome.led_set(noteNumberToGridX(note),noteNumberToGridY(note),15);
                if (i == seq1step-1){
                  MIDI.sendNoteOn(note, 127, 2);
                  notes[2][note] = 1;  /// set on channel two for polygome
                }
             }
            }
        }
      }
  }
  //draw current offset step key
//  x = (seq1offset + seq1[seq1currentstep])%16;
//  y = (seq1offset + seq1[seq1currentstep]) - (16 *x);
//  monome.led_set(x,y,15);
}


void polygomeTrigger() {
//  seq1currentstep = play_position - seq1startstep; ///potentially crashing if < than 0

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

