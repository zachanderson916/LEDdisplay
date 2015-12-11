// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif  

#define BUTTON_PIN_ONE   4    // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.
#define BUTTON_PIN_TWO 5
#define BUTTON_PIN_THREE 6
#define BUTTON_PIN_FOUR 7
#define BUTTON_PIN_FIVE 8

#define PIXEL_PIN    9    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 484

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldStateOne = HIGH;
bool oldStateTwo= HIGH;
bool oldStateThree = HIGH;
bool oldStateFour = HIGH;
bool oldStateFive = HIGH;
bool oldStateBlue = LOW;
int showType = 0;
char val;
long randNumberR;
long randNumberG;
long randNumberB;

void setup() {
  pinMode(BUTTON_PIN_ONE, INPUT_PULLUP);
  pinMode(BUTTON_PIN_TWO, INPUT_PULLUP);
  pinMode(BUTTON_PIN_THREE, INPUT_PULLUP);
  pinMode(BUTTON_PIN_FOUR, INPUT_PULLUP);
  pinMode(BUTTON_PIN_FIVE, INPUT_PULLUP);
  pinMode(PIXEL_PIN, OUTPUT);
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(0));
}

void loop() {
  // Get current button state.
  bool newStateOne = digitalRead(BUTTON_PIN_ONE);
  bool newStateTwo = digitalRead(BUTTON_PIN_TWO);
  bool newStateThree = digitalRead(BUTTON_PIN_THREE);
  bool newStateFour = digitalRead(BUTTON_PIN_FOUR);
  bool newStateFive = digitalRead(BUTTON_PIN_FIVE);
  bool newStateBlue;
  
  
  
  
  if( Serial.available() )       // if data is available to read
  {
    val = Serial.read();         // read it and store it in 'val'
    if( val == 'C' )               // if 'H' was received
    {
       Serial.println("OFF");
       newStateBlue = HIGH;
    } else { 
      Serial.println("NO INPUT");
      newStateBlue = LOW;
    }
  }
  

  if(newStateOne == LOW && oldStateOne == HIGH) {
    delay(20);
    newStateOne = digitalRead(BUTTON_PIN_ONE);
    startShow(2);
  }
  else if(newStateTwo == LOW && oldStateTwo == HIGH) {
    delay(20);
    newStateTwo = digitalRead(BUTTON_PIN_TWO);
    startShow(8);
  } 
  else if(newStateThree == LOW && oldStateThree == HIGH) {
    delay(20);
    newStateThree = digitalRead(BUTTON_PIN_THREE);
    startShow(3);
  }
  else if(newStateFour == LOW && oldStateFour == HIGH) {
    delay(20);
    newStateFour = digitalRead(BUTTON_PIN_FOUR);
    startShow(9);
  }
  else if(newStateFive == LOW && oldStateFive == HIGH) {
    delay(20);
    newStateFive = digitalRead(BUTTON_PIN_FIVE);
    startShow(1);
  }
  else if(newStateBlue == HIGH && oldStateBlue == LOW) {
    delay(20);
    if(newStateBlue == HIGH) 
    startShow(0);
  }
/*  
  // Check if state changed from high to low (button press).
  if ((newStateOne == LOW && oldStateOne == HIGH)||(newStateTwo == LOW && oldStateTwo == HIGH)||(newStateThree == LOW && oldStateThree == HIGH)||(newStateFour == LOW && oldStateFour == HIGH)||(newStateFive == LOW && oldStateFive == HIGH)||(newStateBlue == LOW && oldStateBlue == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newStateOne = digitalRead(BUTTON_PIN_ONE);
    newStateTwo = digitalRead(BUTTON_PIN_TWO);
    newStateThree = digitalRead(BUTTON_PIN_THREE);
    newStateFour = digitalRead(BUTTON_PIN_FOUR);
    newStateFive = digitalRead(BUTTON_PIN_FIVE);
    if (newStateOne == LOW || newStateTwo == LOW || newStateThree == LOW || newStateFour == LOW || newStateFive == LOW || newStateBlue == LOW) {
      showType++;
      if (showType > 9)
        showType=0;
      startShow(showType);
    }
  }
*/
  // Set the last button state to the old state.
  oldStateOne = newStateOne;
  oldStateTwo = newStateTwo;
  oldStateThree = newStateThree;
  oldStateFour = newStateFour;
  oldStateFive = newStateFive;
  oldStateBlue = newStateBlue;
}

void startShow(int i) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), 0);    // Black/off
            break;
    case 1: colorWipe(strip.Color(50, 0, 0), 0);  // Red
            break;
    case 2: colorWipe(strip.Color(0, 50, 0), 0);  // Green
            break;
    case 3: colorWipe(strip.Color(0, 0, 50), 0);  // Blue
            break;
    case 4: theaterChase(strip.Color(50,50,50), 50); // White
            break;
    case 5: theaterChase(strip.Color(50, 0, 0), 50); // Red
            break;
    case 6: theaterChase(strip.Color(0, 0, 50), 50); // Blue
            break;
    case 7: rainbow(20);
            break;
    case 8: rainbowCycleVaried(20);
            break;
    case 9: theaterChaseVaried(20);
            break;
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    //strip.show();
    //delay(wait);
  }
  strip.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*2; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 50 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycleVaried(uint8_t wait) {//every other row fades through the rainbow. One is at a clock rate that is four times the other
  uint16_t i, j, k;

  bool newStateOne = digitalRead(BUTTON_PIN_ONE);
  bool newStateTwo = digitalRead(BUTTON_PIN_TWO);
  bool newStateThree = digitalRead(BUTTON_PIN_THREE);
  bool newStateFour = digitalRead(BUTTON_PIN_FOUR);
  bool newStateFive = digitalRead(BUTTON_PIN_FIVE);
  bool newStateBlue = digitalRead(BUTTON_PIN_FIVE);

  //for(j=0; j<256*2; j++) { // 5 cycles of all colors on wheel
  for(j=0; ; j++) {
    newStateOne = digitalRead(BUTTON_PIN_ONE);
    newStateTwo = digitalRead(BUTTON_PIN_TWO);
    newStateThree = digitalRead(BUTTON_PIN_THREE);
    newStateFour = digitalRead(BUTTON_PIN_FOUR);
    newStateFive = digitalRead(BUTTON_PIN_FIVE);
    for(i=0; i< strip.numPixels(); i = i + 44) {
      for(k=0; k<22; k++)
      strip.setPixelColor(i+k, Wheel(((i * 50 / strip.numPixels()) + j) & 255));
      for(k=22; k<44; k++)
      strip.setPixelColor(i+k, Wheel(((i * 50 / strip.numPixels()) - j*8) & 255));
      //strip.setPixelColor(i+22, Wheel(((i * 50 / strip.numPixels()) - j) & 255));
    }
    if(newStateOne == LOW && oldStateOne == HIGH) {
      delay(20);
      newStateOne = digitalRead(BUTTON_PIN_ONE);
      startShow(2);
      break;
    }
    else if(newStateTwo == LOW && oldStateTwo == HIGH) {
      delay(20);
      newStateTwo = digitalRead(BUTTON_PIN_TWO);
      startShow(8);
      break;
    } 
    else if(newStateThree == LOW && oldStateThree == HIGH) {
      delay(20);
      newStateThree = digitalRead(BUTTON_PIN_THREE);
      startShow(3);
      break;
    }
    else if(newStateFour == LOW && oldStateFour == HIGH) {
      delay(20);
      newStateFour = digitalRead(BUTTON_PIN_FOUR);
      startShow(9);
      break;
    }
    else if(newStateFive == LOW && oldStateFive == HIGH) {
      delay(20);
      newStateFive = digitalRead(BUTTON_PIN_FIVE);
      startShow(1);
      break;
    }
    
    oldStateOne = newStateOne;
    oldStateTwo = newStateTwo;
    oldStateThree = newStateThree;
    oldStateFour = newStateFour;
    oldStateFive = newStateFive;
    oldStateBlue = newStateBlue;
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 128; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void theaterChaseVaried(uint8_t wait) { // every third pixel is a different color and every other one changes at a varied clock rate.
  int vary = wait;
  int decrementer = vary/5;
  bool breaker = false;
  bool newStateOne = digitalRead(BUTTON_PIN_ONE);
  bool newStateTwo = digitalRead(BUTTON_PIN_TWO);
  bool newStateThree = digitalRead(BUTTON_PIN_THREE);
  bool newStateFour = digitalRead(BUTTON_PIN_FOUR);
  bool newStateFive = digitalRead(BUTTON_PIN_FIVE);
  bool newStateBlue = digitalRead(BUTTON_PIN_FIVE);
  for (int j=0; ; j++) {     // cycle all 256 colors in the wheel
    newStateOne = digitalRead(BUTTON_PIN_ONE);
    newStateTwo = digitalRead(BUTTON_PIN_TWO);
    newStateThree = digitalRead(BUTTON_PIN_THREE);
    newStateFour = digitalRead(BUTTON_PIN_FOUR);
    newStateFive = digitalRead(BUTTON_PIN_FIVE);
    for (int q=0; q < 3; q++) {
      delay(wait-vary%7);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(vary%7);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i*j) % 255));        //turn every third pixel off
      }
      if(vary>decrementer) vary = vary - decrementer;
      else vary = wait;
    }
    if(newStateOne == LOW && oldStateOne == HIGH) {
      delay(20);
      newStateOne = digitalRead(BUTTON_PIN_ONE);
      startShow(2);
      break;
    }
    else if(newStateTwo == LOW && oldStateTwo == HIGH) {
      delay(20);
      newStateTwo = digitalRead(BUTTON_PIN_TWO);
      startShow(8);
      break;
    } 
    else if(newStateThree == LOW && oldStateThree == HIGH) {
      delay(20);
      newStateThree = digitalRead(BUTTON_PIN_THREE);
      startShow(3);
      break;
    }
    else if(newStateFour == LOW && oldStateFour == HIGH) {
      delay(20);
      newStateFour = digitalRead(BUTTON_PIN_FOUR);
      startShow(9);
      break;
    }
    else if(newStateFive == LOW && oldStateFive == HIGH) {
      delay(20);
      newStateFive = digitalRead(BUTTON_PIN_FIVE);
      startShow(1);
      break;
    }
    
    oldStateOne = newStateOne;
    oldStateTwo = newStateTwo;
    oldStateThree = newStateThree;
    oldStateFour = newStateFour;
    oldStateFive = newStateFive;
    oldStateBlue = newStateBlue;
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color((255 - WheelPos * 3)/5, 0, (WheelPos * 3)/5);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, (WheelPos * 3)/5, (255 - WheelPos * 3)/5);
  }
  WheelPos -= 170;
  return strip.Color((WheelPos * 3)/5, (255 - WheelPos * 3)/5, 0);
}
