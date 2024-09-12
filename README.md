# KeyboardMatrix
In Progress<br/><br/>

I made this library because the Keypad.h library for Arduino is not fast enough, causing my keylogger to often read a different key in the same row.<br/><br/>

Arduino Due utilizing pins 23-39 <br/>
Wires connected to the bottom of the 17-pin keyboard matrix on Brother WP2600Q Word Processor PCB<br/>
Ground pin of Arduino connected to Ground of PCB<br/>
All wires other than ground have 1N4148 Diodes<br/><br/>

23-30: Cathode towards PCB<br/>
31-39: Cathode towards Arduino<br/><br/>

component_pio.h is included because when importing this library, my Arduino IDE was unable to find it inside its own files
