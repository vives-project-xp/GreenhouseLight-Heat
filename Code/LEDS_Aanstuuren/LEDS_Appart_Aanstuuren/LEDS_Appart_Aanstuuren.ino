#include <FastLED.h>

// Number of LEDs in your strip
#define NUM_LEDS 144

// Pin for controlling the LEDs
#define DATA_PIN 2

// Define the array of LEDs
CRGB leds[NUM_LEDS];

// Time scaling factors for each component (optional, can be removed if not used)
#define TIME_FACTOR_HUE 60
#define TIME_FACTOR_SAT 100
#define TIME_FACTOR_VAL 100

void setup() {
    Serial.begin(115200);
    
    // Initialize LEDs with RGBW configuration
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setRgbw(RgbwDefault());
    FastLED.setBrightness(255);  // Set global brightness to 50%

    // Set all LEDs to purple (mix of red and blue)
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(255, 255, 0); // Rood , Blauw , Groen
    }
    FastLED.show();  // Update the LEDs
}

void loop() {
    // No changes needed in loop for static purple light
}
