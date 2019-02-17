//LED project.ino

#include <Arduino.h>
// #include <serstream>
#include <math.h>
#include <FastLED.h>
#define NUM_STRIPS 6

#define PACMAN_RADIUS 36 // radius in inches
#define PACMAN_MOUTH_STEP 5 // additional leds to step for mouth animation

const float PACMAN_CIRCUMFERENCE = 2 * 3.14159 * PACMAN_RADIUS;
const int PACMAN_OUTLINE_LEDS = (int) PACMAN_CIRCUMFERENCE*0.0254*30;
const int PACMAN_RAYS_LEDS = (int) PACMAN_RADIUS*0.0254*30;

// for main PacMan circle
#define STRIP_1_NUM_LEDS PACMAN_OUTLINE_LEDS
#define STRIP_2_NUM_LEDS PACMAN_OUTLINE_LEDS

// for pacman mouth rays
#define STRIP_3_NUM_LEDS PACMAN_RAYS_LEDS
#define STRIP_4_NUM_LEDS PACMAN_RAYS_LEDS
#define STRIP_5_NUM_LEDS PACMAN_RAYS_LEDS
#define STRIP_6_NUM_LEDS PACMAN_RAYS_LEDS

// #define NUM_LEDS STRIP_1_NUM_LEDS + STRIP_2_NUM_LEDS + STRIP_3_NUM_LEDS + STRIP_4_NUM_LEDS + STRIP_5_NUM_LEDS + STRIP_6_NUM_LEDS
const int NUM_LEDS = STRIP_1_NUM_LEDS + STRIP_2_NUM_LEDS + STRIP_3_NUM_LEDS + STRIP_4_NUM_LEDS + STRIP_5_NUM_LEDS + STRIP_6_NUM_LEDS;
#define STRIP_1_PIN 5
#define STRIP_2_PIN 6
#define STRIP_3_PIN 7
#define STRIP_4_PIN 8
#define STRIP_5_PIN 9
#define STRIP_6_PIN 10

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND  8

CRGBPalette16 currentPalette( PartyColors_p );
uint8_t       colorLoop = 1;

//FOR FIRE
bool gReverseDirection = false;
CRGBPalette16 gPal;


uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t rHue = 0; // for controlling rainbow sweep
uint8_t chomp_frame = 0;

CRGB outline_frames[3][5] = {
  { CRGB::Black, CRGB::Black, CRGB::Yellow, CRGB::Black, CRGB::Black },
  { CRGB::Black, CRGB::Yellow, CRGB::Yellow, CRGB::Yellow, CRGB::Black },
  { CRGB::Yellow, CRGB::Yellow, CRGB::Yellow, CRGB::Yellow, CRGB::Yellow }
};
// CRGB outline_frames[0] = { CRGB::Black, CRGB::Black, CRGB::Yellow, CRGB::Black, CRGB::Black };
// CRGB outline_frames[1] = { CRGB::Black, CRGB::Yellow, CRGB::Yellow, CRGB::Yellow, CRGB::Black };
// CRGB outline_frames[2] = { CRGB::Yellow, CRGB::Yellow, CRGB::Yellow, CRGB::Yellow, CRGB::Yellow };
// std::vector<int> array = {1,3,34,5,6};

CRGB ray_frames[3][2] = {
  { CRGB::Yellow, CRGB::Black },
  { CRGB::Black, CRGB::Yellow },
  { CRGB::Black, CRGB::Black }
};
// CRBG ray_frames[0] = { CRGB::Yellow, CRGB::Black };
// CRBG ray_frames[1] = { CRGB::Black, CRGB::Yellow };
// CRBG ray_frames[2] = { CRGB::Black, CRGB::Black };

int animation_frame = 0;
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, STRIP_1_PIN>(leds, 0, STRIP_1_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_2_PIN>(leds, 0, STRIP_2_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_3_PIN>(leds, PACMAN_OUTLINE_LEDS, STRIP_3_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_4_PIN>(leds, PACMAN_OUTLINE_LEDS, STRIP_4_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_5_PIN>(leds, PACMAN_OUTLINE_LEDS + PACMAN_RAYS_LEDS, STRIP_5_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_6_PIN>(leds, PACMAN_OUTLINE_LEDS + PACMAN_RAYS_LEDS, STRIP_6_NUM_LEDS);
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

void chomp(int chomp_frame) {

    // outline animation
    for(int i = 0; i < PACMAN_MOUTH_STEP; i++) {
    leds[i] = outline_frames[chomp_frame][0];
    }

    for(int i = PACMAN_MOUTH_STEP; i < PACMAN_MOUTH_STEP*2; i++) {
    leds[i] = outline_frames[chomp_frame][1];
    }

    for(int i = PACMAN_MOUTH_STEP*2; i < PACMAN_OUTLINE_LEDS - PACMAN_MOUTH_STEP*2; i++) {
    leds[i] = outline_frames[chomp_frame][2];
    }

    for(int i = PACMAN_OUTLINE_LEDS - PACMAN_MOUTH_STEP*2; i < PACMAN_OUTLINE_LEDS - PACMAN_MOUTH_STEP; i++) {
    leds[i] = outline_frames[chomp_frame][3];
    }

    for(int i = PACMAN_OUTLINE_LEDS - PACMAN_MOUTH_STEP; i < PACMAN_OUTLINE_LEDS; i++) {
    leds[i] = outline_frames[chomp_frame][4];
    }

    // ray animation
    for(int i = PACMAN_OUTLINE_LEDS; i < PACMAN_OUTLINE_LEDS + PACMAN_RAYS_LEDS; i++) {
    leds[i] = ray_frames[chomp_frame][0];
    }

    for(int i = PACMAN_OUTLINE_LEDS + PACMAN_RAYS_LEDS; i < NUM_LEDS; i++) {
    leds[i] = ray_frames[chomp_frame][1];
    }

}

void loop() {
  // random16_add_entropy( random());
  // // Call the current pattern function once, updating the 'leds' array
  // // // do some periodic updates
  // EVERY_N_MILLISECONDS( 10 ) { animation_frame++; } // slowly cycle the "base color" through the rainbow
  chomp(animation_frame);
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
}
