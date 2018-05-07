# monardaxo
monome - arduino - axoloti > standalone setup


## features
- external clokc sync
- demo arduino app

## wanted features
- multiple apps running in pages style selection mode
- Terms-polygome style app
- polygome withe gomeizer
- multiple scales for midi grid-keyboard

## introduction
I've been trying numerous times to create a "no-laptop" setup for the monome:
 - first using only a raspberry pi (during the 1st gen RPi days) but having numerous disconnect errors
 - then trying to implement the monome serial protocol on an axoloti
 - then using an arduino witrh serial port and some hacking
 - even using a miniPC which felt too much like a laptop and had essentially the same drawbacks

Seeing the grid studies or arduino, I finally made the jump to the arduino DUE, with USB hist capabilities and fantastic MonomeHost library https://github.com/monome/MonomeHost I felt confident enough I could start making something that is good enough for live shows. It was fairly easy to combine this with a sparkfun arduino MIDI shield I had lying around (https://github.com/sparkfun/MIDI_Shield/), I was playing out MIDI (great intro on the MIDI protocol https://learn.sparkfun.com/tutorials/midi-tutorial/all) from the monome sequencer within one hour of toying around. 
Now Just sending it to ableton will not solve a whole lot of problems, so to finish this of I'm looking to make a descent axoloti companion-patch

## setup
### needed hardware
- monome (https://monome.org/) I have a walnut 128
- arduino DUE (https://store.arduino.cc/arduino-due)
- sparkfun midi shield (or brew your own) https://www.sparkfun.com/products/12898
- axoloti (http://www.axoloti.com/) open-source DSP hardware 
- OTG usb cable (for connecting the monome to the DUE)
- two midi cables (DIN5)

### needed software
- https://github.com/FortySevenEffects/arduino_midi_library


## status - hello world! v0.1
- simple sequencer from gridstudies (copy paste https://monome.org/docs/grid-studies/arduino/)
- external midi clock sets speed for sequencer!
- simple axo test patch (contains clock and monophonic sine synth)










