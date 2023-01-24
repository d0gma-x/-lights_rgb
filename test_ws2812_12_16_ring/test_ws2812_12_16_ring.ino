#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN_12 6
#define PIXEL_COUNT_12 12

#define PIXEL_PIN_16 7
#define PIXEL_COUNT_16 16

Adafruit_NeoPixel strip_12(PIXEL_COUNT_12, PIXEL_PIN_12, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_16(PIXEL_COUNT_16, PIXEL_PIN_16, NEO_GRB + NEO_KHZ800);

const int sampleWindow = 15; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
int mode = 0;

void setup() {
  Serial.begin(9600);
  strip_12.begin();
  strip_12.setBrightness(255); //0 - 255
  strip_12.show(); // Initialize all pixels to 'off'

  strip_16.begin();
  strip_16.setBrightness(255); //0 - 255
  strip_16.show(); // Initialize all pixels to 'off'
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip_12.numPixels(); i++) { // For each pixel in strip...
    strip_12.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip_12.show();                          //  Update strip to match
  }

  for (int i = 0; i < strip_16.numPixels(); i++) { // For each pixel in strip...
    strip_16.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip_16.show();                          //  Update strip to match
  }
}

void loop() {
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(0);
    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample;
      }
      else if (sample < signalMin)
      {
        signalMin = sample;
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

  //  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  //  mode = map(volts, 0, 3.3, 0, 9);
  //  Serial.println(volts);

  float db = map(peakToPeak, 20, 900, 49.5, 90);
  mode = map(db, 49.5, 90, 0, 9);
  Serial.println(db);

  //  mode = map(peakToPeak, 0, 650, 0, 9);
  //  Serial.println(peakToPeak);
  //    Serial.println(sample);

  if (++mode > 9) mode = 0;
  switch (mode) {
    case 0:
      colorWipe(strip_12.Color(0, 0, 0), 0); //off
      colorWipe(strip_16.Color(0, 0, 0), 0); //off
      break;
    case 1:
      colorWipe(strip_12.Color(0, 255, 255), 0); //cyan
      colorWipe(strip_16.Color(0, 255, 255), 0); //cyan
      break;
    case 2:
      colorWipe(strip_12.Color(0, 255, 0), 0); //green
      colorWipe(strip_16.Color(0, 255, 0), 0); //green
      //      Color(0, 255, 0);
      break;
    case 3:
      colorWipe(strip_12.Color(255, 255, 0), 0); //yellow
      colorWipe(strip_16.Color(255, 255, 0), 0); //yellow
      
      //      Color(255, 255, 0);
      break;
    case 4:
      colorWipe(strip_12.Color(255, 127, 0), 0); //orange
      colorWipe(strip_16.Color(255, 127, 0), 0); //orange
      //      Color(255, 127, 0);
      break;
    case 5:
      colorWipe(strip_12.Color(255, 0, 0), 0); //red
      colorWipe(strip_16.Color(255, 0, 0), 0); //red
      //      Color(255, 0, 0);
      break;
    case 6:
      colorWipe(strip_12.Color(255, 0, 255), 0); //magenta
      colorWipe(strip_16.Color(255, 0, 255), 0); //magenta
      //      Color(255, 0, 255);
      break;
    case 7:
      colorWipe(strip_12.Color(0, 0, 255), 0); //blue
      colorWipe(strip_16.Color(0, 0, 255), 0); //blue
      //      Color(0, 0, 255);
      break;
    case 8:
      colorWipe(strip_12.Color(255, 0, 127), 0); //red + magenta
      colorWipe(strip_16.Color(255, 0, 127), 0); //red + magenta
      //      Color(255, 0, 127);
      break;
    case 9:
      colorWipe(strip_12.Color(127, 0, 255), 0); //blue + magenta
      colorWipe(strip_16.Color(127, 0, 255), 0); //blue + magenta
      //      Color(127, 0, 255);
      break;

      //      colorWipe(strip.Color(0, 255, 167), 0); //green + cyan
      //      colorWipe(strip.Color(127, 255, 0), 0); //green + yellow
      //      colorWipe(strip.Color(255, 0, 127), 0); //red + magenta
      //      colorWipe(strip.Color(0, 127, 255), 0); //blue + cyan
  }
}
