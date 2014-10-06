rFlea_Arduino
=============

rFlea is an open-source Arduino compatible board based ATmega 328 (http://www.atmel.com/Images/doc8161.pdf), same used for the following Arduino baords:
  - Arduino UNO (http://arduino.cc/en/Main/arduinoBoardUno)
  - Arduino Lilypad(http://arduino.cc/en/Main/arduinoBoardLilyPad)
  - Arduino Pro Mini (http://arduino.cc/en/Main/ArduinoBoardMini)
  
For any reference on programming, libraries and how to work with rFlea, use the previous Arduino boards as rFlea is 100% compatible with them or just check this list for compatibility (http://arduino.cc/en/Products.Compare). rFlea has a same lebeled pinout.
*you can program rFlea Arduino using the Arduino Pro Mini board, and its usual FTDI cable/board used in such a boards.

rFlea has ANT+ onboard, ANT+ is a ultra low power wireless tranceiver used in sports activities, sensor body networks and medical equipment. ANT+ allows very long lasting sensors with small coin batteries. With more information check http://www.thisisant.com/consumer/ant-101/what-is-ant.

rFlea is compatible with most body and sports sensors and compatible with some old phones and mostly with new Android phones, together with sports watches, bike computers and more compatibilities. You can also connect your rFlea to a raspberry pi, or you own computer. You can also create you own network and make rFlea's work in their own.

rFlea belongs to INSBITS STUDIO framework (www.insbits.com). Insbits is a framework that aims to make quick prototyping possible,. With a compatible Android phone and our web app (reference to APK insbits web app) you can connect directly your rFlea to the phone, and program in Javascript all your projects, without the need of Andorid native or Arduino programming. We also support visual programming and with our web app you can directly connect rFlea to our Node-red (http://nodered.org/) and Spacebrew (http://docs.spacebrew.cc/) based open source Server (www.insbits.com:1880) or download it from us (https://github.com/MobileLifeCentre/rFlea-toolkit)

Installation
------------
Download the library, unzip it, change the name to "rFlea_Arduino" (Github adds "rFlea_Arduino-master", remove the "-master") and put it in the folder libraries from your Arduino IDE workspace folder. Remember to re-start Arduino IDE to see the changes.

Examples
------------
These libraries come with several examples on how to use the library. You can find them in File->Examples->rFlea_Arduino 

- rFlea_Android_TxOnly: this example sets up your rFlea to send the value of the digital inputs 3,10,11,12 and analog inputs A4 and A5 to your Android phone (refer to this coming soon  repo with the app). It shows how to use the rFlea profile SENSOR for such application.

- rFlea_Android_TxRx: this example sets up your rFlea to send the value of the digital inputs 3,10,11,12 and analog inputs A4 and A5 and, on the other hand, will receive data and set the LED depending on the value of the first data parameter (ON/OFF).

Step by Step Guide (Arduino standalone)
------------
1 Install Arudino libraries as indicated above. 

2 Restart the Arduino IDE to refresh the libraries.

3 Get your rFlea ready, together with an FTDI cable or board (https://www.sparkfun.com/products/9717).

4 Flash the example and run it with the  Serial Monitor from Arduino IDE, in the very first lines, you will read "S/N: xxxxxx" where the x's are a number, that number is the unique serial number of the rFlea you are using, S/N stands for Serial number. Write it down in case you want to pair it exclusively with your phone or other rFlea's.

5 Write you Arduino code as normal using the example of your chose as template.

	
