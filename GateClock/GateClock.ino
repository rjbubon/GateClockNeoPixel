/*
 *======================================================================================================================
 * GateClock - NeoPixel Based
 *   Board Type : Adafruit Feather M0 (WiFi Board) https://www.adafruit.com/product/3010
 *   Addon Board: Adalogger FeatherWing - RTC + SD https://www.adafruit.com/product/2922
 *   Description: Run a analog clock on a ring of 60 leds. On the quarter hour run Stargate effects.
 *   Author: Robert Bubon
 *   Date:   2025-03-09 RJB Based on code running backyard GE Color Effects Xmas light Gate Clock
 *           2025-03-XX RJB
 *   
 *======================================================================================================================
 */

/* 
 *=======================================================================================================================
 * Includes
 *=======================================================================================================================
 */
#include <WiFi101.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <SD.h>
#include <ctime>                // Provides the tm structure
#include <RTClib.h>             // https://github.com/adafruit/RTClib
#include <Adafruit_NeoPixel.h>

/*
 * ======================================================================================================================
 * Pin Definitions
 * 
 * Board Label   Arduino  Info & Usage
 * ======================================================================================================================
 * BAT           VBAT Power
 * En            Control - Connect to ground to disable the 3.3v regulator
 * USB           VBUS Power - Tied to Neopixel LEDs
 * 13            D13      LED                            
 * 12            D12      Serial Console Enable          
 * 11            D11      Daylight Savings Time Enable
 * 10            D10      Used by SD Card as CS
 * 9             D9/A7    Voltage Battery Pin
 * 6             D6       Button - Increase LED Brightness
 * 5             D5       Button - Lower LED Brightness     
 * SCL           D3       i2c Clock
 * SDA           D2       i2c Data 
 * RST
 
 * 3V            3v3 Power
 * ARef
 * GND           - Tied to Neopixel LEDs
 * A0            A0       
 * A1            A1       
 * A2            A2       Neopixel LEDs 
 * A3            A3       
 * A4            A4       
 * A5            A5       
 * SCK           SCK      SPI0 Clock               
 * MOS           MOSI     Used by SD Card
 * MIS           MISO     Used by SDCard
 * RX0           D0
 * TX1           D1
 * 
 * Not exposed on headers
 * D8 = WiFI
 * D7 = WiFI
 * D4 = WiFI
 * D2 = WiFI
 * ======================================================================================================================
 */

/*
 * ======================================================================================================================
 *  Globals
 * ======================================================================================================================Y
 * 
 */
#define MAX_MSGBUF_SIZE   1024
char msgbuf[MAX_MSGBUF_SIZE];   // Used to hold messages
char *msgp;                     // Pointer to message text
char Buffer32Bytes[32];         // General storage

#define SCE_PIN           12  // Serial Console Enable Pin
#define DST_PIN           11  // Ground Pin for Daylight Saving tine -7
#define LED_PIN           LED_BUILTIN

#define TM_VALID_YEAR_START     2025
#define TM_VALID_YEAR_END       2033

#define NEOPIX_PIN A2    // Arduino pin to Ge Color Effects Lights
#define lightCount 60    //Total # of lights on string (usually 50, 48, or 36)
#define WH_MIN 2         //wormhole min and max run times (Seconds)
#define WH_MAX 5

// Rotary Encoder Pins
#define ButtonUpPin 6
#define ButtonDownPin 5
unsigned long lastUp=0;
unsigned long lastDown=0;

volatile int ledBrightness = 5; // Start in the middle of the range
volatile bool encoderChanged = false;

Adafruit_NeoPixel pixels(lightCount, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);

static uint16_t c;
static uint32_t Leds [lightCount];
static unsigned long timerSec = millis();

#include "SF.h"
#include "Output.h"
#include "CF.h"                   // Configuration File Variables
#include "Lights.h"
#include "TM.h"
#include "WiFi.h"
#include "SDC.h"                  // SD Card

/*
 * ======================================================================================================================
 * setup()
 * ======================================================================================================================
 */
void setup()
{
  Output_Initialize();
  delay(2000); // prevents usb driver crash on startup, do not omit this
  Output("GateClock");

  // Initialize SD card if we have one.
  Output("SD:INIT");
  SD_initialize();
  if (!SD_exists) {
    Output("!!!HALTED!!!");
    while (true) {
      delay(1000);
    }
  }

  SD_ReadConfigFile();

  // Init DST Enable Pin
  pinMode(DST_PIN, INPUT_PULLUP);   // Internal pullup resistor biases the pin to supply voltage.                            
  if (digitalRead(DST_PIN) == LOW) {
    dst_active = 1;
    Output("DST Active");
  }

  // Give some indication we are alive
  Blink(2, 750);

  rtc_initialize();
  WiFi_initialize(); // Once Connected RTC is update from NTP server

  randomSeed(rtc_unixtime());

  pinMode(ButtonUpPin, INPUT_PULLUP);
  pinMode(ButtonDownPin, INPUT_PULLUP);

  // Initialize external NeoPixel strip separately
  pixels.begin(); // INITIALIZE NeoPixel strip object
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setBrightness(ledBrightness);
  pixels.show();  // Send the updated pixel colors to the hardware.
}

/*
 * ======================================================================================================================
 * loop() - 
 * ======================================================================================================================
 */
void loop()
{
  int h, m, s;
  static int lastminute=0;
  static int lastsecond=0;

  if (!RTC_valid) {  // Must get time from human
    static bool first = true;
    
    delay (1000);
      
    if (first) {
      // Enable Serial if not already
      if (digitalRead(SCE_PIN) != LOW) {
        Serial.begin(9600);
        delay(2000);
        SerialConsoleEnabled = true;
      }  

      // Show invalid time and prompt for UTC Time
      sprintf (msgbuf, "%d:%02d:%02d:%02d:%02d:%02d", 
        now.year(), now.month(), now.day(),
        now.hour(), now.minute(), now.second());
      Output(msgbuf);
      Output("SET RTC w/GMT, ENTER:");
      Output("YYYY:MM:DD:HH:MM:SS");
      first = false;
    }

    // Now two things can happen. User enters valid time or we get time from GPS
    rtc_readserial(); // check for serial input, validate for rtc, set rtc, report result

    // We could ask the network for time again...here

    if (RTC_valid) {
      Output("!!!!!!!!!!!!!!!!!!!");
      Output("!!! Press Reset !!!");
      Output("!!!!!!!!!!!!!!!!!!!"); 
      while (true) {
        delay (1000);
      }     
    }
  }
  else {  // Normal operation

    if (!digitalRead(ButtonUpPin) && !digitalRead(ButtonDownPin)) {
      ledBrightness = 0;
      pixels.setBrightness(ledBrightness);
      sprintf (msgbuf, "Led Off [%d]", ledBrightness);
      Output (msgbuf);
    }
    else if (!digitalRead(ButtonUpPin)) {
      if ((millis()-lastUp) > 250) {
        if (ledBrightness < 10) {
          ledBrightness ++;
        }
        if ((ledBrightness >= 10) && (ledBrightness < 20))  { 
          ledBrightness += 2;
        }
        else if (ledBrightness >= 20) {
          ledBrightness += 5;
        }
        if (ledBrightness > 200) {
          ledBrightness = 200;
        }
        pixels.setBrightness(ledBrightness);
        sprintf (msgbuf, "Led Up [%d]", ledBrightness);
        Output (msgbuf);
        lastUp = millis();
      }
    }  
    else if (!digitalRead(ButtonDownPin)) {
      if ((millis()-lastDown) > 250) {
        if (ledBrightness < 10) {
          ledBrightness --;
        }
        if ((ledBrightness >= 10) && (ledBrightness < 20))  { 
          ledBrightness -= 2;
        }
        else if (ledBrightness >= 20) {
          ledBrightness -= 5;
        }

        if (ledBrightness < 1) {
          ledBrightness = 1;
        }
        pixels.setBrightness(ledBrightness);
        sprintf (msgbuf, "Led Down [%d]", ledBrightness);
        Output (msgbuf);
        lastDown = millis();
      }
    }

    // get the current rtc date-time
    now = rtc.now();   
    h = now.hour();
    m = now.minute();
    s = now.second();

    // Get Network time and update the RTC @ 2:05:00 each day
    if (WiFi_valid && (h==2) && (m==5) && (s==0)) {
      WiFi_UpdateTime();
    }
    
    // Do chime before updating the clock
    if (((m%15) == 0) && s == 0) {  // On the hour and half hour
      checkChime();
    }

    // Only update the clock time when the second has changed.
    if (lastsecond != s) {    
      int mth,mday,yr;
      
      lastsecond=s; 
      displayClock(h, m, s);
      
      mth = now.month();
      mday = now.day();
      yr = now.year();
      sprintf (msgbuf, "%02d:%02d:%02d", h,m,s);
      
      if (DisplayEnabled) {
        if (OLED32) {      
          display32.clearDisplay();
          display32.setTextSize(2); // Draw 2X-scale text
          
          display32.setCursor(0, 0);
          display32.print(msgbuf);

          sprintf (msgbuf, "%02d/%02d/%04d", mth,mday,yr);

          display32.setCursor(0, 17);
          display32.print(msgbuf);
          
          display32.display();
          
          display32.setTextSize(1); // Draw 2X-scale text
          display32.display();
        }
        else {
          display64.clearDisplay();
          display64.setTextSize(2); // Draw 2X-scale text
          
          display64.setCursor(0, 0);
          display64.print(msgbuf);

          sprintf (msgbuf, "%02d/%02d/%04d", mth,mday,yr);

          display64.setCursor(0, 17);
          display64.print(msgbuf);
          
          display64.display();
          
          display64.setTextSize(1); // Draw 2X-scale text
          display64.display();          
        }
      }
      else {
        Output (msgbuf);
      }
    }
 
    // Get Network time and update the RTC @ 2:05:00 each day
    if (WiFi_valid && (h==2) && (m==5) && (s==0)) {
      WiFi_UpdateTime();
    } 
  }
}
