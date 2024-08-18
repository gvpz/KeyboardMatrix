#include <KeyboardMatrix.h>

const byte rows = 8;
const byte cols = 9;

PinMap rowPins[rows] = {
  {PIOA, PIO_PA14}, {PIOA, PIO_PA15}, {PIOD, PIO_PD0}, {PIOD, PIO_PD1}, 
  {PIOD, PIO_PD2}, {PIOD, PIO_PD3}, {PIOD, PIO_PD6}, {PIOD, PIO_PD9}
};

PinMap colPins[cols] = {
  {PIOA, PIO_PA7}, {PIOD, PIO_PD10}, {PIOC, PIO_PC1}, {PIOC, PIO_PC2}, {PIOC, PIO_PC3},
  {PIOC, PIO_PC4}, {PIOC, PIO_PC5}, {PIOC, PIO_PC6}, {PIOC, PIO_PC7}
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
  Serial.println("PIOA Clock Enabled");
  keyboardMatrix.enableClock(PIOC);
  Serial.println("PIOC Clock Enabled");
  keyboardMatrix.enableClock(PIOD);
  Serial.println("PIOD Clock Enabled");

  keyboardMatrix.setDebounceTime(1);
  Serial.println("Debounce Time Set");

  Serial.println("Setup Complete");

  keyboardMatrix.testPins();
}

void loop() {
  char key = keyboardMatrix.getKey();

  if (key) {
    Serial.print("KEY DETECTED: ");
    Serial.println(key);
  }
}