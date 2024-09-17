#include <KeyboardMatrix.h>

const byte rows = 8;
const byte cols = 9;

PinMap rowPins[rows] = {
  {PIOA, PIO_PA14}, {PIOA, PIO_PA15}, {PIOD, PIO_PD0}, {PIOD, PIO_PD1}, 
  {PIOD, PIO_PD2}, {PIOD, PIO_PD3}, {PIOD, PIO_PD6}, {PIOD, (1u << 9)}
  };

PinMap colPins[cols] = {
  {PIOA, PIO_PA7}, {PIOD, PIO_PD10}, {PIOC, PIO_PC1}, {PIOC, PIO_PC2}, {PIOC, PIO_PC3},
  {PIOC, PIO_PC4}, {PIOC, (1u << 5)}, {PIOC, PIO_PC6}, {PIOC, PIO_PC7}
};

//  ~ = blank
//  ^ = shift
//  _ = space
//  < = backspace
//  * = capslock
char keymap[rows][cols] = {
  {'MENU','RIGHT','INSPECT','UP','TAB','~','*','^','~',},
  {'m','b','g','c','d','.','a','<','LEFT',},
  {',','n','v','f','x','/','s','WORDOUT','DOWN',},
  {'~','~','~','~','~','~','~','_','~',},
  {'o','u','h','t','e','k','z','<','1/2',},
  {'0','j','7','6','3','l','2','CANCEL',';',},
  {'-','9','8','5','4','=','1','Switch(TW/WP)','FILE',},
  {'p','i','y','r','w','+','q','ENTER',']',}
};

//Creates object; calls constructor
KeyboardMatrix keyboard(makeKeymap(keymap), rowPins, colPins, rows, cols);



void setup() {
  Serial.begin(9600);

  keyboard.enableClock(PIOA);
  keyboard.enableClock(PIOC);
  keyboard.enableClock(PIOD);
  keyboard.setDebounceTime(1);
}

void loop() {
  char key = keyboard.getKey();

  if(key){
    Serial.print("KEY DETECTED: ");
    Serial.println(key);
  }
}

