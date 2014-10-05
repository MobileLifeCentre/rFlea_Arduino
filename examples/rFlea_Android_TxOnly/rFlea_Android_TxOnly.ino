/*********************************************************************
 * rFlea_Arduino library v1.0
 * 5 october 2014
 *
 * This is an example sketch for the rFlea Stiva ANT+ arduino compatible
 *
 * This example is known as Transmition only (TxOnly). It refers to the
 * only ability of sending data to the phone, tablet or any other rFlea is
 * set up correctly (Beacon_listener example). This set up can't receive 
 * any data, this is useful for applications that don't need to receive and
 * save energy preventing listenning. Low power configuration.
 * 
 * rFlea Stiva are prototyping boards with ultra low power wireless ANT:
 * Refer to http://www.thisisant.com/ for more information
 * 
 * rFlea Stiva are based in ATmega 328, same as Arduino Pro Mini or Arduino
 * LilyPad. Use those boards to flash rFlea Stiva in the tools menu.
 * 
 * Written by Jordi Solsona. September 2014. Email: Jordisb@kth.se
 * 
 * This example shows how to connect to a Android mobile phone or tablet
 * compatible with ANT+
 * Refer to http://www.thisisant.com/consumer/ant-101/ant-in-phones
 * 
 * The rFlea webapp must be downloded and installed in the device.
 * 
 * 
 * Ultra low power is achieved by only sending data. Use this example in
 * case of sensors or beacons.
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

  //Set digital and analog pins
  pinMode(3,INPUT);
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT);

  //Set pullup resistors
  digitalWrite(3, HIGH);  
  digitalWrite(10, HIGH); 
  digitalWrite(11, HIGH); 
  digitalWrite(12, HIGH);  


  //Registrer the functions that will be called to help syncronisation 
  // and low power.
  rflea.register_onSync(onSync);
  
  //Set our SENSOR channel. SENSOR can oly transmit
  rflea.rFlea_profile(SENSOR);
  
    //Get the unique serial number from this rFlea and print it
  serialNumber=rflea.my_serial_number();
  Serial.println(" ");
  Serial.println(serialNumber);

  //Reset and Initialize the ANT+.
  rflea.init();

  //Connect!!
  rflea.connect(SENSOR);
}
void loop() {
  //Update rFlea every loop.
  rflea.update();

  // use this function to close radio activity
  //rflea.disconnect();
  //and rflea.connect to start again sending
}

//In case we are a Sensor, this function will be called
// every time we are ready to send next message. For a proper
// low power usage, use this function to include all you sketch
// instructions before sending the data, like the loop() function
// would be used in a normal arduino sketch. Be aware that this 
// Function will be called few time per second. For HIGH speed 
// codes keep using the loop() function and refering to this one
// just for sending. Use the function rFlea.send(byte[]) to 
// send the 8 bytes of data.
// WARNING: don't call this function from you sketch, the function
// will automatically be called when new data is possible to send
// The message you can send every time slot is a 8bytes array (message)
// In this example there are random numbers and fixed ones.
void onSync(){
  byte  message[8];

  message[0] = unsigned(analogRead(A4)/4);//arduino is 10 bits, 1byte is 8 bits.
  //Dividing by 4 we remove 2 bits
  message[1] = unsigned(analogRead(A5)/4);
  message[2] = digitalRead(3);
  message[3] = digitalRead(10);
  message[4] = digitalRead(11);
  message[5] = digitalRead(12);
  message[6] = 0; //Empty
  message[7] = 0; //Empty
  rflea.send(SENSOR,message);
}













