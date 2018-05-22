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


boolean dirty;
byte play_position;
byte next_position;
boolean cutting;
byte keys_held, key_last;
byte loop_start, loop_end = 15;
byte step[6][16];
byte notes[16][127]; /// notesplaying 16 channels / 127 notes
int notes_channel = 16; //midi channel for notes app

unsigned long t = millis();
unsigned long interval = 200;

int currentMode = 0;  //default 0 = steps, 1 = polygome (needed for draw/update/keys/...)

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
  if (currentMode == 0)
    stepsKey(x,y,z);
  else if (currentMode == 1)
    polygomeKey(x,y,z,play_position);
  else if (currentMode == 2)
    notesKey(x,y,z);
}




void loop() 
{
getMidiData();   //// all incoming midi processing is done in midishielfunctions

  ///monomestuff
  usb.Task();

  
  // redraw if dirty
  if(dirty) {
    if (currentMode == 0)
      stepsRedraw();
    if (currentMode == 1)
      polygomeRedraw();  
    if (currentMode == 2)
      notesRedraw();  
    monome.refresh();
    dirty = false;
  }


  
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
    
  
  // TRIGGER SOMETHING for STEPS
  for(byte y=0;y<6;y++)
    if(step[y][play_position] == 1)
      stepsTrigger(y);

  //TRIGGER for polygome
    polygomeTrigger();
  
  dirty = true;
}
  


