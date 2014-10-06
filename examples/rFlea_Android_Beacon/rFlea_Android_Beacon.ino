/*********************************************************************
 * rFlea_Arduino library v1.0
 * 5 october 2014
 *
 * This is an example sketch for the rFlea ANT+ arduino compatible
 * 
 * This example sets rFlea to beacon a message every 2 seconds. 
 * Use this example if you have an application where only ID is considered. 
 * It can be used as keyring beacon or RFID substitute. Once the rFlea sets
 * up everything it goes to shutdown mode leaving the RF the only working 
 * system onboard, transmitting forever the payload. In this case, the first 
 * two data fields contain the Serial number of the board, like an RFID tag.
 *
 * rFlea are prototyping boards with ultra low power wireless ANT:
 * Refer to http://www.thisisant.com/ for more information
 * 
 * rFlea are based in ATmega 328, same as Arduino Pro Mini or Arduino
 * LilyPad. Use those boards to flash rFlea in the tools menu.
 * 
 * Written by Jordi Solsona. September 2014. Email: Jordisb@kth.se
 * 
 * This example shows how to connect to a Android mobile phone or tablet
 * compatible with ANT+
 * Refer to http://www.thisisant.com/consumer/ant-101/ant-in-phones
 * 
 * The rFlea webapp must be downloded and installed in the device.
 *********************************************************************/

#include <rFlea_Arduino.h>
#include <EEPROM.h>

//rFlea Object and constructor.
rFlea_Arduino rflea = rFlea_Arduino();
unsigned int serialNumber;

void setup() {
  //rFlea object uses Serial and need to be at 57600
  //don't change it or the wireless will not work
  //you can still use it as debug normally.
  Serial.begin(57600);  

  //Registrer the functions that will be called to help syncronisation
  // and low power.
  rflea.register_onSync(onSync);
  
  //Set our SENSOR channel. SENSOR can oly transmit
  rflea.rFlea_profile(SENSOR);
  
  //Change default sensor update frequency to 0.5Hz, or 1 transmission every 2 seconds.
  //_0_5_Hz,_1_Hz,_2_Hz,_4_Hz,_8_Hz,_16_Hz,_32_Hz,_64_Hz
  rflea.set_period(SENSOR,_0_5_Hz);
  
  //Get the unique serial number from this rFlea and print it
  serialNumber=rflea.my_serial_number();
  Serial.println(" ");
  Serial.print("S/N: ");
  Serial.println(serialNumber);
  
  //Reset and Initialize the ANT+.
  rflea.init();

  //Connect!!
  rflea.connect(SENSOR);
}
void loop() {
  //Update rFlea every loop.
  rflea.update();
}

//In case we are a Sensor, this function will be called
// every time we are ready to send next message. Use the
// function rFlea.send(byte[]) to send the 8 bytes of data
void onSync(){
  byte  message[8];
  message[0] = serialNumber>>8;
  message[1] = serialNumber& 0x00FF;
  message[2] = 0;
  message[3] = 0;
  message[4] = 0;
  message[5] = 0;
  message[6] = 0;
  message[7] = 0;
  rflea.send(SENSOR,message);
}














