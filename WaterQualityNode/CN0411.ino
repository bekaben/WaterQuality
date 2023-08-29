/***************************************************************************//**
 *   @file   CN0411.ino
 *   @brief  CN0411 With Arduino Uno From CN0411_example.
 *   @author Belkacem BENADDA 2023
*******************************************************************************/

#include <Wire.h>
#include "CN0411.h"
#include "Timer.h"
#include "Communication.h"

struct cn0411_init_params cn0411_init_params= {
	CH_GAIN_RES_20M,
	ADC_SINGLE_CONV,
	RTD_RES_100,
	{
		GAIN_RES_20,
		GAIN_RES_200,
		GAIN_RES_2K,
		GAIN_RES_20K,
		GAIN_RES_200K,
		GAIN_RES_2M,
		GAIN_RES_20M
	},
	OFFSET_RES_INIT,
	DAC_OUT_DEFAULT_VAL,
	EXC_DEFAULT_VAL,
	VR20S_DEFAULT_VAL,
	VR200S_DEFAULT_VAL,
	RDRES_DEFAULT_VAL,
	EXC_DEFAULT_VAL,
	CELL_CONST_NORMAL,
	TEMP_DEFAULT_VAL,
	VPP_DEFAULT_VAL,
	VINP_DEFAULT_VAL,
	VINN_DEFAULT_VAL,
	COND_DEFAULT_VAL,
	COMP_COND_DEFAULT_VAL,
	TDS_DEFAULT_VAL,
	{
		TEMP_COEFF_NACL,
		TDS_NACL
	}
};

struct cn0411_device cn0411_dev;
String cmd = "";
float measure = -1.0;
char MeasureBuff[7];
bool measuredone= false;

void setup()
{
	uint32_t ret;

	timer_start(); /* Start the System Tick Timer. */

  Serial.begin(115200);
	/* Initialize SPI */
	SPI_Init();
	if (ret == CN0411_FAILURE)
		return;

	/* Initialize CN0411 */
	ret = CN0411_init(&cn0411_dev, cn0411_init_params);

	if (ret == CN0411_FAILURE) {
		Serial.print(F("CN0411 Initialization error!\n"));
		return;
	}

  Wire.begin(17);                /* join i2c bus with address 0x11 */
  Wire.onReceive(receiveCmd);      /* register receive command event */
  Wire.onRequest(requestMeasure);  /* register send measure */
}

void receiveCmd() {
 Serial.println("receiving command");
 cmd = "";
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    cmd+= c;           /* print the command */
 }
    Serial.println(cmd);  
    measuredone = true;
    if(cmd=="temp"){
      CN0411_cmd_temp(0,&cn0411_dev);
      measure = cn0411_dev.temp;
    }else if(cmd=="cond"){
      CN0411_cmd_cond(0,&cn0411_dev);
      measure = cn0411_dev.cond;
    }else if(cmd=="tds"){
      CN0411_cmd_tds(0,&cn0411_dev);
      measure = cn0411_dev.tds;
    }else{
      measure = -1 ;
      measuredone = false;
    }
    
  }
 

void requestMeasure() {
  if(measuredone) {
  Serial.println("Sending Measure");
  dtostrf(measure,7,2, MeasureBuff);
  Serial.println(MeasureBuff);
  Wire.write(MeasureBuff);
  measuredone = false;
  }else{
        Serial.println("Waiting for measure");
        Wire.write("Error");
  }
}
void loop()
{
	//if( Serial.available())     CN0411_cmd_process(&cn0411_dev); 
  
}
