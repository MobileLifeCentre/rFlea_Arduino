/*********************************************************************
 * rFlea_Arduino library v1.0
 * 5 october 2014
 *
 * This is an example sketch for the rFlea ANT+ arduino compatible
 * 
 * This example is shows how to receive from a Master device, or "beacon". 
 * It can be used with the beacon example from this libraries, or setting 
 * "true" in the JavaScript example:
 *         var antConencted=AntInterface.addNewChannel(true,57433,4);
 * The second parameter is the ID of the beacon. Set up here the same ID 
 * with the instruction:
 *      rflea.set_beacon_serial_number(your ID);
 * if you don't set up it will pair with any.
 * The examples also includes how to enable Signal Strength reception. This
 * is an indicator that return how powerful the received signal is at any
 * moment. The example also flashes rFlea LED every time a beacon is received.
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

int led = 13;

void setup() {
  //rFlea object uses Serial and need to be at 57600
  //don't change it or the wireless will not work
  //you can still use it as debug normally.
  Serial.begin(57600);

  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);

  //Registrer the functions that will be called to help syncronisation,
  // low power, receive data and signal strength indicator.
  rflea.register_onMessageBeaconListener(onMessageBeaconListener);
  rflea.register_onRssi(onRssi);

  //Set our BEACON_LISTENER channel. BEACON_LISTENER will connect to a SESNOR, SENSOR_RX or BEACON profiles
  rflea.rFlea_profile(BEACON_LISTENER);

  
  //rflea.set_beacon_serial_number(your_ID_HERE); //Set the serial number of your beacon if needed. It will ONLY connect to this beacon. Otherwise will conenct to the first found.
  
  //Get the unique serial number from this rFlea and print it
  serialNumber=rflea.my_serial_number(); //print the serial number of this board
  Serial.println(" ");
  Serial.println(serialNumber);
  
  //Reset and Initialize the ANT+. We print the unic serial number for this board.
  rflea.init();
  
  //we enable Received Signal Strength Indicator (RSSI)
  rflea.enable_rssi(); //enable RSSI. Dont forget to add the listener.

  //Connect!!
  rflea.connect(BEACON_LISTENER);
}
void loop() {
  //Update rFlea every loop.
  rflea.update();
}

//This Function will be called everytime we receive something
void onMessageBeaconListener(byte* message){ 
  //Change to true if you want to print the data received 
  if(false){
    Serial.println("");
    Serial.print("Rx from: ");
    for(int i=0;i<8;i++){
      Serial.print(message[i],HEX);    
      Serial.print(" ");
    }  
    Serial.println("");
  }
   
   //Blinks the LED for every message received
  digitalWrite(led, HIGH); //If the first byte is 0 then LED off
  delay(25);
    digitalWrite(led, LOW); //If the first byte is other than 0 then LED on
    delay(25);
}

void onRssi(byte rssi){ //callback that will tell the rssi (indicator)
  if(false){ // true if you want to print the values of RSSI
    Serial.println("");
    Serial.print("RSSI: ");
      Serial.print(rssi);    
    Serial.println("");
  }
}













