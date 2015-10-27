#ifndef BMP085_H
#define BMP085_H

/**
 @brief Read calibration data from BMP085
 */
extern void bmp085_readCalibrationData(void);


/**
 @brief Read raw temperature bytes from BMP085 and combine them to variable measuredTemperatureRaw
 @return Raw temperature value
 */
extern int32_t bmp085_readTempRaw(void);


/**
 @brief Read raw pressure bytes from BMP085 and combine them to variable measuredPressureRaw
 @input Oversampling code (0-3)
 @return Raw pressure value
 */
extern int32_t bmp085_readPressRaw(char oversampling);


/**
 @brief Read corrected temperature value from BMP085
 @return Corrected temperature value
 */
extern int bmp085_readTemp(void);


/**
 @brief Read corrected pressure value from BMP085
 @return Corrected pressure value
 */
extern long bmp085_readPress(char oversampling, char newTempRead);


/**
 @brief Calculate height above sea level in cetimeters from measured pressure value using the barometric formula
 @return Calculated height above sea level in centimeters
 */
extern long bmp085_calcHeigthCentimeter(char oversampling, char newTempRead);

#endif // BMP085_H

