const byte IMAGES[][8] = {
{
  B00000000,
  B11000011,
  B11000011,
  B00000000,
  B00000000,
  B01000010,
  B00111100,
  B00000000
}, {
  B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001
}, {
  B00000000,
  B01100000,
  B01111000,
  B01111110,
  B01111111,
  B01111110,
  B01111000,
  B01100000
}, {
  B00000000,
  B00000000,
  B00111100,
  B00111100,
  B00111100,
  B00111100,
  B00000000,
  B00000000
}, {
  B00000000,
  B00000000,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B00000000,
  B00000000
}};
const int IMAGES_LEN = sizeof(IMAGES)/8;


enum EMOTES {
  HAPPY,
  ERROR,
  PLAY,
  STOP,
  PAUSE
};

enum EYES {
  OPEN,
  CLOSED,
  XES
};

enum ConnectionStatus {
  DISCONNECTED,
  CONNECTED,
  MUTED,
  DEAFENED,
  ConnectionStatusLength
};

/* Pinouts */
const int DIN = 2;
const int CLK = 3;
const int CS = 4;

/* Animation frequency */
const long BLINK_RATE = 83657L;  // It's an ugly way to do timing, but it works
const long MOUTH_RATE = 43657L;
//const long BLINK_RATE = 13657L;  // For faster demos
//const long MOUTH_RATE = 3657L;


/* Animation Duration */
const int BLINK_LENGTH = 150;
