/*
  reboots after certain uptime
*/
// true for enable, false for disable
//
//bool enable_reboot = ENABLE_REBOOT;
//unsigned long rebootTimeout = REBOOT_TIMEOUT;  

//hw_timer_t *timer = NULL; // initialise hardware timer





// counter for milliseconds after start
unsigned long previousMillis_sensor = 0; // will store the last time the event (in loop) was triggered
unsigned long previousMillis_send2madavi = 0; // will store the last time the event (in loop) was triggered
unsigned long previousMillis_send2luftdaten = 0; // will store the last time the event (in loop) was triggered
const long sensor_interval = SENSOR_INTERVAL;
const long send2madavi_interval = SEND2MADAVI_INTERVAL;
const long send2luftdaten_interval = SEND2LUFTDATEN_INTERVAL;
const long apmode_reboot_timeout = APMODE_REBOOT_TIMEOUT;
bool enable_apmode_reboot = ENABLE_APMODE_REBOOT;
bool enable_daily_reboot = ENABLE_DAILY_REBOOT;


// reboot esp if called
void IRAM_ATTR resetModule() {
  ets_printf("reboot\n");
  esp_restart();
}
