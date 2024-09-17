#include "KeyboardMatrix.h"

KeyboardMatrix::KeyboardMatrix(){

}

// KeyboardMatrix::KeyboardMatrix(char *customKeymap, PinMap *row, PinMap *col, byte numRows, byte numCols)
//     : startTime(0), rowPins(row), columnPins(col), matrixSize({numRows, numCols}) {
//     begin(customKeymap);
//     setDebounceTime(10);
//     setHoldTime(500);

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
        columnPins[c].port->PIO_SODR = columnPins[c].pin; //Set Output Data Registry (Set to output high/default)
        columnPins[c].port->PIO_ODR = columnPins[c].pin; //Output Disable Register (Set to input)
    }
}

void KeyboardMatrix::begin(char* customKeymap) {
    Serial.println("Begin");
    keymap = customKeymap;
}

char KeyboardMatrix::getKey() {
    //Serial.println("GetKey");
    if(getKeys()) {
        //Serial.println("getKeysPassed");
        if(key[0].stateChanged && key[0].keyState == PRESSED) {
            //Serial.println("statechanged and pressed");
            return key[0].keyChar;
        }
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

// void KeyboardMatrix::scanKeys() {
//     //Serial.println("ScanKeys");
//     setRowPinsToInputPullup();

//     for(byte c = 0; c < matrixSize.columns; c++) {
//         columnPins[c].port->PIO_OER = columnPins[c].pin;
//         columnPins[c].port->PIO_CODR = columnPins[c].pin;

//         for(byte r = 0; r < matrixSize.rows; r++) {
//             bitWrite(bitMap[r], c, rowPins[r].port->PIO_PDSR & 1 << r);
//         }
//     }
//     resetColumnPins();
// }

void KeyboardMatrix::scanKeys() {
    setRowPinsToInputPullup(); 

    for(byte c = 0; c < matrixSize.columns; c++) {
        columnPins[c].port->PIO_OER = columnPins[c].pin; //Output Enable Registry (Set to output)
        columnPins[c].port->PIO_CODR = columnPins[c].pin; //Clear Output Data Registry (Set to low)

        for(byte r = 0; r < matrixSize.rows; r++) {
            bool keyState = !(rowPins[r].port->PIO_PDSR & rowPins[r].pin);
            bitWrite(bitMap[r], c, keyState); //Writes keystate to the bitmap
        }

        columnPins[c].port->PIO_SODR = columnPins[c].pin; //Set Output Data Registry (Set to output high/default)
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

            // Serial.print(keyCode);
            // Serial.print(" ");
            // Serial.print(keyChar);
            // Serial.print(" ");
            // Serial.println(button);


            if(idx > -1) {
                nextKeyState(idx, button);
                //Serial.println("Next key state");
            }
            if(idx == -1 && button) {
                //Serial.println("updateList button");
                for (byte i = 0; i < LIST_MAX; i++) {
                    if (key[i].keyChar == NO_KEY) {
                        key[i].keyChar = keyChar;
                        key[i].keyCode = keyCode;
                        key[i].keyState = IDLE;
                        nextKeyState(i, button);
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
            //Serial.println("ACTIVITY");
        }
    }

    return activity;
}

void KeyboardMatrix::nextKeyState(byte n, boolean button) {
    // Serial.print("Next key state ");
    // Serial.print(n);
    // Serial.print(" ");
    // Serial.println(button);

    key[n].stateChanged = false;

    switch(key[n].keyState) {
        case IDLE:
            //Serial.println("IDLE");
            if(button == CLOSED) {
                transitionState(n, PRESSED);
                holdTimer = millis();
            }
            break;
        case PRESSED:
            //Serial.println("PRESSED");
            if((millis() - holdTimer) > holdTime) {
                transitionState(n, HOLD);
            }
            else if(button == OPEN) {
                transitionState(n, RELEASED);
            }
            break;
        case HOLD:
            //Serial.println("HOLD");
            if(button == OPEN) {
                transitionState(n, RELEASED);
            }
            break;
        case RELEASED:
            //Serial.println("RELEASED");
            transitionState(n, IDLE);
            break;
        default:
            //Serial.println("No Key State");
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

void KeyboardMatrix::test(PinMap row, PinMap col){
    col.port->PIO_OER = col.pin;
    col.port->PIO_CODR = col.pin;

    row.port->PIO_PUER = row.pin;

    if (!(row.port->PIO_PDSR & row.pin)) {
        Serial.println("test: READ OUT TRUE");
    }

    col.port->PIO_SODR = col.pin;
    col.port->PIO_ODR = col.pin;
}