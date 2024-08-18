//
// Created by Giovanni on 8/1/2024.
//

#ifndef KEY_H
#define KEY_H

#include <Arduino.h>

#define OPEN LOW
#define CLOSED HIGH

typedef unsigned int uint;
typedef enum{ IDLE, PRESSED, HOLD, RELEASED } KeyState;

const char NO_KEY = '\0';

class Key {
public:
// members~
    char keyChar;
    int keyCode;
    KeyState keyState;
    boolean stateChanged;

    // methods
    Key();
    Key(char userKeyChar);
    void key_update(char userKeyChar, KeyState userState, boolean userStatus);

private:

};

#endif
