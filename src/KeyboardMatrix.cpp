#include "KeyboardMatrix.h"

KeyboardMatrix::KeyboardMatrix(){
    // Created empty constructor for debugging.
    // Normally, initializeMatrix wouldn't exist but nothing
    // inside the normal constructor was getting called.
}

// KeyboardMatrix::KeyboardMatrix(char *customKeymap, PinMap *row, PinMap *col, byte numRows, byte numCols)
//     : startTime(0), rowPins(row), columnPins(col), matrixSize({numRows, numCols}) {
//     begin(customKeymap);
//     setDebounceTime(10);
//     setHoldTime(500);
//
//     setRowPinsToInputPullup();
// }

void KeyboardMatrix::initializeMatrix(char *customKeymap, PinMap *row, PinMap *col, byte numRows, byte numCols) {
    startTime = 0;
    rowPins = row;
    columnPins = col;
    matrixSize = {numRows, numCols};

    begin(customKeymap);
    setDebounceTime(10);
    setHoldTime(500);

    setRowPinsToInputPullup();
}


void KeyboardMatrix::enableClock(Pio* port) {
    if (port == PIOA) {
        PMC->PMC_PCER0 |= (1 << ID_PIOA);
        Serial.println("PIOA Clock Enabled");
    } else if (port == PIOB) {
        PMC->PMC_PCER0 |= (1 << ID_PIOB);
        Serial.println("PIOB Clock Enabled");
    } else if (port == PIOC) {
        PMC->PMC_PCER0 |= (1 << ID_PIOC);
        Serial.println("PIOC Clock Enabled");
    } else if (port == PIOD) {
        PMC->PMC_PCER0 |= (1 << ID_PIOD);
        Serial.println("PIOD Clock Enabled");
    }
}


void KeyboardMatrix::setRowPinsToInputPullup() {
    //Serial.println("Set Row To Up");
    for(byte r = 0; r < matrixSize.rows; r++) {
        rowPins[r].port->PIO_PER = rowPins[r].pin;
        rowPins[r].port->PIO_ODR = rowPins[r].pin;
        rowPins[r].port->PIO_PUER = rowPins[r].pin;
    }
}

void KeyboardMatrix::resetColumnPins() {
    //Serial.println("Reset Col");
    for(byte c = 0; c < matrixSize.columns; c++) {
        columnPins[c].port->PIO_SODR = columnPins[c].pin;
        columnPins[c].port->PIO_ODR = columnPins[c].pin;
    }
}

void KeyboardMatrix::begin(char* customKeymap) {
    Serial.println("Begin");
    keymap = customKeymap;
}

char KeyboardMatrix::getKey() {
    //Serial.println("GetKey");
    if(getKeys() && key[0].stateChanged && key[0].keyState == PRESSED) {
        Serial.println("getKey Passed");
        return key[0].keyChar;
    }
    return NO_KEY;
}

bool KeyboardMatrix::getKeys() {
    //Serial.println("getKeysss");
    bool keyActivity = false;

    if(millis() - startTime >= debounceTime) {
        scanKeys();
        keyActivity = updateList();
        startTime = millis();
    }

    return keyActivity;
}

void KeyboardMatrix::scanKeys() {
    setRowPinsToInputPullup();

    for(byte c = 0; c < matrixSize.columns; c++) {
        columnPins[c].port->PIO_OER = columnPins[c].pin;
        columnPins[c].port->PIO_CODR = columnPins[c].pin;

        for(byte r = 0; r < matrixSize.rows; r++) {
            bool keyState = !(rowPins[r].port->PIO_PDSR & rowPins[r].pin);
            bitWrite(bitMap[r], c, keyState);
        }

        columnPins[c].port->PIO_SODR = columnPins[c].pin;
    }
    resetColumnPins();
}

bool KeyboardMatrix::updateList() {
    //Serial.println("Update List");
    bool activity = false;

    for(auto & i : key) {
        if(i.keyState == IDLE) {
            i.keyChar = NO_KEY;
            i.keyCode = -1;
            i.stateChanged = false;
        }
    }

    for(byte r = 0; r < matrixSize.rows; r++) {
        for(byte c = 0; c < matrixSize.columns; c++) {
            bool button = bitRead(bitMap[r], c);
            char keyChar = keymap[r * matrixSize.columns + c];
            int keyCode = r * matrixSize.columns + c;
            int idx = findInList(keyCode);

            if(idx == -1) {
                nextKeyState(idx, button);
            }
            else if(button) {
                for (byte i = 0; i < LIST_MAX; i++) {
                    if (key[i].keyChar == NO_KEY) {
                        key[i].keyChar = keyChar;
                        key[i].keyCode = keyCode;
                        key[i].keyState = IDLE;
                        nextKeyState(i, button);
                        Serial.println("button update list");
                        break;
                    }
                }
            }
        }
    }
    for (auto & i : key) {
        if (i.stateChanged) 
        {
            activity = true;
            Serial.println("ACTIVITY");
        }
    }

    return activity;
}

void KeyboardMatrix::nextKeyState(byte n, boolean button) {
    //Serial.println("Next key state");
    key[n].stateChanged = false;

    switch(key[n].keyState) {
        case IDLE:
            if(button == CLOSED) {
                transitionState(n, PRESSED);
                holdTimer = millis();
            }
            break;
        case PRESSED:
            if((millis() - holdTimer) > holdTime) {
                transitionState(n, HOLD);
            }
            else if(button == OPEN) {
                transitionState(n, RELEASED);
            }
            break;
        case HOLD:
            if(button == OPEN) {
                transitionState(n, RELEASED);
            }
            break;
        case RELEASED:
            transitionState(n, IDLE);
            break;
    }
}

bool KeyboardMatrix::isPressed(char keyChar) {
    for(auto & i : key) {
        if(i.keyChar == keyChar) {
            return i.keyState == PRESSED && i.stateChanged;
        }
    }
    return false;
}

int KeyboardMatrix::findInList(char keyChar) {
    for (byte i = 0; i < LIST_MAX; i++) {
        if (key[i].keyChar == keyChar) {
            return i;
        }
    }
    return -1;
}

int KeyboardMatrix::findInList(int keyCode) {
    for (byte i = 0; i < LIST_MAX; i++) {
        if (key[i].keyCode == keyCode) {
            return i;
        }
    }
    return -1;
}

char KeyboardMatrix::waitForKey() {
    char waitKey = NO_KEY;
    while ((waitKey = getKey()) == NO_KEY) {}
    return waitKey;
}

void KeyboardMatrix::setDebounceTime(uint time) {
    debounceTime = time > 1 ? time : 1;
}

void KeyboardMatrix::setHoldTime(uint time) {
    holdTime = time > 10 ? time : 10;
}

void KeyboardMatrix::transitionState(byte n, KeyState nextState) {
    key[n].keyState = nextState;
    key[n].stateChanged = true;
}

void KeyboardMatrix::testPins(){
    for(int r = 0; r < matrixSize.rows; r++){
        Serial.print(rowPins[r].pin);
        Serial.print(" ");
    }
    Serial.println();
        for(int c = 0; c < matrixSize.columns; c++){
        Serial.print(columnPins[c].pin);
        Serial.print(" ");
    }
}
