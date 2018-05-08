int seq1[10]={9,3,2,4,3,2,7,8,9,11}; ///sequence of offsets
bool seq1play = false;
int seq1startstep = 0;
int seq1currentstep = 0;
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
  if(z == 1 && y > 0) {
    heldkeys[x][y] = 1;
    seq1offset = (y * 16) + x;
    seq1startstep = play_position;
    seq1play = true;
//    monome.led_set(x,y,15);
    dirty = true; 
  }
  else if (z == 0 &&  y > 0) {
    heldkeys[x][y] = 0;
    seq1offset = (y * 16) + x;
    seq1play = false;
//    monome.led_set(x,y,0);
    dirty = true; 
  }
 }


void polygomeRedraw() {
  monome.led_clear();
//  byte x = seq1offset % 16;
//  byte y = seq1offset - (16 * x) ;
  //draw current held keys
  for(byte x=0;x<16;x++){
      for(byte y=0;y<8;y++){
        if(heldkeys[x][y] == 1){
          monome.led_set(x,y,15);
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
