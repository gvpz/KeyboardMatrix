//This code is used for an escape room.  
//Therefore, the keywords required to solve the puzzle have been redacted.

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

#define port44 PIO_PC19 //REDACTED1
#define port45 PIO_PC18 //REDACTED2

const byte rows = 8;
const byte cols = 9;

const int holdTime = 1000;
int startTime = 0;

const byte listMax = 10;
const byte redacted1Length = 6;
const byte redacted2Length = 7;
const byte redacted1[redacted1Length] = {?, ?, ?, ?, ?, ?};
const byte redacted2[redacted2Length] = {?, ?, ?, ?, ?, ?, ?};

byte currentWord[listMax];

PinMap rowPins[rows] = {
  {PIOA, port23}, {PIOA, port24}, {PIOD, port25}, {PIOD, port26}, 
  {PIOD, port27}, {PIOD, port28}, {PIOD, port29}, {PIOD, port30}
};

PinMap colPins[cols] = {
  {PIOA, port31}, {PIOD, port32}, {PIOC, port33}, {PIOC, port34}, {PIOC, port35},
  {PIOC, port36}, {PIOC, port37}, {PIOC, port38}, {PIOC, port39}
};

PinMap redacted1Output = {PIOC, port44};
PinMap redacted2Output = {PIOC, port45};

//  ~ = blank
//  ^ = shift
//  _ = space
//  < = backspace
//  * = capslock
char keymap[rows][cols] = {
  {'MENU','RIGHT','INSPECT','UP','TAB','~','*','^','~'}, //0
  {'m','b','g','c','d','.','a','<','LEFT'},              //1
  {',','n','v','f','x','/','s','WORDOUT','DOWN'},        //2
  {'~','~','~','~','~','~','~','_','~'},                 //3
  {'o','u','h','t','e','k','z','<','1/2'},               //4
  {'0','j','7','6','3','l','2','CANCEL',';'},            //5
  {'-','9','8','5','4','=','1','Switch(TW/WP)','FILE'},  //6
  {'p','i','y','r','w','+','q','ENTER',']'}              //7
};

KeyboardMatrix keyboardMatrix;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup Start");

  keyboardMatrix.initializeMatrix(makeKeymap(keymap), rowPins, colPins, rows, cols);

  keyboardMatrix.enableClock(PIOA);
  keyboardMatrix.enableClock(PIOC);
  keyboardMatrix.enableClock(PIOD);

  redacted1Output.port -> PIO_CODR = redacted1Output.pin;
  redacted2Output.port -> PIO_CODR = redacted2Output.pin;


  keyboardMatrix.setDebounceTime(1);
  Serial.println("Debounce Time Set");

  Serial.println("Setup Complete");

}

void addToFront(byte newNumber) {
  for (int i = listMax - 1; i > 0; --i) {
    currentWord[i] = currentWord[i - 1];
  }
  currentWord[0] = newNumber;
}

bool checkForRedacted1(){
  for(int i = 0; i < redacted1Length; i++){
    if(currentWord[i] != redacted1[i]) return false;
  }
  return true;
}

bool checkForRedacted2(){
  for(int i = 0; i < redacted2Length; i++){
    if(currentWord[i] != redacted2[i]) return false;
  }
  return true;
}

void loop() {
  int row = keyboardMatrix.getRow();

  if(row == -1) return;
  startTime = millis();

  if(row == currentWord[0]) return;
  addToFront(row);

  if(checkForRedacted2()){
    redacted2Output.port -> PIO_SODR = redacted2Output.pin;
    Serial.println("REDACTED2");
  }
  else if(checkForRedacted1()){
    redacted1Output.port -> PIO_SODR = redacted1Output.pin;
    Serial.println("REDACTED1");
  }

  for(int i = 0; i < 9; i++){
    Serial.print(currentWord[i]);
  }
  Serial.println();
}
