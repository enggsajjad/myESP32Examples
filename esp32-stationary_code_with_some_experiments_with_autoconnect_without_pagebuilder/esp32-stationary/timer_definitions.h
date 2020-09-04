/**
 * \file timer_definitions.h
 *
 * \brief variable and function definitions for timings
 *
 * This is the files that contains the variables, macros, pins definitions and functions for timing related things.
 * 
 * \date 19.08.2019
 */

/// will store the last time the event (in loop) was triggered for sending sensor values to FrostManager
unsigned long previousMillis_sensor = 0; 
/// will store the last time the event (in loop) was triggered sending sensor values to  madavi
unsigned long previousMillis_send2madavi = 0; 
/// will store the last time the event (in loop) was triggered sending sensor values to luftdaten
unsigned long previousMillis_send2luftdaten = 0;
/// time interval to send values to Frostmanager 
const long sensor_interval = SENSOR_INTERVAL;   
/// time interval to send values to MADAVI
const long send2madavi_interval = SEND2MADAVI_INTERVAL; 
/// time interval to send values to LUFTDATEN
const long send2luftdaten_interval = SEND2LUFTDATEN_INTERVAL;
///timeout for AP capitive portal connection; 
const long apmode_reboot_timeout = APMODE_REBOOT_TIMEOUT;   
/// whether to reboot if AP does not find
bool enable_apmode_reboot = ENABLE_APMODE_REBOOT;     
/// whether to reboot daily at some specific time
bool enable_daily_reboot = ENABLE_DAILY_REBOOT;     


/**************************************************************************/
/*!
    @brief  ISR called for Reset ESP32
    @returns void
*/
/**************************************************************************/
void IRAM_ATTR resetModule() {
  ets_printf("reboot\n");
  esp_restart();
}
