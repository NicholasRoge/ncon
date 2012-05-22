/******************************************************************************\
|File:  /src/headers/key_definitions.h										   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

//const char ESC_KEY=0x1B;

const char GENKEY_MOD=0x1B;
const char ARROWKEY_MOD=0x5B;  //Also includes the delete key, so...  TODO:  find a better name for this.
const char PAGEKEY_MOD=0x4F;
const char DELETEKEY_MOD=0x33;  //Since at the moment, we don't know what else 0x33 indicates, we'll just call it the delete key modifier.

const char ARROWKEY_UP=0x41;
const char ARROWKEY_DOWN=0x42;
const char ARROWKEY_RIGHT=0x43;
const char ARROWKEY_LEFT=0x44;

const char PAGEKEY_END=0x46;
const char PAGEKEY_HOME=0x48;

const char KEY_BACKSPACE=0x7F;
const char KEY_DELETE=0x7E;