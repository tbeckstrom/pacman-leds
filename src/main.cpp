//LED project.ino

#include <Arduino.h>
#include <math.h>
#include <FastLED.h>
#define NUM_STRIPS 6

#define PACMAN_RADIUS 36 // radius in inches
const float PACMAN_CIRCUMFERENCE = 2 * 3.14159 * PACMAN_RADIUS;
const int PACMAN_OUTLINE_LEDS = (int) PACMAN_CIRCUMFERENCE*0.0254;
const int PACMAN_RAYS_LEDS = (int) PACMAN_RADIUS*0.0254;

#define STRIP_1_NUM_LEDS PACMAN_OUTLINE_LEDS  // for main PacMan circle
#define STRIP_2_NUM_LEDS PACMAN_OUTLINE_LEDS
#define STRIP_3_NUM_LEDS 95
#define STRIP_4_NUM_LEDS 95
#define STRIP_5_NUM_LEDS 95
#define STRIP_6_NUM_LEDS 95

// #define NUM_LEDS STRIP_1_NUM_LEDS + STRIP_2_NUM_LEDS + STRIP_3_NUM_LEDS + STRIP_4_NUM_LEDS + STRIP_5_NUM_LEDS + STRIP_6_NUM_LEDS
const int NUM_LEDS = STRIP_1_NUM_LEDS + STRIP_2_NUM_LEDS + STRIP_3_NUM_LEDS + STRIP_4_NUM_LEDS + STRIP_5_NUM_LEDS + STRIP_6_NUM_LEDS;
#define STRIP_1_PIN 5
#define STRIP_2_PIN 6
#define STRIP_3_PIN 7
#define STRIP_4_PIN 8
#define STRIP_5_PIN 9
#define STRIP_6_PIN 10

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND  90

CRGBPalette16 currentPalette( PartyColors_p );
uint8_t       colorLoop = 1;

//FOR FIRE
bool gReverseDirection = false;
CRGBPalette16 gPal;
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING  250

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 90

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t rHue = 0; // for controlling rainbow sweep

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, STRIP_1_PIN>(leds, 0, STRIP_1_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_2_PIN>(leds, 0, STRIP_2_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_3_PIN>(leds, 0, STRIP_3_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_4_PIN>(leds, 0, STRIP_4_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_5_PIN>(leds, 0, STRIP_5_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_6_PIN>(leds, 0, STRIP_6_NUM_LEDS);
  printf(NUM_LEDS);
  // x = random16();
  // y = random16();
  // z = random16();
    // FastLED.addLeds<NEOPIXEL, STRIP_2_PIN>(leds, NUM_LEDS);

  FastLED.setMaxPowerInVoltsAndMilliamps(5,4500);

  gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White); // fire color palette
  }

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// typedef void (*SimplePatternList[])();
// SimplePatternList gPatterns = { juggle_plus_mirrored ,rainbow, confetti, sine_rainbow_fwd_glitter, Fire2012, bpm, rainbowWithGlitter, Fire2012WithPalette};

void loop() {
  // random16_add_entropy( random());
  // // Call the current pattern function once, updating the 'leds' array
  // // // do some periodic updates
  EVERY_N_MILLISECONDS( 10 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
}


// uint16_t XY( uint8_t x, uint8_t y)
// {
//   uint16_t i;

//   if( kMatrixSerpentineLayout == false) {
//     i = (y * kMatrixWidth) + x;
//   }

//   if( kMatrixSerpentineLayout == true) {
//     if( y & 0x01) {
//       // Odd rows run backwards
//       uint8_t reverseX = (kMatrixWidth - 1) - x;
//       i = (y * kMatrixWidth) + reverseX;
//     } else {
//       // Even rows run forwards
//       i = (y * kMatrixWidth) + x;
//     }
//   }

//   return i;
// }

// void fillnoise8() {
//   // If we're runing at a low "speed", some 8-bit artifacts become visible
//   // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
//   // The amount of data smoothing we're doing depends on "speed".
//   uint8_t dataSmoothing = 0;
//   if( speed < 50) {
//     dataSmoothing = 200 - (speed * 4);
//   }

//   for(int i = 0; i < MAX_DIMENSION; i++) {
//     int ioffset = scale * i;
//     for(int j = 0; j < MAX_DIMENSION; j++) {
//       int joffset = scale * j;

//       uint8_t data = inoise8(x + ioffset,y + joffset,z);

//       // The range of the inoise8 function is roughly 16-238.
//       // These two operations expand those values out to roughly 0..255
//       // You can comment them out if you want the raw noise data.
//       data = qsub8(data,16);
//       data = qadd8(data,scale8(data,39));

//       if( dataSmoothing ) {
//         uint8_t olddata = noise[i][j];
//         uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
//         data = newdata;
//       }

//       noise[i][j] = data;
//     }
//   }

//   z += speed;

//   // apply slow drift to X and Y, just for visual variation.
//   x += speed / 8;
//   y -= speed / 16;
// }

// void mapNoiseToLEDsUsingPalette()
// {
//   static uint8_t ihue=0;

//   for(int i = 0; i < kMatrixWidth; i++) {
//     for(int j = 0; j < kMatrixHeight; j++) {
//       // We use the value at the (i,j) coordinate in the noise
//       // array for our brightness, and the flipped value from (j,i)
//       // for our pixel's index into the color palette.

//       uint8_t index = noise[j][i];
//       uint8_t bri =   noise[i][j];

//       // if this palette is a 'loop', add a slowly-changing base value
//       if( colorLoop) {
//         index += ihue;
//       }

//       // brighten up, as the color palette itself often contains the
//       // light/dark dynamic range desired
//       if( bri > 127 ) {
//         bri = 255;
//       } else {
//         bri = dim8_raw( bri * 2);
//       }

//       CRGB color = ColorFromPalette( currentPalette, index, bri);
//       leds[XY(i,j)] = color;
//     }
//   }

//   ihue+=1;
// }
