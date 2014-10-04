#ifndef _rFlea_Arduino_h
#define _rFlea_Arduino_h

#include <ANT.h>
#include <inttypes.h>
#include "antmessage.h"
#include "antdefines.h"
#include "Arduino.h"
#include <EEPROM.h>

class rFlea_Arduino 
{
public:
  rFlea_Arduino();
  
  ////funtions
  void init();
  void connect();
  void disconnect();

  void update();
  bool ready();
  void send(byte* message); 
  bool READY;
  int SerialNumber;

  void register_onMessage(void (*callback_funct)(byte * message));
  void register_onSync(void (*callback_funct)());

private:
  ANT ant;
  antConfiguration CH0;
  
    ////funtions
  int getSerial();
  void ready(bool state);
  void parseData(byte* message);
  int getSerialNumber();

  
  void (*onMessageCallback)(byte * message);
  void (*onSyncCallback)();


};

#endif