/*
 * ======================================================================================================================
 *  WiFi.h - WiFi Functions
 * ======================================================================================================================
 */
WiFiClient wifi;
bool WiFi_valid = false;          // True if this is a Feather WiFi Board
bool WiFiFeather = false;

/*
 * ======================================================================================================================
 * WiFi.status() returns:
 * WL_NO_SHIELD:      No Wi-Fi shield is present or detected. This indicates that the Wi-Fi hardware is not available.
 * WL_IDLE_STATUS:    The Wi-Fi shield is idle and ready to connect. It indicates that the shield is not currently 
 *                    connected to a network but is prepared to start a connection attempt.
 * WL_NO_SSID_AVAIL:  No SSID (network name) is available. This indicates that the Wi-Fi shield cannot find 
 *                    the specified SSID.
 * WL_SCAN_COMPLETED: A scan for available networks has completed. This status indicates that a network scan is finished, 
 *                    and the device can now proceed to connect to a network.
 * WL_CONNECTED:      The device is successfully connected to a Wi-Fi network. This status indicates that the connection 
 *                    to the specified network is active.
 * WL_CONNECT_FAILED: The attempt to connect to a Wi-Fi network failed. This status indicates that the connection 
 *                    attempt did not succeed.
 * WL_DISCONNECTED:   The device has disconnected from the Wi-Fi network. This status indicates that the connection 
 *                    was lost or terminated.
 * ======================================================================================================================
 */
int WiFiStatus = WL_NO_SHIELD;

#define NTP_PACKET_SIZE 48
#define NTP_TIMEOUT 5000  // ms
WiFiUDP udp;
unsigned int localPort = 2390;

/*
 * ======================================================================================================================
 * WiFi_SendNTP() - Send a NTP request
 * ======================================================================================================================
 */
void WiFi_SendNTP(IPAddress& address) {
  byte packetBuffer[NTP_PACKET_SIZE];
  
  // Initialize packetBuffer
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  
  // Set the first byte to 0b11100011, which is LI (Leap Indicator) = 3 (not in sync),
  // VN (Version Number) = 4, and Mode = 3 (Client)
  packetBuffer[0] = 0b11100011;
  
  // Send the NTP request
  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

/*
 * ======================================================================================================================
 * Wifi_GetTime() - Make a NTP request
 * ======================================================================================================================
 */
unsigned long Wifi_GetTime() {
  IPAddress ntpServerIP;
    
  Output ("WiFi GetTime()");

  udp.begin(localPort);   // Start UDP
  WiFi.hostByName(cf_ntpserver, ntpServerIP);  // Do DNS Lookup to get NTP server's IP

  Output ("WiFi NTP Req");
  WiFi_SendNTP(ntpServerIP);

  unsigned long startMillis = millis();
  Output ("WiFi NTP Wait");
  while (!udp.parsePacket()) {
    if (millis() - startMillis >= NTP_TIMEOUT) {
      return 0; // Timeout
    }
  }

  Output ("WiFi UDP Read");
  byte packetBuffer[NTP_PACKET_SIZE];
  udp.read(packetBuffer, NTP_PACKET_SIZE);
  
  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  
  const unsigned long seventyYears = 2208988800UL; // Epoch starts at 1970
  unsigned long epoch = secsSince1900 - seventyYears; // If Local Time Zone desired add  "+ timeZone * 3600"

  Output ("WiFi NTP OK");
  return (epoch);
}

/*
 * ======================================================================================================================
 * WiFi_UpdateTime() - Function to get network time and set rtc
 * ======================================================================================================================
 */
void WiFi_UpdateTime() {
  Output("WiFi UpdateTime");

  WiFiStatus = WiFi.status();

  if ((WiFiStatus == WL_CONNECTED) && RTC_exists) {
    unsigned long networktime = Wifi_GetTime();

    // Adjust for Time Zone
    networktime += (cf_utc_offset * 3600); // Ex MST = -7hrs diff from UTC * 3600s 

    if (dst_active) {
      networktime += 3600; // Add the hour so MDT is -6hrs diff
    }

    DateTime dt_networktime = DateTime(networktime);
    if ((dt_networktime.year() >= TM_VALID_YEAR_START) && (dt_networktime.year() <= TM_VALID_YEAR_END)) {
      rtc.adjust(dt_networktime);
      Output("WiFi RTC SET");
      rtc_timestamp();
      sprintf (msgbuf, "%s WiFi", timestamp);
      Output (msgbuf);
      RTC_valid = true;
    }
    else {
      Output("WiFi RTC ERROR");
    }
  }
  else {
    Output("WiFi UpdateTime Error");
  }
}

/*
 * ======================================================================================================================
 * WiFi_Feather() - check for the presence of WiFi
 * ======================================================================================================================
 */
bool WiFi_Feather() {
  // check for the presence of the shield:
  WiFiStatus = WiFi.status();
  if (WiFiStatus == WL_NO_SHIELD) {
    WiFi_valid = false;
  }
  else {
    WiFi_valid = true;
  }
  return (WiFi_valid);
}

/*
 *=======================================================================================================================
 * WiFi_status()
 *=======================================================================================================================
 */
void WiFi_status() {
  // print the SSID of the network you're attached to:
  sprintf (msgbuf, "WiFi_SSID->[%s]", WiFi.SSID());
  Output(msgbuf);

  // print your WiFi 101 Shield's IP address:
  IPAddress ip = WiFi.localIP();
  sprintf (msgbuf, "WiFi_IP->[%d.%d.%d.%d]", ip[0],ip[1],ip[2],ip[3]);
  Output(msgbuf);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  sprintf (msgbuf, "WiFi_RSSI->[%d dBm]", rssi);
  Output(msgbuf);
}

/* 
 *=======================================================================================================================
 * WiFi_connect - return true if called WiFi.begin
 *=======================================================================================================================
 */
bool WiFi_connect() {
  if (strcmp(cf_wifi_ssid,"") != 0) {
    Output ("WiFiConn SSID");
    sprintf (msgbuf, "WiFi SSID: [%s]", cf_wifi_ssid);
    Output(msgbuf);
    
    if ((strcmp(cf_wifi_pw,"") == 0) && (strcmp(cf_wifi_wepkey,"") == 0)) {
      // Connect to Open WiFi Network
      Output("WiFi Type: Open");
      WiFiStatus = WiFi.begin(cf_wifi_ssid);
      return (true);
    }
    else if ((strcmp(cf_wifi_pw,"") != 0) && (strcmp(cf_wifi_wepkey,"") == 0)) {
      // connect to WPA-encrypted Wifi network
      Output("WiFi Type: WPA/2");
      WiFiStatus = WiFi.begin(cf_wifi_ssid, cf_wifi_pw);
      return (true);
    }
    else if ((strcmp(cf_wifi_pw,"") != 0) && (strcmp(cf_wifi_wepkey,"") != 0)) {
       // Connect to WEP-secured network using default key index (typically 0).
      Output("WiFi Type: WEP");
      WiFiStatus = WiFi.begin(cf_wifi_ssid, cf_wifi_wepkey);
      return (true);
    }
    else {
      // Error unknown Method
      sprintf (msgbuf, "WiFi CONERR: NO PW/WKEY");
      Output(msgbuf);
      return(false);   
    }
  }
  else {
    // Error no SID defined
    sprintf (msgbuf, "WiFi CONERR: NO SSID");
    Output(msgbuf);
    return(false);
  }
}

/* 
 *=======================================================================================================================
 * WiFi_connect_wait()
 *=======================================================================================================================
 */
void WiFi_connect_wait(int wait_seconds) {
  Output ("WiFiConnWait()");
  if (WiFi_connect()) {
    // Wait until connected or timeout
    
    unsigned long startTime = millis();
    while ((WiFi.status() != WL_CONNECTED) && (millis() - startTime < (wait_seconds*1000) ) ) {
      Output("WiFi Connecting....");
      delay(1000);
    }
    if (WiFi.status() != WL_CONNECTED) {
      Output("WiFi Connect Timeout");
    }
    else {
      Output("WiFi Connected");
      IPAddress ip = WiFi.localIP();
      char ipStr[16]; // Buffer to hold the IP address as a string
      sprintf(msgbuf, "WiFi IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
      Output(msgbuf);
  
      long rssi = WiFi.RSSI();
      sprintf(msgbuf, "WiFi RSSI: %ld", rssi);
      Output(msgbuf); 
    }
  }
  else {
    Output("WiFi Connected Failed");
  }
}

/*
 *=======================================================================================================================
 * WiFi_initialize()
 *=======================================================================================================================
 */
void WiFi_initialize() {
  Output ("WiFi INIT");
  WiFi.setPins(8,7,4,2);

  if (WiFi_Feather()) {
    if ( (strcmp(cf_wifi_ssid,"") == 0) || (strcmp(cf_ntpserver,"") == 0)) {
      // No WiFi Config
      WiFi_valid = false;
      Output("WiFi No Config");
    }
    else {
      Output("WiFi Connect Wait");    
      WiFi_connect_wait(30);
      WiFi_UpdateTime();
    }
  }
  else {
    Output ("WiFi NO WiFi");
  }
}
