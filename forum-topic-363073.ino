
// we want to randomly select from these colours

const int COLOURS[][3] = {
{100, 200, 255}, //Blue
{255, 250, 170}, //Yellow
{255, 170, 255}, //Pink
{255, 255, 255} //White
};

const int NCOLOURS = sizeof(COLOURS) / sizeof(*COLOURS);

// this structure holds the state of the given pixel

struct PixelState {
  // is the pixel currently on
  boolean on;
  // time at which the pixel was turned on or off
  unsigned long timeMarkMs;
  // amount of time for which the pixel should remain off
  unsigned long offTimeMs;
  // selected colout if the pixel is on
  int selectedColour;
  // most recent max brightness. We keep this to stop us
  // sending colour changes to the pixels if it is unnecessary
  // this is important, because neopixels use a timing-based protocol
  int mostRecentBrightness;
};

// we have 4 pixels

const int NPIXELS = 4;
struct PixelState pixel[NPIXELS];

// I want the pixels to sparkle, so I will use a triangle wave, 
// which is to say that they will come on at full brightness and them fade

const unsigned long FADE_TIME_MS = 3000; // three seconds

// the pixels should come on at a random time, To simulate this, 
// the pixel will be off for a random amount of time. 

const int MIN_OFF_TIME_MS = 100;
const int MAX_OFF_TIME_MS = 250;

void setup() {
  for(int i = 0; i<NPIXELS;i++) {
    pixel[i].on = false;
    pixel[i].timeMarkMs = millis();
    pixel[i].offTimeMs = 0;
  }
}

// this is a dummy function. I put it here just to check that the code compiles.
// Replace calls to this function with your
// strip.setPixelColor function
void setPixelColor(int strip, int r, int g, int b) {
}

void loop() {
  for(int i = 0; i< NPIXELS; i++) {
    if(pixel[i].on) {
      // pixel is on
      
      if(millis() - pixel[i].timeMarkMs >= FADE_TIME_MS) {
        // time to turn the pixel off
        setPixelColor(i,0,0,0);
        pixel[i].timeMarkMs = millis();
        pixel[i].offTimeMs = random(MIN_OFF_TIME_MS, MAX_OFF_TIME_MS);
        pixel[i].mostRecentBrightness = 0;
        pixel[i].on = false;
      }
      else {
        // calculate the new brightness as flaoting point
        // this is the bit that you change if you want the fade pattern to change
        float brightness = 1 - (millis() - pixel[i].timeMarkMs)/((float)FADE_TIME_MS);
        
        // ok. do we actually need to change the pixel colour?
        int mostRecentBrightness = 255 * brightness;
        if(mostRecentBrightness != pixel[i].mostRecentBrightness) {
          // yes we do
          pixel[i].mostRecentBrightness = mostRecentBrightness;
          setPixelColor(i,
             (int)(COLOURS[pixel[i].selectedColour][0] * brightness),
             (int)(COLOURS[pixel[i].selectedColour][1] * brightness),
             (int)(COLOURS[pixel[i].selectedColour][2] * brightness)
          );
        }
      }
    }
    else {
      // pixel is off. do we need to turn it on?
      
      if(millis()-pixel[i].timeMarkMs > pixel[i].offTimeMs) {
        pixel[i].on = true;
        pixel[i].mostRecentBrightness = 0; // this will force an update next loop
        pixel[i].selectedColour = random(NCOLOURS);
      }
    }
  }
}
