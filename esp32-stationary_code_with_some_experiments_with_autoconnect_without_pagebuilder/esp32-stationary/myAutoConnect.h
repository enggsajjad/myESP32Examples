/**
 * \file myAutoConnect.h
 *
 * \brief Inherits the AutoConnect class and overrides few functions
 *
 * This is the files that contains the functions inherited from AutoConnect Class which need some modfications to add OLED display code.
 * These functions are then over ridden here.
 * 
 * \date 19.08.2019
 */


#if defined(ARDUINO_ARCH_ESP8266)
#define SOFT_RESET()  ESP.reset()
#define SET_HOSTNAME(x) do { WiFi.hostname(x); } while(0)
#elif defined(ARDUINO_ARCH_ESP32)
#define SOFT_RESET()  ESP.restart()
#define  SET_HOSTNAME(x) do { WiFi.setHostname(x); } while(0)
#endif

/// Support for the OLED Display
#include "SSD1306.h"

/// Externally defined OLED Display object
extern SSD1306 disp;//78

/// Timeout to search WiFi already configured.
#define WIFI_REBOOT_TIMEOUT 150000

// initialise udp for network time
//WiFiUDP ntpUDP;

///myAutoConnect Class inherited from AutoConnect
class myAutoConnect : public AutoConnect 
{
public:
 /**
 *  myAutoConnect default constructor. This entry activates WebServer
 *  internally and the web server is allocated internal.
 */
   myAutoConnect(WebServerClass& webServer) : AutoConnect(webServer){
    }
    /**
     *  Wait for establishment of the connection until the specified time expires.
     *  @param  timeout Expiration time by millisecond unit.
     *  @return wl_status_t
     */
    wl_status_t _waitForConnect(unsigned long timeout) {
      wl_status_t wifiStatus;
    
      AC_DBG("Connecting");
      // show also on display
      disp.drawString(0, 0, "SUCHE WIFI...   ");
      disp.display();
      unsigned long st = millis();
      while ((wifiStatus = WiFi.status()) != WL_CONNECTED) {
        if (timeout) {
          if (millis() - st > timeout)
            break;
        }
        AC_DBG_DUMB("%c", '.');
        delay(300);
      }
      AC_DBG_DUMB("%s IP:%s\n", wifiStatus == WL_CONNECTED ? "established" : "time out", WiFi.localIP().toString().c_str());
      return wifiStatus;
    }

/**
 *  Starts establishing WiFi connection without SSID and password.
 */
bool begin(void) {
  return begin(nullptr, nullptr,WIFI_REBOOT_TIMEOUT);//30000
  //return begin(nullptr, nullptr,30000);//
}

/**
 *  Starts establishing WiFi connection.
 *  Before establishing, start the Web server and DNS server for the captive
 *  portal. Then begins connection establishment in WIFI_STA mode. If
 *  connection can not established with the specified SSID and password,
 *  switch to WIFI_AP_STA mode and activate SoftAP.
 *  @param  ssid        SSID to be connected.
 *  @param  passphrase  Password for connection.
 *  @param  timeout     A time out value in milliseconds for waiting connection.
 *  @return true        Connection established, AutoConnect service started with WIFI_STA mode.
 *  @return false       Could not connected, Captive portal started with WIFI_AP_STA mode.
 */
    
    //
    bool begin(const char* ssid, const char* passphrase, unsigned long timeout) {
    bool  cs;
    AC_DBG("Starting from the myAutoConnect\n");
    // Overwrite for the current timeout value.
    _connectTimeout = timeout;
  
    // Start WiFi connection with station mode.
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);
    delay(100);
  
    // Set host name
    if (_apConfig.hostName.length())
      SET_HOSTNAME(_apConfig.hostName.c_str());
  
    // Advance configuration for STA mode.
  #ifdef AC_DEBUG
    String staip_s = _apConfig.staip.toString();
    String staGateway_s = _apConfig.staGateway.toString();
    String staNetmask_s = _apConfig.staNetmask.toString();
    String dns1_s = _apConfig.dns1.toString();
    String dns2_s = _apConfig.dns2.toString();
    //AC_DBG("WiFi.config(IP=%s, Gateway=%s, Subnetmask=%s, DNS1=%s, DNS2=%s)\n", staip_s.c_str(), staGateway_s.c_str(), staNetmask_s.c_str(), dns1_s.c_str(), dns2_s.c_str());
    AC_DBG("WiFi.config(IP=%s, Gateway=%s, Subnetmask=%s, DNS1=%s, DNS2=%s)\n", _apConfig.staip.toString().c_str(), staGateway_s.c_str(), staNetmask_s.c_str(), dns1_s.c_str(), dns2_s.c_str());//Sajjad
  #endif
    if (!WiFi.config(_apConfig.staip, _apConfig.staGateway, _apConfig.staNetmask, _apConfig.dns1, _apConfig.dns2)) {
      AC_DBG("failed\n");
      return false;
    }
  #ifdef ARDUINO_ARCH_ESP8266
    AC_DBG("DHCP client(%s)\n", wifi_station_dhcpc_status() == DHCP_STOPPED ? "STOPPED" : "STARTED");
  #endif
  
    // If the portal is requested promptly skip the first WiFi.begin and
    // immediately start the portal.
    if (_apConfig.immediateStart) {
      cs = false;
      _apConfig.autoReconnect = false;
      _apConfig.autoRise = true;
      AC_DBG("Start the portal immediately\n");
    }
    else {
      // Try to connect by STA immediately.
      if (ssid == nullptr && passphrase == nullptr)
        WiFi.begin();
      else {
        _disconnectWiFi(false);
        WiFi.begin(ssid, passphrase);
      }
      AC_DBG("WiFi.begin(%s%s%s)\n", ssid == nullptr ? "" : ssid, passphrase == nullptr ? "" : ",", passphrase == nullptr ? "" : passphrase);
      cs = _waitForConnect(_connectTimeout) == WL_CONNECTED;
    }
  
    // Reconnect with a valid credential as the autoReconnect option is enabled.
    if (!cs && _apConfig.autoReconnect && (ssid == nullptr && passphrase == nullptr)) {
      // Load a valid credential.
      if (_loadAvailCredential()) {
        // Try to reconnect with a stored credential.
        AC_DBG("autoReconnect loaded SSID:%s\n", reinterpret_cast<const char*>(_credential.ssid));
        const char* psk = strlen(reinterpret_cast<const char*>(_credential.password)) ? reinterpret_cast<const char*>(_credential.password) : nullptr;
        WiFi.begin(reinterpret_cast<const char*>(_credential.ssid), psk);
        AC_DBG("WiFi.begin(%s%s%s)\n", _credential.ssid, psk == nullptr ? "" : ",", psk == nullptr ? "" : psk);
        cs = _waitForConnect(_connectTimeout) == WL_CONNECTED;
      }
    }
    _currentHostIP = WiFi.localIP();
  
    // Rushing into the portal.
    if (!cs) {
  
      // The captive portal is effective at the autoRise is valid only.
      if (_apConfig.autoRise) {
  
        // Change WiFi working mode, Enable AP with STA
        WiFi.setAutoConnect(false);
        _disconnectWiFi(false);
  
        // Activate the AP mode with configured softAP and start the access point.
        WiFi.mode(WIFI_AP_STA);
        while (WiFi.getMode() != WIFI_AP_STA) {
          delay(1);
          yield();
        }
  
        // Connection unsuccessful, launch the captive portal.
  #if defined(ARDUINO_ARCH_ESP8266)
        if (!(_apConfig.apip == IPAddress(0, 0, 0, 0) || _apConfig.gateway == IPAddress(0, 0, 0, 0) || _apConfig.netmask == IPAddress(0, 0, 0, 0))) {
          _config();
        }
  #endif
        WiFi.softAP(_apConfig.apid.c_str(), _apConfig.psk.c_str(), _apConfig.channel, _apConfig.hidden);
        do {
          delay(100);
          yield();
        } while (WiFi.softAPIP() == IPAddress(0, 0, 0, 0));
  #if defined(ARDUINO_ARCH_ESP32)
        if (!(_apConfig.apip == IPAddress(0, 0, 0, 0) || _apConfig.gateway == IPAddress(0, 0, 0, 0) || _apConfig.netmask == IPAddress(0, 0, 0, 0))) {
          _config();
        }
  #endif
        if (_apConfig.apip != IPAddress(0, 0, 0, 0)) {
          do {
            delay(100);
            yield();
          } while (WiFi.softAPIP() != _apConfig.apip);
        }
        _currentHostIP = WiFi.softAPIP();
        AC_DBG("SoftAP %s/%s Ch(%d) IP:%s %s\n", _apConfig.apid.c_str(), _apConfig.psk.c_str(), _apConfig.channel, _currentHostIP.toString().c_str(), _apConfig.hidden ? "hidden" : "");
        
          disp.clear();
          disp.drawString(0, 0, "BITTE           ");
          disp.drawString(0, 8, "SENSORKNOTEN    ");
          disp.drawString(0, 16, "EINRICHTEN      ");
          disp.drawString(0, 24, "UNTER           ");
          disp.drawString(0, 32, "SENSOR-WIFI     ");
          disp.drawString(0, 40, "SSID:"); 
          disp.drawString(38, 40, esp32_chipid.c_str());
          disp.drawString(0, 47, "PW: 12345678    ");
          disp.drawString(0, 56, "IP:192.168.244.1");
          disp.display(); 
  
        // Fork to the exit routine that starts captive portal.
        cs = _onDetectExit ? _onDetectExit(_currentHostIP) : true;
  
        // Start Web server when TCP connection is enabled.
        _startWebServer();
  
        // Start captive portal without cancellation by DetectExit.
        if (cs) {
          // Prepare for redirecting captive portal detection.
          // Pass all URL requests to _captivePortal to disguise the captive portal.
          _startDNSServer();
  
          // Start the captive portal to make a new connection
                  // Start the captive portal to make a new connection
    bool  hasTimeout = false;
        _portalAccessPeriod = millis();
     AC_DBG("Trying the portal\n");
        while (WiFi.status() != WL_CONNECTED && !_rfReset) {
      //AC_DBG_DUMB("%c", '.');
      //delay(300);
          handleClient();
          // Force execution of queued processes.
          yield();
          // Check timeout
          if ((hasTimeout = _hasTimeout(_apConfig.portalTimeout))) {
            AC_DBG("CP timeout exceeded:%ld\n", millis() - _portalAccessPeriod);
            break;
          }
        }
        cs = WiFi.status() == WL_CONNECTED;
        /*
          bool  hasTimeout = false;
          _portalAccessPeriod = millis();
          while (WiFi.status() != WL_CONNECTED && !_rfReset) {
            handleClient();
            // Force execution of queued processes.
            yield();
            // Check timeout
            if ((hasTimeout = _hasTimeout(_apConfig.portalTimeout))) {
              AC_DBG("CP timeout exceeded:%ld\n", millis() - _portalAccessPeriod);
              break;
            }
          }
          cs = WiFi.status() == WL_CONNECTED;*/
  
          // If WLAN successfully connected, release DNS server.
          if (cs) {
            _dnsServer->stop();
            _dnsServer.reset();
            AC_DBG("DNS server stopped\n");
          }
          // Captive portal staying time exceeds timeout,
          // Close the portal if an option for keeping the portal is false.
          else if (hasTimeout) {
            if (_apConfig.retainPortal) {
              _purgePages();
              AC_DBG("Maintain portal\n");
            }
            else {
              _stopPortal();
            }
          }
        }
      }
      else {
        AC_DBG("Suppress autoRise\n");
      }
    }
  
    // It doesn't matter the connection status for launching the Web server.
    if (!_responsePage)
      _startWebServer();
  
    return cs;
  }
    //
};
