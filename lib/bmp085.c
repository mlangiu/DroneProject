
#ifndef F_CPU
#define F_CPU 4000000UL
#warning "Subsitute F_CPU defined in " ## __FILE__
#endif


#include "twimaster.h"
#include <util/delay.h>

#define BMP085WRITEADRESS 0b11101110
//#define BMP085READADDRESS 0b11101111
#define CONTROLREGISTER 0xF4
#define DATAREGISTERMSB 0xF6
#define DATAREGISTERLSB 0xF7
#define DATAREGISTERXSB 0xF8
#define MEASURETEMPERATURE 0x2E
#define MEASUREPRESSURE0 0x34
#define MEASUREPRESSURE1 0x74
#define MEASUREPRESSURE2 0xB4
#define MEASUREPRESSURE3 0xF4
#define MEASTEMPMAXTIMEMS 5
#define MEASPRES0MAXTIMEMS 5
#define MEASPRES1MAXTIMEMS 8
#define MEASPRES2MAXTIMEMS 14
#define MEASPRES3MAXTIMEMS 26
#define AC1Register 0xAA
#define AC2Register 0xAC
#define AC3Register 0xAE
#define AC4Register 0xB0
#define AC5Register 0xB2
#define AC6Register 0xB4
#define B1Register 0xB6
#define B2Register 0xB8
#define MBRegister 0xBA
#define MCRegister 0xBC
#define MDRegister 0xBE

int16_t AC1 = 0;
int16_t AC2 = 0;
int16_t AC3 = 0;
uint16_t AC4 = 0;
uint16_t AC5 = 0;
uint16_t AC6 = 0;
int16_t B1 = 0;
int16_t B2 = 0;
int16_t MB = 0;
int32_t MC = 0;
int16_t MD = 0;
int32_t B5 = 0;




/*************************************************************************
 Read calibration data from BMP085
*************************************************************************/
void bmp085_readCalibrationData(void){
	char calibrationData[22] = {0};	
	i2c_readMultipleByte(BMP085WRITEADRESS, 0xAA, 22, calibrationData);
	AC1 = (*calibrationData << 8) + *(calibrationData+1);
	AC2 = (*(calibrationData+2) << 8) + *(calibrationData+3);
	AC3 = (*(calibrationData+4) << 8) + *(calibrationData+5);
	AC4 = (*(calibrationData+6) << 8) + *(calibrationData+7);
	AC5 = (*(calibrationData+8) << 8) + *(calibrationData+9);
	AC6 = (*(calibrationData+10) << 8) + *(calibrationData+11);
	B1 = (*(calibrationData+12) << 8) + *(calibrationData+13);
	B2 = (*(calibrationData+14) << 8) + *(calibrationData+15);
	MB = (*(calibrationData+16) << 8) + *(calibrationData+17);
	MC = ((*(calibrationData+18) << 8) + *(calibrationData+19)) << 11;
	MD = (*(calibrationData+20) << 8) + *(calibrationData+21);
	return;
}


/*************************************************************************
 Read raw temperature value from BMP085
 
 Return:  Raw temperature value
*************************************************************************/
int32_t bmp085_readTempRaw(void){
	int32_t measuredTemperatureRaw = 0;
	char tmp[2] = {0};
	i2c_writeByte (BMP085WRITEADRESS, CONTROLREGISTER, MEASURETEMPERATURE);	//ask for temperature measurement
	_delay_ms(MEASTEMPMAXTIMEMS); //wait for measurement to be finished
	i2c_readMultipleByte(BMP085WRITEADRESS, DATAREGISTERMSB, 2, tmp); //read data bytes from BMP085
	measuredTemperatureRaw = (((uint32_t) tmp[0]) << 8) + tmp[1]; //combine data bytes according to the data sheet
	return measuredTemperatureRaw;
	}/* bmp085_readTempRaw */


/*************************************************************************
 Read raw pressure value from BMP085
 
 Input:   Oversampling code (0-3)
 
 Return:  Raw pressure value
*************************************************************************/
int32_t bmp085_readPressRaw(char oversampling){
	int32_t measuredPressureRaw = 0;
	char tmp[3] = {0};
	switch (oversampling)
	{
	case 0:{
	i2c_writeByte (BMP085WRITEADRESS, CONTROLREGISTER, MEASUREPRESSURE0); //ask for pressure measurement, oversampling 0
	_delay_ms(MEASPRES0MAXTIMEMS);	//wait for measurement to be finished
	break;}
	case 1:{
	i2c_writeByte (BMP085WRITEADRESS, CONTROLREGISTER, MEASUREPRESSURE1); //ask for pressure measurement, oversampling 1
	_delay_ms(MEASPRES1MAXTIMEMS); //wait for measurement to be finished
	break;}
	case 2:{
	i2c_writeByte (BMP085WRITEADRESS, CONTROLREGISTER, MEASUREPRESSURE2); //ask for pressure measurement, oversampling 2
	_delay_ms(MEASPRES2MAXTIMEMS); //wait for measurement to be finished
	break;}
	case 3: {
	i2c_writeByte (BMP085WRITEADRESS, CONTROLREGISTER, MEASUREPRESSURE3); //ask for pressure measurement, oversampling 3
	_delay_ms(MEASPRES3MAXTIMEMS); //wait for measurement to be finished
	break;}
	default: return 0;
	}
	i2c_readMultipleByte(BMP085WRITEADRESS, DATAREGISTERMSB, 3, tmp);	//read data bytes from BMP085
	measuredPressureRaw = ((((((uint32_t) tmp[0]) << 8) + tmp[1])  << 8) + tmp[2]) >> (8-oversampling); //combine data bytes according to the data sheet
	return measuredPressureRaw;	
}/* bmp085_readPressRaw */



/*************************************************************************
 Read corrected temperature value from BMP085
 
 Return:  Corrected temperature value
*************************************************************************/
int bmp085_readTemp(void){
	volatile int32_t rawTemp = bmp085_readTempRaw();
	long X1 = ((rawTemp - AC6) * AC5) >> 15;
	volatile int32_t X2 = MC / (X1 + MD);
	B5 = X1 + X2;
	int temperature = (int)((B5 + 8) >> 4);
	return temperature;
}



/*************************************************************************
 Read corrected pressure value from BMP085
 
 Input:   Oversampling code (0-3)
 
 Return:  Corrected pressure value
*************************************************************************/
long bmp085_readPress(char oversampling, char newTempRead){
	volatile long pressure = 0;
	volatile long rawPressure = bmp085_readPressRaw(oversampling);
	if (newTempRead==1){bmp085_readTemp();}
	volatile long B6 = B5 - 4000;
	volatile long X1 = (B2 * B6 * (B6 >> 12)) >> 11;
	volatile long X2 = (AC2 * B6) >> 11;
	volatile long X3 = X1 + X2;
	volatile long B3 = (((AC1 * 4 + X3) << oversampling) + 2)/4;
	X1 = AC3 * B6 >> 13;
	X2 = (B1 * B6 * (B6 >> 12)) >> 16;
	X3 = (X1 + X2 +2) >> 2;
	volatile unsigned long B4 = AC4 * (unsigned long) (X3 + 32768) >> 15;
	volatile unsigned long B7 = (unsigned long) (rawPressure - B3) * (50000 >> oversampling);
	if (B7 < 0x8){pressure = (B7 * 2)/B4;}
	else{pressure = (B7/B4)*2;}
	X1 = (pressure >> 8) * (pressure >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * pressure) >> 16;
	pressure = pressure + ((X1 + X2 + 3791) >> 4);
	return pressure;
}



/*************************************************************************
Calculate height above sea level in cetimeters from measured pressure value using the barometric formula
 
 Input:   Oversampling code (0-3), request new temperature measurement (0/1), (pressure at sea level in Pascal as precompiler command)
 
 Return:  Calculated height above sea level in centimeters
*************************************************************************/
long bmp085_calcHeigthCentimeter(char oversampling, char newTempRead){
	#define PRESSUREATSEALEVEL 106646 //101325
	return (long)(4433000*(1-pow((double)bmp085_readPress(oversampling, newTempRead)/PRESSUREATSEALEVEL, 0.19029496)));
}