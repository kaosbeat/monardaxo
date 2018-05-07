#include <Usb.h>  
#include "MonomeController.h"
#include <MIDI.h>


//MIDISHIELD PINS
#define PIN_LED_PLAYING 6
#define PIN_LED_TEMPO 7
#define PIN_PLAY_INPUT 2
#define PIN_CONTINUE_INPUT 3
#define PIN_TEMPO_POT 1

static const uint8_t PIN_BTN0 = 2;
static const uint8_t PIN_BTN1 = 3;
static const uint8_t PIN_BTN2 = 4;

static const uint8_t PIN_POT0 = 0;
static const uint8_t PIN_POT1 = 1;
///END MIDISHIELD PINS

typedef struct inputStates
{
  uint8_t button[3];
  uint8_t pot[2];
}inputStates;

inputStates lastdata;
inputStates newdata;


static const uint16_t DEBOUNCE_COUNT = 50;


MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);
USBHost usb;
MonomeController monome(usb);

byte step[6][16];
byte notes[16][127]; /// notesplaying 16 channels / 127 notes

boolean dirty;
byte play_position;
byte next_position;
boolean cutting;
byte keys_held, key_last;
byte loop_start, loop_end = 15;

unsigned long t = millis();
unsigned long interval = 200;


// SETUP //////////////////

void setup() 
{
  // LED outputs for MIDI debug
  pinMode(PIN_LED_PLAYING, OUTPUT);
  pinMode(PIN_LED_TEMPO, OUTPUT);
  digitalWrite(PIN_LED_PLAYING, HIGH);
  digitalWrite(PIN_LED_TEMPO, HIGH);
#if 1
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
#endif  
//  Serial.begin(115200);
//  Serial.print("\r\ninitialized.\r\n");
//  delay(200);

  //monomesetup
  monome.SetConnectCallback(&ConnectCallback);
  monome.SetGridKeyCallback(&GridKeyCallback);

  //midishieldbuttonsetup
  pinMode(PIN_BTN0, INPUT_PULLUP);
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);

//  pinMode(PIN_LED_GRN, OUTPUT);
//  pinMode(PIN_LED_RED, OUTPUT);
  readinputs(&lastdata);

}


// CONNECT //////////////////
void ConnectCallback(const char * name, byte cols, byte rows) {
//  Serial.print("\r\nmonome device connected!\r\n\n");
}

// KEY //////////////////


void GridKeyCallback(byte x, byte y, byte z) {
  stepsKey(x,y,z);
}




void loop() 
{
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

        //Serial.print('.');
//        Serial.println(ticks);        
        
        if(ticks < 6)
        {
          digitalWrite(PIN_LED_TEMPO, LOW);
          //Serial.print('#');       
        }
        else if(ticks == 6)
        {
          digitalWrite(PIN_LED_TEMPO, HIGH);
          next();
        }
        else if(ticks >= 24)
        {
          ticks = 0;
//          Serial.print('\n');
        }
      }
      break;
      
      case midi::Start :
      {
        digitalWrite(PIN_LED_PLAYING, LOW);
        ticks = 0;
//        Serial.println("Starting");
      }
      break;

      case midi::Stop :
      {
        digitalWrite(PIN_LED_PLAYING, HIGH);
        prev_ticks = ticks;
//        Serial.println("Stopping");
      }
      break;
      case midi::Continue :
      {

        digitalWrite(PIN_LED_PLAYING, LOW);

        // Restore the LED blink counter
        ticks = prev_ticks;
//        Serial.println("continuing");
      }
      break;
      
      default:
      break;
    }
  }

  loops++;


  ///monomestuff
  usb.Task();
  // check timer for next step
  if(millis() - t > interval) {
    t = millis();
//    next();
  }
  
  // redraw if dirty
  if(dirty) {
    redraw();
    monome.refresh();
    dirty = false;
  }


  
}

  


// REDRAW ////////////////// 
void redraw() {
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

// NEXT ////////////////// 
void next() {
  killNotes();

  if(cutting)
    play_position = next_position;
  else if(play_position == 15)
    play_position = 0;
  else if(play_position == loop_end)
    play_position = loop_start;
  else 
      play_position++;
  
  cutting = false;
    
  
  // TRIGGER SOMETHING
  for(byte y=0;y<6;y++)
    if(step[y][play_position] == 1)
      trigger(y);
  
  dirty = true;
}

// TRIGGER ////////////////// 
void trigger(byte i) {
   MIDI.sendNoteOn(i, 127, 1);
//   Serial.print(i);
   notes[1][i] = 1;  ///set notes playing on channel/note

}


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
