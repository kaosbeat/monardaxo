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
