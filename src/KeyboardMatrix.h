#ifndef KEYBOARD_MATRIX_H
#define KEYBOARD_MATRIX_H

#include "Key.h"
#include "sam.h"
#include "component_pio.h"

#ifndef INPUT_PULLUP
#warning "Using  pinMode() INPUT_PULLUP AVR emulation"
#define INPUT_PULLUP 0x2
#define pinMode(_pin, _mode) _mypinMode(_pin, _mode)
#define _mypinMode(_pin, _mode)  \
do {							 \
    if(_mode == INPUT_PULLUP)	 \
        pinMode(_pin, INPUT);	 \
        digitalWrite(_pin, 1);	 \
    if(_mode != INPUT_PULLUP)	 \
        pinMode(_pin, _mode);	 \
}while(0)
#endif

#define OPEN LOW
#define CLOSED HIGH

typedef unsigned int uint;
typedef unsigned long ulong;

typedef struct {
    byte rows;
    byte columns;
} MatrixSize;

typedef struct {
    Pio* port;
    uint32_t pin;
} PinMap;

//Example: pin 23: {PIOA, PIO_PA14}

#define makeKeymap(x) ((char*)x)
#define LIST_MAX 10
#define MAPSIZE 10

class KeyboardMatrix : public Key {
public:
    KeyboardMatrix();
    void initializeMatrix(char *customKeymap, PinMap *row, PinMap *col, byte numRows, byte numCols);

    uint bitMap[MAPSIZE];
    Key key[LIST_MAX];
    ulong holdTimer;

    void enableClock(Pio* port);
    char getKey();
    bool getKeys();
    void begin(char *customKeymap);
    bool isPressed(char keyChar);
    void setDebounceTime(uint);
    char waitForKey();
    void setHoldTime(uint);
    int findInList(char keyChar);
    int findInList(int keyCode);

    void testPins();

private:
    ulong startTime;
    char *keymap;
    PinMap *rowPins;
    PinMap *columnPins;
    MatrixSize matrixSize;
    uint debounceTime;
    uint holdTime;

    void setRowPinsToInputPullup();
    void resetColumnPins();
    void scanKeys();
    bool updateList();
    void nextKeyState(byte n, boolean button);
    void transitionState(byte n, KeyState nextState);
};

#endif
