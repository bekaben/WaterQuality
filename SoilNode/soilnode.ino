#include <Wire.h>
#include "dht.h"

DHT dht(1, DHT11);

void setup() {
  Serial.begin(115200);
  
  dht.begin();

  Wire.begin(18);                
  Wire.onReceive(receiveCmd);      
  Wire.onRequest(requestMeasure);  
  Serial.println("I am the soil moisture node");
}

String cmd;
float measure;
char MeasureBuff[7];

void receiveCmd() {
 Serial.println("receiving command");
 cmd = "";
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    cmd+= c;           /* print the command */
 }
    Serial.println(cmd);  
    
    if(cmd=="temp"){
      measure = dht.readTemperature();
    }else if(cmd=="hum"){
      measure = dht.readHumidity();
    }else if(cmd=="soil"){
      measure = analogRead(0);
      Serial.print("Soil Moisture : ");
      Serial.println(measure);
    }else{
      measure = -1 ;
    }
    
  }

void requestMeasure() {
  Serial.println("Sending measure");
  dtostrf(measure,7,2, MeasureBuff);
  Serial.println(MeasureBuff);
  Wire.write(MeasureBuff);
}
void loop() {
  dht.read();
  delay(5000);
}
