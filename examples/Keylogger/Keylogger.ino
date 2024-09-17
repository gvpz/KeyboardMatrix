#include <KeyboardMatrix.h>

//rows
#define port23 PIO_PA14
#define port24 PIO_PA15
#define port25 PIO_PD0
#define port26 PIO_PD1
#define port27 PIO_PD2
#define port28 PIO_PD3
#define port29 PIO_PD6
#define port30 PIO_PD9

//coloumns
#define port31 PIO_PA7
#define port32 PIO_PD10
#define port33 PIO_PC1
#define port34 PIO_PC2
#define port35 PIO_PC3
#define port36 PIO_PC4
#define port37 PIO_PC5
#define port38 PIO_PC6
#define port39 PIO_PC7

const byte rows = 8;
const byte cols = 9;

PinMap rowPins[rows] = {
  {PIOA, port23}, {PIOA, port24}, {PIOD, port25}, {PIOD, port26}, 
  {PIOD, port27}, {PIOD, port28}, {PIOD, port29}, {PIOD, port30}
};

PinMap colPins[cols] = {
  {PIOA, port31}, {PIOD, port32}, {PIOC, port33}, {PIOC, port34}, {PIOC, port35},
  {PIOC, port36}, {PIOC, port37}, {PIOC, port38}, {PIOC, port39}
};

//  ~ = blank
//  ^ = shift
//  _ = space
//  < = backspace
//  * = capslock
char keymap[rows][cols] = {
  {'MENU','RIGHT','INSPECT','UP','TAB','~','*','^','~'},
  {'m','b','g','c','d','.','a','<','LEFT'},
  {',','n','v','f','x','/','s','WORDOUT','DOWN'},
  {'~','~','~','~','~','~','~','_','~'},
  {'o','u','h','t','e','k','z','<','1/2'},
  {'0','j','7','6','3','l','2','CANCEL',';'},
  {'-','9','8','5','4','=','1','Switch(TW/WP)','FILE'},
  {'p','i','y','r','w','+','q','ENTER',']'}
};

KeyboardMatrix keyboardMatrix;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup Start");

  keyboardMatrix.initializeMatrix(makeKeymap(keymap), rowPins, colPins, rows, cols);

  keyboardMatrix.enableClock(PIOA);
  keyboardMatrix.enableClock(PIOC);
  keyboardMatrix.enableClock(PIOD);

  keyboardMatrix.setDebounceTime(1);
  Serial.println("Debounce Time Set");

  Serial.println("Setup Complete");

}

void loop() {
  char key = keyboardMatrix.getKey();

  if (key == NO_KEY) return;
  Serial.println(key);
}