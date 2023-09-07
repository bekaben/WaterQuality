#include <Wire.h>
#include "SparkFun_SinglePairEthernet.h"


    SinglePairEthernet adin1110;

    byte thisDeviceMAC[6] = {0x00, 0xE0, 0x22, 0xFE, 0xDA, 0xC9};
    byte destinationMAC[6] = {0x00, 0xE0, 0x22, 0xFE, 0xDA, 0xCA};
    const int MAX_MSG_SIZE = 200;
    char message[MAX_MSG_SIZE];
    
static void rxCallback(byte * data, int dataLen, byte * senderMac)
{
    Serial.print("Recieved:\t");
    Serial.println((char *)data); 
    if(!adin1110.indenticalMacs(senderMac, destinationMAC))
    {
        Serial.print("From an unknown source: ");
        for(int i = 0; i < 6; i++)
        {
          Serial.print(senderMac[i]);
          Serial.print(" ");    
        }
        
    }
    Serial.println();
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); 
  while(!Serial);
  Serial.println("I am SPE Water Node");
  Wire.begin(); /* join i2c bus as master */
  
  
  
  if (!adin1110.begin(thisDeviceMAC)) 
    {
      Serial.print("Failed to setup ADIN1110 MACPHY. Make sure board is connected and pins are defined for target.");
      while(adin1110.begin(thisDeviceMAC)) Serial.print("MACPHY Configuration in progress"); //If we can't connect just stop here      
    }
    Serial.println("MACPHY Setup correctly"); 
    /* Set up callback, to control what we do when data is recieved */
    adin1110.setRxCallback(rxCallback);

    //while (adin1110.getLinkStatus() != true) Serial.println("Waiting for link");
    
}

void sendCommand(int nodeADR, char cmd[]){
  Wire.beginTransmission(nodeADR); 
  Serial.print("Sending command : ");
  Serial.println(cmd);
  Wire.write(cmd);  /* sends hello string */
  Wire.endTransmission();    /* stop transmitting */
}


float Measure;
String StrMeasure;
void getMeasure(int nodeADR, int datasize){
  StrMeasure = "";
  Wire.requestFrom(nodeADR, datasize); /* request & read data of size 9 from slave */
  while(Wire.available()){
    char c = Wire.read();/* read data received from slave */
    StrMeasure += c;
 }
 Measure = StrMeasure.toFloat();
}

void sendoverSPE(char* msg){
  while(!adin1110.getLinkStatus());
  Serial.print("Sending:\t");
  Serial.println(*msg); //This is ok since we know they are all null terminated strings 
  adin1110.sendData((byte *)msg, sizeof(msg), destinationMAC);
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}



void loop() {
  sendCommand(17, "temp");
  delay(1500);
  getMeasure(17, 7);  
  StrMeasure = "Water Temperature: "+StrMeasure;
  Serial.println(StrMeasure);
  StrMeasure.toCharArray(message, StrMeasure.length()+1);
  sendoverSPE(message);

  sendCommand(17, "cond");
  delay(4000);
  getMeasure(17, 7);
  StrMeasure = "Water Conductivity: "+StrMeasure;
  Serial.println(StrMeasure);
  StrMeasure.toCharArray(message, StrMeasure.length()+1);
  sendoverSPE(message);

  sendCommand(17, "tds");
  delay(4000);
  getMeasure(17, 7);

  StrMeasure = "Water Total Disolved Solids: "+StrMeasure;
  Serial.println(StrMeasure);
  StrMeasure.toCharArray(message, StrMeasure.length()+1);
  sendoverSPE(message);

  sendCommand(18, "soil");
  delay(1500);
  getMeasure(18, 7);

  StrMeasure = "Soil Moisture: "+StrMeasure;
  Serial.println(StrMeasure);
  StrMeasure.toCharArray(message, StrMeasure.length()+1);
  sendoverSPE(message);

  sendCommand(18, "temp");
  delay(1500);
  getMeasure(18, 7);

  StrMeasure = "Air Temperature: "+StrMeasure;
  Serial.println(StrMeasure);
  StrMeasure.toCharArray(message, StrMeasure.length()+1);
  sendoverSPE(message);

  sendCommand(18, "hum");
  delay(1500);
  getMeasure(18, 7);
  
  StrMeasure = "Air Humidity: "+StrMeasure;
  Serial.println(StrMeasure);
  StrMeasure.toCharArray(message, StrMeasure.length()+1);
  sendoverSPE(message); 
  
}