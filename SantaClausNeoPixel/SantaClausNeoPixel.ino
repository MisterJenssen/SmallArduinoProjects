// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6
#define BUTTON_PIN 3

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 12

#define EEP_ADDR  0

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

float MaximumBrightness = 255;
float MinimumBrightness = 50;
float NumberOfSteps = MaximumBrightness - MinimumBrightness;
float BreathTime = 5000;  //msec
float StepSize = 1;

bool ButtonPressed = false;
int ProgramState = 0;


// setup() function -- runs once at startup --------------------------------
void setup() 
{
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(MaximumBrightness); // Set BRIGHTNESS (max = 255)

  // initialize the button pin as a input:
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  EIFR |= (1 << INTF1);   // Clearing interrupt caused by INPUT_PULLUP on a nano
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterrupt, RISING);  

  ProgramState = EEPROM.read(EEP_ADDR);
}
 
void loop() 
{
  if(ButtonPressed)
  {
    delay(200);
    ProgramState++;
    
    if(ProgramState > 15)
      ProgramState = 0;
    
    EEPROM.update(EEP_ADDR, ProgramState);
    ButtonPressed = false;
  }

  switch(ProgramState)
  {
    case 0:
    {
        breath(255, 255, 255);  //White
    }
    break;
    
    case 1:
    {
        breath(255, 0, 0);    //Red
    }
    break;

    case 2:
    {
        breath(0, 255, 0);    //Green
    }
    break;

    case 3:
    {
        breath(0, 0, 255);    //Blue
    }
    break;

    case 4:
    {
        breath(255, 255, 0);    //Yellow
    }
    break;

    case 5:
    {
        breath(255, 0, 255);    //Purple
    }
    break;

    case 6:
    {
        breath(0, 255, 255);    //Light Blue
    }
    break;

    case 7:
    {
      setColor(255, 255, 255); // White
    }
    break;
      
    case 8:
    {
      setColor(255, 0, 0); // Red
    }
    break;

    case 9:
    {
      setColor(0, 255, 0); // Green
    }
    break;

    case 10:
    {
      setColor(0, 0, 255); // Blue
    }
    break;

    case 11:
    {
        setColor(255, 255, 0);    //Yellow
    }
    break;

    case 12:
    {
       setColor(255, 0, 255);    //Purple
    }
    break;

    case 13:
    {
        setColor(0, 255, 255);    //Light Blue
    }
    break;
      
    case 14:
    {
      rainbow(250, false);
    }
    break;

    case 15:
    {
      rainbow(10, true);
    }
    break;
      
    default:
    {
      ProgramState = 0;
    }
  }
}

void breath(int r, int g, int b)
{
  //Fade in
  for(int i = MinimumBrightness; i < MaximumBrightness; i+=StepSize)
  {    
    strip.setBrightness(i);

    for (int ledNumber=0; ledNumber<strip.numPixels(); ledNumber++) 
      strip.setPixelColor(ledNumber, r, g, b);
    
    strip.show();
    //Wait a bit before continuing to breathe
    delay((BreathTime/2)/(NumberOfSteps/StepSize));  
  }

  delay(100);
  
  //Fade out
  for(int i = MaximumBrightness; i > MinimumBrightness; i-=StepSize)
  {      
    strip.setBrightness(i);

    for (int ledNumber=0; ledNumber<strip.numPixels(); ledNumber++) 
      strip.setPixelColor(ledNumber, r, g, b);
    
    strip.show();
    //Wait a bit before continuing to breathe
    delay((BreathTime/2)/(NumberOfSteps/StepSize));  
  }
}


void setColor(int r, int g, int b)
{
  strip.setBrightness(MaximumBrightness);

  for (int ledNumber=0; ledNumber<strip.numPixels(); ledNumber++) 
    strip.setPixelColor(ledNumber, r, g, b);

  strip.show();
  delay(1000);
}


// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait, bool theatre)
{
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) 
  {
    for(int i=0; i<strip.numPixels(); i++) 
    { 
      int pixelHue;
      // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      if(theatre)
        pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      else
        pixelHue = firstPixelHue;
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment

    if(ButtonPressed)
      return;
  }
}


void buttonInterrupt()
{
  ButtonPressed = true;
}
