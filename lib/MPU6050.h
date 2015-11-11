#ifndef 
#define MPU_6050_H

/**
 @brief Start simple setup routine
 @return none
 */
extern void mpu6050_setup();


/**
 @brief Read raw accelerometer data from MPU6050 and write measurements to specified location
 @return none
 */
extern void mpu6050_readAccelerometerDataRaw(int *xData, int *yData, int *zData);


/**
 @brief Read raw gyroscope data from MPU6050 and write measurements to specified location
 @return none
 */
extern void mpu6050_readGyroDataRaw(int *xData, int *yData, int *zData);


/**
 @brief Read raw temperature data from MPU6050
 @return Temperature data integer
 */
extern int mpu6050_readTempDataRaw(void);
#endif