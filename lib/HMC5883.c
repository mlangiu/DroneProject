#define HMC5883_ADDRESS 0x3C
#define HMC5883_CONFIG_A 0
#define HMC5883_CONFIG_B 1
#define HMC5883_MODE 2
#define HMC5883_XDATA_H 3	//Data registers are read-only registers
#define HMC5883_XDATA_L 4
#define HMC5883_YDATA_H 5
#define HMC5883_YDATA_L 6
#define HMC5883_ZDATA_H 7
#define HMC5883_ZDATA_L 8
#define HMC5883_STATUS 9	//0: Data ready bit, set when data is written to all six measurement registers, DRDY pin can be used as alternative; 
							//1: Lock bit indicates lock state, when not all data bytes are read; 
							//2: Regulator enabled bit is set when internal voltage regulator is enabled
#define HMC5883_IDENTIFICATION_A 10 //Identification registers are read-only registers to identify the device
#define HMC5883_IDENTIFICATION_B 11
#define HMC5883_IDENTIFICATION_C 12

void HMC5883_setup(){
/*
	DO2	|	DO1	|	DO0	|	Typical Data Output Rate (Hz)
	========================================================
	0	|	0	|	0	|	0.75
	0	|	0	|	1	|	1.5
	0	|	1	|	0	|	3
	0	|	1	|	1	|	7.5
	1	|	0	|	0	|	15 (default)
	1	|	0	|	1	|	30
	1	|	1	|	0	|	75
	1	|	1	|	1	|	Not used*/
	#define DO2 4	//DO2-0 can be used to select the output rate for continuous read mode, 116 Hz can be achieved in single read mode with use of interrupts
	#define DO1 3
	#define DO0 2
	#define MS1 1	//00 for normal measurement mode (default), 01 for positive bias configuration, 10 for negative bias configuration
	#define MS0 0
	i2c_writeByte (HMC5883_ADDRESS, HMC5883_CONFIG_A, 1 << DO2 | 0 << DO1 | 0 << DO0 | 0 << MS1 | 0 << MS0);
	
	#define GN2 7	//Used to configure the device gain
	#define GN1 6
	#define GN0 5
	i2c_writeByte (HMC5883_ADDRESS, HMC5883_CONFIG_B, 0 << GN2 | 0 << GN1 | 1 << GN0);
	
	#define MD1 1	//Used to control the operation mode: 00 for continuous measuring mode - RDY goes high when data is available, 01 for single measurement mode - device stays in sleep mode until measurement in read, 10 for idle mode, 11 for sleep mode 
	#define MD0 0
	i2c_writeByte (HMC5883_ADDRESS, HMC5883_MODE, 0 << MD1 | 0 << MD0);
{
	
	
void HMC5883_readRawData(int *xData, int *yData, int *zData){ //burst mode nutzen! Seite 11 Datenblatt
	int data[6] = {0};
	i2c_readMultipleByte(HMC5883_ADDRESS, HMC5883_XDATA_H, 6, &data);
	*xData = (data[0] << 8) | data[1];
	*yData = (data[2] << 8) | data[3];
	*zData = (data[4] << 8) | data[5];
{
	
	
//void configureCalibrationData(){}