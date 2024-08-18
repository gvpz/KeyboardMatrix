#include "Key.h"

Key::Key() {
    keyChar = NO_KEY;
    keyState = IDLE;
    stateChanged = false;
}

Key::Key(char userKeyChar) {
    keyChar = userKeyChar;
    keyState = IDLE;
    stateChanged = false;
}