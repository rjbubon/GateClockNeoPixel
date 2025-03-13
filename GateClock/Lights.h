/*
 * ======================================================================================================================
 *  Lights.h
 * ======================================================================================================================
 */
#define CHEVRON_COLOR    0xCF3000
#define COLOR_BLACK      0x000000
#define COLOR_WARMWHITE  0xF6E7D2
#define COLOR_WHITE      0xFFFFFF
#define LIGHT_INTENSITY  0xCC
#define COLOR_SPARK      0XFFFFFF


    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    // pixels.setPixelColor(i, pixels.Color(0, 150, 0));

byte WORMHOLE_COLOR[3] = {0xAD,0xD8,0xE6}; //blueish
int GateAddress[10];

/*
 * ======================================================================================================================
 * clearLedsLeds()
 * ======================================================================================================================
 */
void clearLeds() {
  int i;
  for (i=0; i<lightCount; i++) {
    Leds[i] = COLOR_BLACK;
  }
}

/*
 * ======================================================================================================================
 * updateLeds()
 * ======================================================================================================================
 */
void updateLeds() {
  int i;
  int p;
  for (i=0; i<lightCount; i++) {
    // make 0 be the bottom of the circle aka 30
    p=(i+30) % 60; 
    pixels.setPixelColor(p, Leds[i]);
  }
  pixels.show();
}

/*
 * ======================================================================================================================
 * setClockMarks()
 * ======================================================================================================================
 */
void setClockMarks(){
  int i;
  for (i=0; i<12; i++) {
    Leds[i*5] = COLOR_WARMWHITE;
  }
}

/*
 * ======================================================================================================================
 * setClockHour() 
 * ======================================================================================================================
 */
void setClockHour(int hour, int minute, int second){
  int hrLed, hrLedM1, hrLedM2, hrLedP1, hrLedP2;

  // Find the Led for the current hour
  if (hour >= 12) {
    hrLed = (hour - 12) * 5;
  }
  else {
    hrLed = hour * 5;
  }

  // Increment the led for every 12 minutes through the hour
  if (minute >= (12*1)) { hrLed++; }
  if (minute >= (12*2)) { hrLed++; }
  if (minute >= (12*3)) { hrLed++; }
  if (minute >= (12*4)) { hrLed++; }

  // Find Hour Led Minus 1
  if (hrLed == 0) {
    hrLedM1 = 59;
  }
  else {
    hrLedM1 = hrLed - 1;
  }

  // Find Hour Led Minus 2
  if (hrLedM1 == 0) {
    hrLedM2 = 59;
  }
  else {
    hrLedM2 = hrLedM1 - 1;
  }


  // Find Hour Led Plus 1
  hrLedP1 = (hrLed+1) % lightCount;

  // Find Hour Led Plus 2
  hrLedP2 = (hrLedP1+1) % lightCount;

  Leds[hrLedM2] = 0xFF0000;
  Leds[hrLedM1] = 0xFF0000;  
  Leds[hrLed]   = 0xFF0000;  // Red 
  Leds[hrLedP1] = 0xFF0000;
  Leds[hrLedP2] = 0xFF0000;
}

/*
 * ======================================================================================================================
 * setClockMinute()
 * ======================================================================================================================
 */
void setClockMinute(int hour, int minute, int second){
  int minLedM1, minLedP1;

  // Find Minute Led Minus 1
  if (minute == 0) {
    minLedM1 = 59;
  }
  else {
    minLedM1 = minute - 1;
  }

  // Find Hour Led Plus 1
  minLedP1 = (minute+1) % lightCount;


  Leds[minLedM1] = 0x00FF00; 
  Leds[minute]   = 0x00FF00;  // Green
  Leds[minLedP1] = 0x00FF00;
}

/*
 * ======================================================================================================================
 * setClockSecond()
 * ======================================================================================================================
 */
void setClockSecond(int hour, int minute, int second){
  int secLedM1;

  // Find Minute Led Minus 1
  if (second == 0) {
    secLedM1 = 59;
  }
  else {
    secLedM1 = second - 1;
  }

  //Leds[secLedM1] = 0x0000FF;  // With Neopixel solutions lest not do 2 leds for seconds
  Leds[second]   = 0x0000FF;  // Blue
}

/*
 * ======================================================================================================================
 * displayClock()
 * ======================================================================================================================
 */
void displayClock(int hour, int minute, int second){

  clearLeds();
  setClockMarks();
  setClockHour  (hour, minute, second);
  setClockMinute(hour, minute, second);
  //setClockMarks();
  setClockSecond(hour, minute, second);
  updateLeds();
}

/*
 * ======================================================================================================================
 * setChevron()
 * ======================================================================================================================
 */
void setChevron(int chevron, uint32_t c)
{
  //add chevron for current state  
  //chervons are orange

  int led1 = 0;
  int led2 = 0;
  int led3 = 0;
  
  if(chevron == 0) { return; }
  if(chevron == 1){led1 = 6; led2 = led1+1; led3 = led1+2;}
  if(chevron == 2){led1 = 12; led2 = led1+1; led3 = led1+2;}
  if(chevron == 3){led1 = 19; led2 = led1+1; led3 = led1+2;}
  if(chevron == 4){led1 = 39; led2 = led1+1; led3 = led1+2;}
  if(chevron == 5){led1 = 46; led2 = led1+1; led3 = led1+2;}
  if(chevron == 6){led1 = 52; led2 = led1+1; led3 = led1+2;}
  if(chevron == 7){led1 = 26; led2 = led1+1; led3 = led1+2;}
  if(chevron == 8){led1 = 32; led2 = led1+1; led3 = led1+2;}
  if(chevron == 9){led1 = 59; led2 = 0; led3 = 1;}

  Leds[led1] = c;
  Leds[led2] = c;
  Leds[led3] = c;
}

/*
 * ======================================================================================================================
 * addChevronFade() -- adds a single chevron to the array at the faded amount 
 * ======================================================================================================================
 */
void addChevronFade(int chevron, uint32_t c)
{
  int led1 = 0;
  int led2 = 0;
  int led3 = 0;
  if(chevron == 0) { return; }
  if(chevron == 1){led1 = 6; led2 = led1+1; led3 = led1+2;}
  if(chevron == 2){led1 = 12; led2 = led1+1; led3 = led1+2;}
  if(chevron == 3){led1 = 19; led2 = led1+1; led3 = led1+2;}
  if(chevron == 4){led1 = 39; led2 = led1+1; led3 = led1+2;}
  if(chevron == 5){led1 = 46; led2 = led1+1; led3 = led1+2;}
  if(chevron == 6){led1 = 52; led2 = led1+1; led3 = led1+2;}
  if(chevron == 7){led1 = 26; led2 = led1+1; led3 = led1+2;}
  if(chevron == 8){led1 = 32; led2 = led1+1; led3 = led1+2;}
  if(chevron == 9){led1 = 59; led2 = 0; led3 = 1;}

  Leds[led1] = c;
  Leds[led2] = c;
  Leds[led3] = c;
}

/*
 * ======================================================================================================================
 * LightChevron()
 * ======================================================================================================================
 */
void LightChevron (int chevron, uint32_t c)
{
  int led1 = 0;
  int led2 = 0;
  int led3 = 0;
  if(chevron == 0) { return; }
  if(chevron == 1){led1 = 6; led2 = led1+1; led3 = led1+2;}
  if(chevron == 2){led1 = 12; led2 = led1+1; led3 = led1+2;}
  if(chevron == 3){led1 = 19; led2 = led1+1; led3 = led1+2;}
  if(chevron == 4){led1 = 39; led2 = led1+1; led3 = led1+2;}
  if(chevron == 5){led1 = 46; led2 = led1+1; led3 = led1+2;}
  if(chevron == 6){led1 = 52; led2 = led1+1; led3 = led1+2;}
  if(chevron == 7){led1 = 26; led2 = led1+1; led3 = led1+2;}
  if(chevron == 8){led1 = 32; led2 = led1+1; led3 = led1+2;}
  if(chevron == 9){led1 = 59; led2 = 0; led3 = 1;}
  
  Leds[led1] = c;
  Leds[led2] = c;
  Leds[led3] = c;
}

/*
 * ======================================================================================================================
 * ChevronFadeUp() -- When a Chevron is faded up, Chevron 9 is also lit at same time 
 * ======================================================================================================================
 */
void ChevronFadeUp(int chevron)
{
  uint32_t c = CHEVRON_COLOR;
  int fadeDelay = 5;

  byte targetR = (c >> 16) & 0xFF;
  byte targetG = (c >> 8) & 0xFF;
  byte targetB = c & 0xFF;
  
  // Start from off (0, 0, 0)
  byte r = 0;
  byte g = 0;
  byte b = 0;

  // Get the number of steps based on the duration
  int steps = 50;  // Number of steps for fading
  
  // Gradually increase each color channel (R, G, B) to the target values
  for (int i = 0; i <= steps; i++) {
    // Calculate the current RGB values based on the target
    byte currentR = (byte)(r + ((float)i / steps) * (targetR - r));
    byte currentG = (byte)(g + ((float)i / steps) * (targetG - g));
    byte currentB = (byte)(b + ((float)i / steps) * (targetB - b));

    c = (currentR<<16) + (currentG<<8) + currentB;
    LightChevron(chevron, c);
    LightChevron(9, c);
    updateLeds();
    
    delay(fadeDelay);
  }
}

/*
 * ======================================================================================================================
 * ChevronFadeDown()
 * ======================================================================================================================
 */
void ChevronFadeDown(int chevron)
{
  uint32_t c = CHEVRON_COLOR;
  int fadeDelay = 5;
  
  // Extract RGB components
  byte r = (c >> 16) & 0xFF;
  byte g = (c >> 8) & 0xFF;
  byte b = c & 0xFF;
  
  // Get the number of steps based on the duration
  int steps = 50;  // Number of steps for fading
  
  // Gradually fade each color channel (R, G, B) to 0
  for (int i = 0; i <= steps; i++) {
    byte dimR = (byte)(r * (1.0 - (float)i / steps));  // Decrease red
    byte dimG = (byte)(g * (1.0 - (float)i / steps));  // Decrease green
    byte dimB = (byte)(b * (1.0 - (float)i / steps));  // Decrease blue

    c = (dimR<<16) + (dimG<<8) + dimB;
    LightChevron(chevron, c);
    updateLeds();
    delay(fadeDelay);
  }
}

/*
 * ======================================================================================================================
 * SetGateAddress()
 * ======================================================================================================================
 */
void SetGateAddress() {
  // Set the first element to 0
  GateAddress[0] = 0;
  GateAddress[9] = 9;

  
  // Create a temporary array to keep track of used numbers
  bool used[8] = {false};
  
  // Fill the remaining elements with unique random numbers from 1 to 8
  for (int i = 1; i < 9; i++) {
    int randomNum;
    do {
      randomNum = random(1, 9); // Generate a random number between 1 and 8
    } while (used[randomNum - 1]); // Repeat if the number has already been used
    
    GateAddress[i] = randomNum;
    used[randomNum - 1] = true; // Mark the number as used
  }
}

/*
 * ======================================================================================================================
 * dialGate()
 * ======================================================================================================================
 */
void dialGate()
{
  //run a stargate dialing effect

  int dialSpeed = 5;
  int chevrons = 0;
  int dotPosition = 0;
  int lastdotposition=0;
  int chevron;

  clearLeds();
  updateLeds();

  randomSeed(analogRead(0));
  
  // Call the function to fill the array
  SetGateAddress();
  sprintf (msgbuf, "GateAdr:");
  for (int i = 0; i <= 9; i++) {
    sprintf (msgbuf+strlen(msgbuf), "%d", GateAddress[i]);
  }
  Output (msgbuf);

  for (int i = 0; i <= 9; i++)
  {
    int dialAmount = random(20,59); //alter this to make it dial diffent Amounts
    int dialDirection = random(0,5);  //do ifs to determine direction

    chevron = GateAddress[i];

    if (chevron != 0) {  // dial first
      ChevronFadeUp (chevron);
    } 
    if (chevron != 9) {
      if (chevron != 0) { 
        ChevronFadeDown(9);
      }
    
      setChevron(chevron, CHEVRON_COLOR);
      // delay(500);     // Chevron Engaging, delay dialing

      while (dialAmount>0)
      {
        updateLeds();     // also turns off dot
        delay(40);

        if (dialAmount != 0)    //if we are still moving the dot
        {
          if (Leds[lastdotposition] == COLOR_WHITE) {
            Leds[lastdotposition] = COLOR_BLACK;
          }
          lastdotposition = dotPosition;
          // Don't overwrite chevron leds
          if (Leds[dotPosition] == COLOR_BLACK) {
            if (dialAmount>1) {  // Don't set the last dot before chevron
              Leds[dotPosition] = COLOR_WHITE;
            }
          } 

          if(dialDirection >2)
            dotPosition++;    //increase dot
          else
            dotPosition--;    //decrease dot

          //checks
          if(dotPosition > 59)
            dotPosition=0;
          if(dotPosition < 0)
            dotPosition = 59;
          delay(dialSpeed);

          dialAmount--;     //reduce the distance to dial
        }
      }
    }
  }
  delay(500);
}

/*
 * ======================================================================================================================
 * fadeWhiteToBlueToBlack() 
 * ======================================================================================================================
 */
void fadeWhiteToBlueToBlack(uint32_t duration) {
  uint32_t startTime = millis();
  uint32_t elapsedTime;

  while ((elapsedTime = millis() - startTime) < duration) {
    float progress = (float)elapsedTime / duration;
    
    if (progress < 0.5) {
      // Fade from white to blue
      uint8_t r = 255 - (255 * (progress * 2));
      uint8_t g = 255 - (255 * (progress * 2));
      uint8_t b = 255;
      
      for (int i = 0; i < lightCount; i++) {
        pixels.setPixelColor(i, r, g, b);
      }
    } else {
      // Fade from blue to black
      uint8_t b = 255 * (2 - (progress * 2));
      
      for (int i = 0; i < lightCount; i++) {
        pixels.setPixelColor(i, 0, 0, b);
      }
    }
    
    pixels.show();
    delay(20); // Small delay to control update rate
  }
  
  // Ensure all pixels are off at the end
  pixels.clear();
  pixels.show();
}

/*
 * ======================================================================================================================
 * fadeBlueToBlack() 
 * ======================================================================================================================
 */
void fadeBlueToBlack(uint32_t duration) {
  uint32_t startTime = millis();
  uint32_t elapsedTime;
  float progress;

  while ((elapsedTime = millis() - startTime) < duration){
    progress = (float)elapsedTime / duration;

    // Fade from blue to black
    uint8_t b = 255 * (2 - (progress * 2));

    if (b <= 10 ) {
      break;
    }
      
    for (int i = 0; i < lightCount; i++) {
      if (random(30) == 0) {  // White sparkle
        pixels.setPixelColor(i, b, b, b);
      }
      else {
        pixels.setPixelColor(i, 0, 0, b);
      }
    }
    pixels.show();
    delay(20); // Small delay to control update rate
  }
  
  // Ensure all pixels are off at the end
  pixels.clear();
  pixels.show();
}

/*
 * ======================================================================================================================
 * kawoosh() 
 * ======================================================================================================================
 */
void kawoosh()
{
  int fadeDelay = 50;

  // Flash
  for (int i=0; i<lightCount; i=i+3) {     
    pixels.setPixelColor(i, COLOR_WHITE);
  }  
  pixels.show();
  delay(50);

  for (int i=2; i<lightCount; i=i+3) {     
    pixels.setPixelColor(i, COLOR_WHITE);
  }
  pixels.show();
  delay(50);

  for (int i=1; i<lightCount; i=i+3) {     
    pixels.setPixelColor(i, COLOR_WHITE);
  }
  pixels.show();
  delay(50);

  fadeWhiteToBlueToBlack(2000);
}

/*
 * ======================================================================================================================
 * wormholeEnd() - gracefull ending of a wormhole 
 * ======================================================================================================================
 */
void wormholeEnd()
{
  fadeBlueToBlack(4000);
}

/*
 * ======================================================================================================================
 * wormholeUnstable() - ripple effect with black outs, which increase until collapse 
 * ======================================================================================================================
 */
void wormholeUnstable()
{
  unsigned long startTime = millis();
  uint8_t pixelBrightness[lightCount];

  // Initialize all pixels with random brightness
  for (int i = 0; i < lightCount; i++) {
    pixelBrightness[i] = random(2200, 256);
  }

  int r=100;
  while (millis() - startTime < 5000) {  // Run for 5 seconds
    int black = random(0,r);
    if (black == 1 || black == 2){  //double the chances
      pixels.clear();
      pixels.show();
      delay(500);
      r--;  //increase probability
    }
    else {
      for (int i = 0; i < lightCount; i++) {    
        if (random(30) == 0) {  // White
          pixels.setPixelColor(i, 255, 255, 255);
        }
        else {
          // Randomly adjust brightness
          if (random(5) == 0) { // 20 percent
            pixelBrightness[i] = random(100, 256);
          }
          // Set pixel color with current brightness
          pixels.setPixelColor(i, 0, 0, pixelBrightness[i]);
        }
      }
      pixels.show();
      delay(10);  // Small delay for visible effect
    }    
  }
}

/*
 * ======================================================================================================================
 * wormholeisopen() 
 * ======================================================================================================================
 */
void wormholeisopen() {
  unsigned long startTime = millis();
  uint8_t pixelBrightness[lightCount];
  
  // Initialize all pixels with random brightness
  for (int i = 0; i < lightCount; i++) {
    pixelBrightness[i] = random(2200, 256);
  }
  
  while (millis() - startTime < 5000) {  // Run for 5 seconds
    for (int i = 0; i < lightCount; i++) {
      
      if (random(30) == 0) {  // White
        pixels.setPixelColor(i, 255, 255, 255);
      }
      else {
        // Randomly adjust brightness
        if (random(5) == 0) { // 20 percent
          pixelBrightness[i] = random(100, 256);
        }
        // Set pixel color with current brightness
        pixels.setPixelColor(i, 0, 0, pixelBrightness[i]);
      }
    }
    
    pixels.show();
    delay(10);  // Small delay for visible effect
  }
}

/*
 * ======================================================================================================================
 * dialFailSparks() -- fails with sparks 
 * ======================================================================================================================
 */
void dialFailSparks()
{
  uint32_t c = CHEVRON_COLOR;
  int fadeDelay = 50;
  
  // Extract RGB components
  byte r = (c >> 16) & 0xFF;
  byte g = (c >> 8) & 0xFF;
  byte b = c & 0xFF;
  
  // Get the number of steps based on the duration
  int steps = 50;  // Number of steps for fading
  int s=0;
  int ls=0;
  
  // Gradually fade each color channel (R, G, B) to 0
  for (int i = 0; i <= steps; i++) {
    Leds[ls]= COLOR_BLACK;
    s = random(0,59);
    Leds[s]= COLOR_SPARK;
    ls = s;
    
    byte dimR = (byte)(r * (1.0 - (float)i / steps));  // Decrease red
    byte dimG = (byte)(g * (1.0 - (float)i / steps));  // Decrease green
    byte dimB = (byte)(b * (1.0 - (float)i / steps));  // Decrease blue

    c = (dimR<<16) + (dimG<<8) + dimB;
    for (int lc=0; lc<=9; lc++) {
      LightChevron(lc, c);
    }
    updateLeds();
    delay(fadeDelay);
  } 
}

/*
 * ======================================================================================================================
 * dialFail() 
 * ======================================================================================================================
 */
void dialFail()
{
  uint32_t c = CHEVRON_COLOR;
  int fadeDelay = 50;
  
  // Extract RGB components
  byte r = (c >> 16) & 0xFF;
  byte g = (c >> 8) & 0xFF;
  byte b = c & 0xFF;
  
  // Get the number of steps based on the duration
  int steps = 50;  // Number of steps for fading
  
  // Gradually fade each color channel (R, G, B) to 0
  for (int i = 0; i <= steps; i++) {    
    byte dimR = (byte)(r * (1.0 - (float)i / steps));  // Decrease red
    byte dimG = (byte)(g * (1.0 - (float)i / steps));  // Decrease green
    byte dimB = (byte)(b * (1.0 - (float)i / steps));  // Decrease blue

    c = (dimR<<16) + (dimG<<8) + dimB;
    for (int lc=0; lc<=9; lc++) {
      LightChevron(lc, c);
    }
    updateLeds();
    delay(fadeDelay);
  } 
}

/*
 * ======================================================================================================================
 * dialIn() - all chevrons fade in 
 * ======================================================================================================================
 */
void dialIn()
{
  int fadeDelay = 3;

  clearLeds();
  for(int j=0; j < LIGHT_INTENSITY; j=j+5)//fadeup sequence
  {
    addChevronFade(1 ,j);
    addChevronFade(2 ,j);
    addChevronFade(3 ,j);
    addChevronFade(4 ,j);
    addChevronFade(5 ,j);
    addChevronFade(6 ,j);
    addChevronFade(7 ,j);
    addChevronFade(8 ,j);
    addChevronFade(9 ,j);
    delay(fadeDelay);
    updateLeds();
  }
  delay(1000);
}

/*
 * ======================================================================================================================
 * checkChime() 
 * ======================================================================================================================
 */
void checkChime()
{
  int sequence = random(1,8);
  switch(sequence)
    {
      case 1: dialGate(); dialFailSparks(); break;
      case 2: dialGate(); kawoosh(); wormholeisopen(); wormholeEnd(); break;
      case 3: dialGate(); kawoosh(); wormholeisopen(); wormholeUnstable(); break;
      case 4: dialIn(); kawoosh(); wormholeisopen(); wormholeEnd(); break;
      case 5: dialIn(); kawoosh(); wormholeisopen(); wormholeUnstable(); break;
      case 6: dialGate(); kawoosh(); wormholeisopen(); wormholeEnd(); break;
      case 7: dialGate(); dialFail(); break;  
      default: dialGate(); kawoosh(); wormholeisopen(); wormholeEnd(); break;
  }
}
