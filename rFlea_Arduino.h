#ifndef _rFlea_Arduino_h
#define _rFlea_Arduino_h

#include <ANT.h>
#include <inttypes.h>
#include "antmessage.h"
#include "antdefines.h"
#include "Arduino.h"
#include <EEPROM.h>


#define SENSOR 0 //master tx only
#define SENSOR_RX 1 //master bidirectional
#define BEACON_LISTENER 2 // SLAVE only Rx
#define MAX_NUM_CHANNEL 3

#define _0_5_Hz 0
#define _1_Hz 1
#define _2_Hz 2
#define _4_Hz 4
#define _8_Hz 8
#define _16_Hz 16
#define _32_Hz 32
#define _64_Hz 64


class rFlea_Arduino 
{
public:
  rFlea_Arduino();
  
  ////funtions
  void init();
  void rFlea_profile(byte rFlea_profile);
  void set_period(byte rFlea_profile,byte _period);
  void set_RF_frequency(byte rFlea_profile,byte _freq);
  void set_beacon_serial_number(int beacon_serial);
  void enable_rssi();
  int my_serial_number();
  void connect(byte rFlea_profile);
  void disconnect(byte rFlea_profile);
  void disconnect_all();

  void update();
  bool ready();
  void send(byte rFlea_profile,byte* message); 
  bool READY;
  int SerialNumber;

  void register_onMessage(void (*callback_funct)(byte rFlea_profile, byte * message));
  void register_onMessageSensor(void (*callback_funct)(byte * message));
  void register_onMessageSensorRx(void (*callback_funct)(byte * message));
  void register_onMessageBeaconListener(void (*callback_funct)(byte * message));
  void register_onRssi(void (*callback_funct)(byte rssi));
  void register_onSync(void (*callback_funct)());

private:
  ANT ant;
  antConfiguration mCH[MAX_NUM_CHANNEL];
  
  bool onMessageCallback_ACTIVE;
  bool onMessageSensorCallback_ACTIVE;
  bool onMessageSensorRxCallback_ACTIVE;
  bool onMessageBeaconListenerCallback_ACTIVE;
  bool onRssi_ACTIVE;
  bool onSyncCallback_ACTIVE;
  
    ////funtions
  int getSerial();
  void ready(bool state);
  void parseData(byte* message);
  int getSerialNumber();

  
  void (*onMessageCallback)(byte rFlea_profile, byte * message);
  void (*onMessageSensorCallback)(byte * message);
  void (*onMessageSensorRxCallback)(byte * message);
  void (*onMessageBeaconListenerCallback)(byte * message);
  void (*onRssiCallback)(byte rssi);
  void (*onSyncCallback)();


};

#endif