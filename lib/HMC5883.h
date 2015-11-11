#ifndef 
#define HMC5883_H


/**
 @brief Start simple setup routine
 @return none
 */
extern void HMC5883_setup();


/**
 @brief Read raw magnetometer data from HMC5883 and write measurements to specified locations
 @return none
 */	
extern void HMC5883_readRawData(int *xData, int *yData, int *zData);
	
	
//extern void configureCalibrationData();
#endif