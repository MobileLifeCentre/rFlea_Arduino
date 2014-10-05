#include "Arduino.h"
#include "rFlea_Arduino.h"
#include "antdefines.h"
#include "antmessage.h"
#include <ANT.h>
#include <EEPROM.h>

rFlea_Arduino::rFlea_Arduino()
{  
  for (int i=0;i<MAX_NUM_CHANNEL;i++){
	mCH[i].channelStatus=STATUS_UNASSIGNED_CHANNEL;
  }
  onMessageCallback_ACTIVE=false;
  onMessageSensorCallback_ACTIVE=false;
  onMessageSensorRxCallback_ACTIVE=false;
  onMessageBeaconListenerCallback_ACTIVE=false;
  onRssi_ACTIVE=false;
  onSyncCallback_ACTIVE=false;
  
}

void rFlea_Arduino::rFlea_profile(byte rFlea_profile){
	if (rFlea_profile==SENSOR){
  	  mCH[rFlea_profile].channelNR=rFlea_profile;
  	  mCH[rFlea_profile].channelType=CHANNEL_TYPE_MASTER_TX_ONLY;
  	  mCH[rFlea_profile].networkID=0x00;
  	  mCH[rFlea_profile].deviceID=getSerialNumber();
  	  mCH[rFlea_profile].deviceType=0x27;
  	  mCH[rFlea_profile].txType=0x01;
  	  mCH[rFlea_profile].channelFreq=DEFAULT_RADIO_CHANNEL;
  	  mCH[rFlea_profile].channelPeriod=4096;
  	  mCH[rFlea_profile].channelStatus=STATUS_ASSIGNED_CHANNEL;
  	}else if (rFlea_profile==SENSOR_RX){
  	  mCH[rFlea_profile].channelNR=rFlea_profile;
  	  mCH[rFlea_profile].channelType=CHANNEL_TYPE_MASTER;
  	  mCH[rFlea_profile].networkID=0x00;
  	  mCH[rFlea_profile].deviceID=getSerialNumber();
  	  mCH[rFlea_profile].deviceType=0x27;
  	  mCH[rFlea_profile].txType=0x01;
  	  mCH[rFlea_profile].channelFreq=DEFAULT_RADIO_CHANNEL;
  	  mCH[rFlea_profile].channelPeriod=4096;
  	  mCH[rFlea_profile].channelStatus=STATUS_ASSIGNED_CHANNEL;
  	}else if (rFlea_profile==BEACON_LISTENER){
  	  mCH[rFlea_profile].channelNR=rFlea_profile;
  	  mCH[rFlea_profile].channelType=CHANNEL_TYPE_SLAVE;
  	  mCH[rFlea_profile].networkID=0x00;
  	  mCH[rFlea_profile].deviceID=0x00; //wild card as default
  	  mCH[rFlea_profile].deviceType=0x27; //wild card as default
  	  mCH[rFlea_profile].txType=0x01;
  	  mCH[rFlea_profile].channelFreq=DEFAULT_RADIO_CHANNEL;
  	  mCH[rFlea_profile].channelPeriod=4096;
  	  mCH[rFlea_profile].channelStatus=STATUS_ASSIGNED_CHANNEL;
  	}
}

void rFlea_Arduino::set_beacon_serial_number(int beacon_serial){
	mCH[BEACON_LISTENER].deviceID=beacon_serial;
}
void rFlea_Arduino::set_period(byte rFlea_profile,byte _period){
	if (_period==0x00){
		mCH[rFlea_profile].channelPeriod=65536;
	}else{
		mCH[rFlea_profile].channelPeriod=32768/_period;
	}
}

void rFlea_Arduino::set_RF_frequency(byte rFlea_profile,byte _freq){
	mCH[rFlea_profile].channelFreq=_freq;
}

void rFlea_Arduino::enable_rssi(){
ant.libConfig(0x40); //enable extend Msg with 0x20 => Rx Timestamp Output; 0x40 -> RSSI Output;  0x80 -> Channel ID Output
}


void rFlea_Arduino::init(){
  ant.initANT();
  
  ant.enableExtClock(); //low power
  
  //loop that initialize only the assigned channels
  for (int i=0;i<MAX_NUM_CHANNEL;i++){
  	if (mCH[i].channelStatus==STATUS_ASSIGNED_CHANNEL){
  			ant.config(mCH[i],200); //configure channel
  		}
  }
}

int rFlea_Arduino::my_serial_number(){
	return(SerialNumber);
}

void rFlea_Arduino::connect(byte rFlea_profile){
  	if (mCH[rFlea_profile].channelStatus==STATUS_ASSIGNED_CHANNEL){
  			ant.opench(mCH[rFlea_profile]); //open channel
  			mCH[rFlea_profile].channelStatus=STATUS_TRACKING_CHANNEL;
  		}
}

void rFlea_Arduino::disconnect(byte rFlea_profile){
  	if (mCH[rFlea_profile].channelStatus==STATUS_TRACKING_CHANNEL){
  			  ant.closech(mCH[rFlea_profile]);
  			mCH[rFlea_profile].channelStatus=STATUS_ASSIGNED_CHANNEL;
  		}
}

void rFlea_Arduino::disconnect_all(){
   for (int i=0;i<MAX_NUM_CHANNEL;i++){
   if (mCH[i].channelStatus==STATUS_TRACKING_CHANNEL){
  			  ant.closech(mCH[i]);
  			mCH[i].channelStatus=STATUS_ASSIGNED_CHANNEL;
  		}
   }
}

void rFlea_Arduino::update(){
  parseData(ant.update());
}

void rFlea_Arduino::parseData(byte* message){
  switch (message[MESG_ID_OFFSET]){
  case MESG_BROADCAST_DATA_ID:
    {
    	
    	uint8_t rxbuff[8];
    	byte rFlea_profile;
    	
    	rFlea_profile=message[3];
    	
    	rxbuff[0] = message[4];
    	rxbuff[1] = message[5];
    	rxbuff[2] = message[6];
    	rxbuff[3] = message[7];
    	rxbuff[4] = message[8];
    	rxbuff[5] = message[9];
    	rxbuff[6] = message[10];
    	rxbuff[7] = message[11];
    	
    	if (onRssi_ACTIVE) onRssiCallback(message[15]);
    	if (onMessageCallback_ACTIVE) onMessageCallback(rFlea_profile, rxbuff);
		if (rFlea_profile==SENSOR){
			if (onMessageSensorCallback_ACTIVE) onMessageSensorCallback(rxbuff);
		}else if (rFlea_profile==SENSOR_RX){
			if (onMessageSensorRxCallback_ACTIVE) onMessageSensorRxCallback(rxbuff);
		}else if (rFlea_profile==BEACON_LISTENER){
			if (onMessageBeaconListenerCallback_ACTIVE) onMessageBeaconListenerCallback(rxbuff);
		}
      break;
    }
  case MESG_ACKNOWLEDGED_DATA_ID:
    {

      break;
    }
  case MESG_RESPONSE_EVENT_ID:
    {
    //Channel Response / Event
      switch (message[5]){
      case RESPONSE_NO_ERROR: // EVENT_TX
        { 

          break; 
        }
        case EVENT_RX_SEARCH_TIMEOUT: // EVENT_TX
        { 




          break; 
        }
        case EVENT_RX_FAIL: // EVENT_TX
        { 




          break; 
        }
      case EVENT_TX: // EVENT_TX
        {
			onSyncCallback();
          break; 
        }  
	  case EVENT_TRANSFER_RX_FAILED: // EVENT_TX
        { 





          break; 
        }
              case EVENT_TRANSFER_TX_COMPLETED: // EVENT_TX
        { 





          break; 
        }
              case EVENT_TRANSFER_TX_FAILED: // EVENT_TX
        { 





          break; 
        }
              case EVENT_CHANNEL_CLOSED: // EVENT_TX
        { 






          break; 
        }
              case EVENT_RX_FAIL_GO_TO_SEARCH: // EVENT_TX
        { 





          break; 
        }    
      default:
        {
            ///channelEventResponseCallback(message,messageSize);
        }

      }

      break;
    }
    default:{
            //////unknownResponseCallback(message,messageSize);    
    }		
  }	
}

void rFlea_Arduino::register_onMessage(void (*callback_funct)(byte rFlea_profile, byte * message)){
  onMessageCallback_ACTIVE=true;
  onMessageCallback=callback_funct;
}

void rFlea_Arduino::register_onMessageSensor(void (*callback_funct)(byte * message)){
  onMessageSensorCallback_ACTIVE=true;
  onMessageSensorCallback=callback_funct;
}
void rFlea_Arduino::register_onMessageSensorRx(void (*callback_funct)(byte * message)){
  onMessageSensorRxCallback_ACTIVE=true;
  onMessageSensorRxCallback=callback_funct;
}
void rFlea_Arduino::register_onMessageBeaconListener(void (*callback_funct)(byte * message)){
  onMessageBeaconListenerCallback_ACTIVE=true;
  onMessageBeaconListenerCallback=callback_funct;
}
void rFlea_Arduino::register_onRssi(void (*callback_funct)(byte rssi)){
  onRssi_ACTIVE=true;
  onRssiCallback=callback_funct;
}
void rFlea_Arduino::register_onSync(void (*callback_funct)()){
  onSyncCallback_ACTIVE=true;
  onSyncCallback=callback_funct;
}
  
void rFlea_Arduino::send(byte rFlea_profile,byte* message){
	 if (mCH[rFlea_profile].channelStatus==STATUS_TRACKING_CHANNEL){
  			  ant.sendData(mCH[rFlea_profile], message);
  		}
}

int rFlea_Arduino::getSerialNumber(){

 SerialNumber =  ((EEPROM.read(0x00)<<8) & 0xFF00) + EEPROM.read(0x01); 
  if(SerialNumber == 0xFFFF){ //no SerialNumber 

    Serial.println("No Serial found .. flashing new one:");   
    randomSeed(analogRead(A2));

    EEPROM.write(0, random(255)); //Serial Number MSB
    EEPROM.write(1, random(255)); //Serial Number LSB
    EEPROM.write(2, 0x54); //Board Type ... 54 = Stiva
    EEPROM.write(3, 0x01); //Version Number  01 = 0.1 
  }

  SerialNumber =  ((EEPROM.read(0x00)<<8) & 0xFF00) + EEPROM.read(0x01); 


return SerialNumber;
}