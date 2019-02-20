//LED project.ino

#include <Arduino.h>
// #include <serstream>
#include <math.h>
#include <FastLED.h>
// #define NUM_STRIPS 6

#define PACMAN_RADIUS 19  // radius in inches
#define PACMAN_MOUTH_STEP 6 // additional leds to step for mouth animation
#define DOT_TRACK_LENGTH 8
#define DOT_INCREMENT 1 // dot animation

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
// const int NUM_LEDS = STRIP_1_NUM_LEDS + STRIP_2_NUM_LEDS + STRIP_3_NUM_LEDS + STRIP_4_NUM_LEDS + STRIP_5_NUM_LEDS + STRIP_6_NUM_LEDS;
const int NUM_LEDS = DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS + PACMAN_RAYS_LEDS*2;
#define STRIP_1_PIN 5
#define STRIP_2_PIN 6
#define STRIP_3_PIN 7
#define STRIP_4_PIN 8
#define STRIP_5_PIN 9
#define STRIP_6_PIN 10

#define BRIGHTNESS 200
#define FRAMES_PER_SECOND 9

// CRGBPalette16 currentPalette( PartyColors_p );
uint8_t       colorLoop = 1;

//FOR FIRE
// bool gReverseDirection = false;
// CRGBPalette16 gPal;


uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t rHue = 0; // for controlling rainbow sweep
uint8_t pacHue = 0;
int chomp_frame = 0;

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
bool opening = true;
int active_dot = 0;
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, STRIP_1_PIN>(leds, 0, DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_2_PIN>(leds, 0, DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_3_PIN>(leds, DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS, PACMAN_RAYS_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_4_PIN>(leds, DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS, PACMAN_RAYS_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_5_PIN>(leds, DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS + PACMAN_RAYS_LEDS, PACMAN_RAYS_LEDS);
  FastLED.addLeds<NEOPIXEL, STRIP_6_PIN>(leds, DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS + PACMAN_RAYS_LEDS, PACMAN_RAYS_LEDS);
  // printf(NUM_LEDS);
  // x = random16();
  // y = random16();
  // z = random16();
    // FastLED.addLeds<NEOPIXEL, STRIP_2_PIN>(leds, NUM_LEDS);

  FastLED.setMaxPowerInVoltsAndMilliamps(5,2400);

// typedef void (*SimplePatternList[])();
// SimplePatternList gPatterns = { juggle_plus_mirrored ,rainbow, confetti, sine_rainbow_fwd_glitter, Fire2012, bpm, rainbowWithGlitter, Fire2012WithPalette};
}

void dot() {
  for(int i = 0; i < DOT_TRACK_LENGTH; i++) {
    if (i == active_dot || i == active_dot + 1) {
      leds[i] = CRGB::White;
    } else {
      leds[i] = CRGB::Black;
    }
    if (animation_frame == 2) {
      leds[i] = CRGB::Yellow;
    }

  }

  if (active_dot == 0) {
    active_dot = DOT_TRACK_LENGTH - 1;
  } else {
    active_dot--;
  }
}

void chomp(int chomp_frame) {

    // outline animation
    for(int i = DOT_TRACK_LENGTH; i < DOT_TRACK_LENGTH + PACMAN_MOUTH_STEP; i++) {
    leds[i] = outline_frames[chomp_frame][0];
    }

    for(int i = DOT_TRACK_LENGTH + PACMAN_MOUTH_STEP; i < DOT_TRACK_LENGTH + PACMAN_MOUTH_STEP*2; i++) {
    leds[i] = outline_frames[chomp_frame][1];
    }

    for(int i = DOT_TRACK_LENGTH + PACMAN_MOUTH_STEP*2; i < DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS - PACMAN_MOUTH_STEP*2; i++) {
    leds[i] = outline_frames[chomp_frame][2];
    }

    for(int i = DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS - PACMAN_MOUTH_STEP*2; i < DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS - PACMAN_MOUTH_STEP; i++) {
    leds[i] = outline_frames[chomp_frame][3];
    }

    for(int i =  DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS - PACMAN_MOUTH_STEP; i < DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS; i++) {
    leds[i] = outline_frames[chomp_frame][4];
    }

    // ray animation
    for(int i = DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS; i < DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS + PACMAN_RAYS_LEDS; i++) {
    leds[i] = ray_frames[chomp_frame][0];
    }

    for(int i = DOT_TRACK_LENGTH + PACMAN_OUTLINE_LEDS + PACMAN_RAYS_LEDS; i < NUM_LEDS; i++) {
    leds[i] = ray_frames[chomp_frame][1];
    }

}

void loop() {
  // random16_add_entropy( random());
  // // Call the current pattern function once, updating the 'leds' array
  // // // do some periodic updates
  // EVERY_N_MILLISECONDS( 10 ) { animation_frame++; } // slowly cycle the "base color" through the rainbow
  chomp(animation_frame);
  dot();
  FastLED.show();
  // FastLED.delay(1000/FRAMES_PER_SECOND);
  // dot();
  // FastLED.show();

  if (animation_frame == 0) {
    opening = true;
  } else if (animation_frame == 2) {
    opening = false;
  }

  if (opening == true) {
    animation_frame++;
  } else {
    animation_frame--;
  }

  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
}
