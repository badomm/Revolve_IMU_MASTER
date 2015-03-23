/*
 * MPU6000.c
 *
 * Created: 2/23/2015 14:48:49
 *  Author: Jimmy Chau
 */ 

#include "MPU6000.h"
#include "MPU6000Registers.h"


//SPI command bytes
#define MPU6000_COMMAND_READ	0x80
#define MPU6000_COMMAND_WRITE	0x00




void mpu6000_writeRegister(uint8_t adress, uint8_t data, uint8_t chipselect){
	adress |= MPU6000_COMMAND_WRITE;
	//spi_tranceive(adress, NOT_LAST_TX);// specify register adress to the MPU
	//send data
	//spi_tranceive(data, LAST_TX);
	uint32_t adress_word = spi_word(false, chipselect, (uint16_t) adress);
	uint32_t data_word = spi_word(true, chipselect, (uint16_t) adress);
	uint32_t transmitbuffer[2] ={adress_word, data_word};
	uint8_t recieve_buffer = NULL;
	spi_freeRTOSTranceive(transmitbuffer, 2, NULL, NULL);

}

uint8_t mpu6000_readRegister(uint8_t adress){
	adress |= MPU6000_COMMAND_READ; //create spi message
	spi_tranceive(adress, NOT_LAST_TX);
	//prepare for return data
	return spi_tranceive(0x00, LAST_TX);
}

void mpu6000_burstReadRegister(uint8_t startAdress, uint8_t *buffer, uint8_t length){
	startAdress |= MPU6000_COMMAND_READ;
	spi_tranceive(startAdress, NOT_LAST_TX);
	for (int i = 0; i <length-1; i++)
	{
		buffer[i] = spi_tranceive(0x00, NOT_LAST_TX);
	}
	buffer[length - 1] = spi_tranceive(0x00, LAST_TX);
	
}

void mpu6000_setSensorRange(enum gyroRange_t gyro, enum accRange_t acc){
	uint8_t gyroSetting =	0b00<<3 ;
	uint8_t accSetting =	0b00<<3 ;
	
	switch(gyro){
		case gyroRange_250DPS:
			gyroSetting = 0b00<<3 ;
			break;
		case gyroRange_500DPS:
			gyroSetting = 0b01<<3;
			break;
		case gyroRange_1000DPS:
			gyroSetting = 0b10<<3;
			break;
		case gyroRange_2000DPS:
			gyroSetting = 0xb11<3;
			break;
		default:
			break;
		}
		mpu6000_writeRegister(MPUREG_GYRO_CONFIG, gyroSetting);
		
	switch(acc){
		case accRange_2g:
			accSetting = 0b00<<3;
			break;
		case accRange_4g:
			accSetting = 0b01<<3;
			break;
		case accRange_8g:
			accSetting = 0b10<<3;
			break;
		case accRange_16g:
			accSetting = 0xb11<3;
			break;
		default:
		break;
	}
	mpu6000_writeRegister(MPUREG_ACCEL_CONFIG, accSetting);
}

void mpu6000_setLowPassFilter(enum lowPassFilterBandWidth lpfBW){
	uint8_t lpfBW_configValue = 0;
	switch(lpfBW){
		case ACC260Hz_GYRO256Hz:
			lpfBW_configValue = 0;
			break;
		case ACC184Hz_GYRO188Hz:
			lpfBW_configValue = 1;
			break;
		case ACC94Hz_GYRO98HZ:
			lpfBW_configValue = 2;
			break;
		case ACC44HZ_GYRO42HZ:
			lpfBW_configValue = 4;
			
		case ACC21HZ_GYRO20HZ:
			lpfBW_configValue = 5;
			break;
		case ACC10HZ_GYRO10HZ:
			lpfBW_configValue = 6;
			break;
		case ACC5HZ_GYRO5HZ:
			lpfBW_configValue = 7;
			break;
	}
	
	mpu6000_writeRegister(MPUREG_CONFIG, lpfBW_configValue);
}

void mpu6000_setSampleRateDivider(uint8_t sampleRateDivider){
	mpu6000_writeRegister(MPUREG_SMPLRT_DIV, sampleRateDivider);
}	

void mpu6000_init(struct mpu6000Settings *settings){
	//turn off I2C
	mpu6000_writeRegister(MPUREG_USER_CTRL, BIT_USER_CTRL_I2C_IF_DIS);
	
	//reset device
	mpu6000_writeRegister(MPUREG_PWR_MGMT_1, BIT_PWR_MGMT_1_DEVICE_RESET);
	
	_delay_ms(100);
	
	//Reset all signals paths
	uint8_t signal_path_reset_all = BIT_SIGNAL_PATH_RESET_TEMP_RESET | BIT_SIGNAL_PATH_RESET_ACCEL_RESET| BIT_SIGNAL_PATH_RESET_GYRO_RESET;
	mpu6000_writeRegister(MPUREG_SIGNAL_PATH_RESET, signal_path_reset_all);
	_delay_ms(100);
	

	
	//set settings
	mpu6000_setSensorRange(settings->gyroRange, settings->accRange);
	mpu6000_setLowPassFilter(settings->lpfBw);
	mpu6000_setSampleRateDivider(settings->sampleRateDivider);
	
	//Turn off sleep, turn off cycle mode, set PLL with X axis gyroscope reference
	mpu6000_writeRegister(MPUREG_PWR_MGMT_1, 1);
}

void mpu6000_readMotionData(motion_data_t *motionData){
	uint8_t startAdress = MPUREG_ACCEL_XOUT_H;
	uint8_t length = 14;
	mpu6000_burstReadRegister(startAdress, motionData->array, length);
}

typedef union{
	uint16_t ui16;
	uint8_t array[2];
	}singleMotionData_t;

uint16_t mpu_readSingleMotionData(enum motionChannel channel){
	singleMotionData_t data;
	
	uint8_t startAddress = 0;
	switch(channel){
		case ACC_X:
			startAddress = MPUREG_ACCEL_XOUT_H;
			break;
		case ACC_Y:
			startAddress = MPUREG_ACCEL_YOUT_H;
			break;
		case ACC_Z:
			startAddress = MPUREG_ACCEL_ZOUT_H;
			break;
		case TEMP:
			startAddress = MPUREG_TEMP_OUT_H;
			break;
		case GYRO_X:
			startAddress = MPUREG_GYRO_XOUT_H;
			break;
		case GYRO_Y:
			startAddress = MPUREG_GYRO_XOUT_H;
			break;
		case GYRO_Z:
			startAddress = MPUREG_GYRO_XOUT_H;
			break;
	}
	mpu6000_burstReadRegister(startAddress, data.array, 2);
	return data.ui16;
}

