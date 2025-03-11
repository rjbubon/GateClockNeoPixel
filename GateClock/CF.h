/*
 * 
 * ======================================================================================================================
 *  CF.h - Configuration File - CONFIG.TXT
 * ======================================================================================================================
 */

/* 
 * ======================================================================================================================
 #
 # CONFIG.TXT
 #
 # Line Length is limited to 60 characters
 #123456789012345678901234567890123456789012345678901234567890

 #################################################
 # WiFi Feather Set the Below
 #################################################
 # WiFi Encryption Methods (WEP, WPA/2, Open Network)  
 #   WPA/2  set wifi_ssid and wifi_pw
 #   WEP    set wifi_ssid, wifi_wepkey
 #   Open   set wifi_ssid
 wifi_ssid=
 wifi_pw=
 #
 # WEPKEY can be 40bit (10 HEX Characters) or 128bit WEP key (26 HEX Characters)
 wifi_wepkey=

 # Time Server
 ntpserver=pool.ntp.org

 # UTC Offset -7 MST, -6 MDT
 utc_offset=-7
 * ======================================================================================================================
 */

/*
 * ======================================================================================================================
 *  Define Global Configuration File Variables
 * ======================================================================================================================
 */
// WiFi
const char *cf_wifi_ssid   = "";
const char *cf_wifi_pw     = "";
const char *cf_wifi_wepkey = "";

// Time Server
const char *cf_ntpserver   = "pool.ntp.org";

// UTC Offset
int cf_utc_offset=-7;
