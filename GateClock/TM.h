/*
 * ======================================================================================================================
 *  TM.h - Time Management
 *  
 *  We read all time references directly from the RTC clock. System clock is not set or used
 * ======================================================================================================================
 */

#define PCF8523_ADDRESS 0x68       // I2C address for PCF8523
int dst_active=0;

/*
 * ======================================================================================================================
 *  RTC Setup
 * ======================================================================================================================
 */
RTC_PCF8523 rtc;              // RTC_PCF8523 rtc;

DateTime now;
char timestamp[32];
bool RTC_valid = false;
bool RTC_exists = false;

/* 
 *=======================================================================================================================
 * rtc_unixtime() - 
 *=======================================================================================================================
 */
uint32_t rtc_unixtime() {
  now = rtc.now();
  return (now.unixtime());
}

/* 
 *=======================================================================================================================
 * rtc_timestamp() - Read from RTC and set timestamp string
 *=======================================================================================================================
 */
void rtc_timestamp() {
  now = rtc.now(); // get the current rtc date-time

  // ISO_8601 Time Format
  sprintf (timestamp, "%d-%02d-%02dT%02d:%02d:%02d", 
    now.year(), now.month(), now.day(),
    now.hour(), now.minute(), now.second());
}

/* 
 *=======================================================================================================================
 * rtc_initialize()
 *=======================================================================================================================
 */
void rtc_initialize() {

  if (!rtc.begin()) { // Always returns true
     Output("RTC:NF ERR");
     return;
  }
 
  if (!I2C_Device_Exist(PCF8523_ADDRESS)) {
    Output("ERR:RTC-I2C NOTFOUND");
    delay (5000);
    return;
  }

  RTC_exists = true; // We have a clock hardware connected

  rtc_timestamp();
  sprintf (msgbuf, "%s RAW", timestamp);
  Output (msgbuf);

  // Do a validation check on the year. 
  // Asumption is: If RTC not set, it will not have the current year.
  
  if ((now.year() >= TM_VALID_YEAR_START) && (now.year() <= TM_VALID_YEAR_END)) {
    RTC_valid = true;
    Output("RTC:VALID");
  }
  else {
    Output ("RTC:NOT VALID");
  }
}

/*
 * =======================================================================================================================
 * rtc_readserial() - // check for serial input, validate for rtc, set rtc, report result
 * =======================================================================================================================
 */
bool rtc_readserial()
{
  boolean ready = false;
  int cnt = 0;
  char buffer[32];
  char *p, *token;
  int year, month, day, hour, minute, second;
  
  while (Serial.available()) {
    char c = Serial.read();
    buffer[cnt++] = c;
    if ((c == '\n') || (cnt == 31) ){
      buffer[cnt] = '\0';  // Note: there will be a \r\n on end of string in buffer
      cnt = 0;
      Serial.flush(); // if anything left in the Serial buffer, get rid of it

      // Validate User input for a good date and time
      p = &buffer[0];
      token = strtok_r(p, ":", &p);
      if (isnumeric(token) && (year = atoi (token)) && (year >= 2021) && (year <= 2031) ) {
        token = strtok_r(p, ":", &p);
        if (isnumeric(token) && (month = atoi (token)) && (month >= 1) && (month <= 12) ) {
          token = strtok_r(p, ":", &p);        
          if (isnumeric(token) && (day = atoi (token)) && 
               (
                 ( (day>=1  && day<=31) && (month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12) ) ||
                 ( (day>=1  && day<=30) && (month==4 || month==6 || month==9 || month==11) ) ||
                 ( (day>=1  && day<=28) && (month==2) ) ||
                 ( (day==29)            && (month==2) && ( (year%400==0) || ( (year%4==0) && (year%100!=0) ) ) )
                ) 
             ) {
            token = strtok_r(p, ":", &p);
            hour = atoi (token);
            if ( (isnumeric(token) && (hour >= 0) && (hour <= 23)) ) {
              token = strtok_r(p, ":", &p);
              minute = atoi (token);
              if ( (isnumeric(token) && (minute >= 0) && (minute <= 59)) ) {
                token = strtok_r(p, "\r", &p);
                second = atoi (token);
                if ( (isnumeric(token) && (second >= 0) && (second <= 59)) ) { 
                  sprintf (msgbuf, ">%d.%d.%d.%d.%d.%d", 
                     year, month, day, hour, minute, second);
                  rtc.adjust(DateTime(year, month, day, hour, minute, second));
                  Output("RTC: Set");
                  RTC_valid = true;
                  rtc_timestamp();
                  sprintf (msgbuf, "%s=", timestamp);
                  Output (msgbuf);
                  return(true);
                }
                else {
                  sprintf (msgbuf, "Invalid Second: %s", token);
                  Output(msgbuf);
                  return(false);
                }
              }
              else {
                sprintf (msgbuf, "Invalid Minute: %s", token);
                Output(msgbuf);
                return(false);
              }
            }
            else {
              sprintf (msgbuf, "Invalid Hour: %s", token);
              Output(msgbuf);
              return(false);
            }
          }
          else {
            sprintf (msgbuf, "Invalid Day: %s", token);
            Output(msgbuf);
            return(false);
          }
        }
        else {
          sprintf (msgbuf, "Invalid Month: %s", token);
          Output(msgbuf);
          return(false);
        }                
      }
      else {
        sprintf (msgbuf, "Invalid Year: %s", token);
        Output(msgbuf);
        return(false);
      }
    } // if line
  } // while
  return(false);
}
