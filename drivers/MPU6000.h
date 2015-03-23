/*
 * MPU6000.h
 *
 * Created: 23.02.2014 22:00:09
 *  Author: Simen
 */ 


#ifndef MPU6000_H_
#define MPU6000_H_

#include "MPU6000Registers.h"
//GYRO_RANGE, degree per second
enum gyroRange_t{	gyroRange_250DPS = 0, 
				gyroRange_500DPS = 1, 
				gyroRange_1000DPS = 2,
				gyroRange_2000DPS = 3};
//ACC RANGE, g
enum accRange_t{	accRange_2g = 0, 
				accRange_4g = 1, 
				accRange_8g = 2, 
				accRange_16g = 3};
enum motionChannel{	ACC_X, 
					ACC_Y, 
					ACC_Z, 
					TEMP, 
					GYRO_X, 
					GYRO_Y, 
					GYRO_Z
				};
enum lowPassFilterBandWidth{	
				ACC260Hz_GYRO256Hz,
				ACC184Hz_GYRO188Hz,
				ACC94Hz_GYRO98HZ,
				ACC44HZ_GYRO42HZ,
				ACC21HZ_GYRO20HZ,
				ACC10HZ_GYRO10HZ,
				ACC5HZ_GYRO5HZ
	};

typedef union{
	struct{
		uint8_t x_accel_h;
		uint8_t x_accel_l;
		uint8_t y_accel_h;
		uint8_t y_accel_l;
		uint8_t z_accel_h;
		uint8_t z_accel_l;
		uint8_t t_h;
		uint8_t t_l;
		uint8_t x_gyro_h;
		uint8_t x_gyro_l;
		uint8_t y_gyro_h;
		uint8_t y_gyro_l;
		uint8_t z_gyro_h;
		uint8_t z_gyro_l;
	} reg;
	struct{
		int16_t x_accel;
		int16_t y_accel;
		int16_t z_accel;
		int16_t temperature;
		int16_t x_gyro;
		int16_t y_gyro;
		int16_t z_gyro;
	} value;
	uint8_t array[14];
}motion_data_t;

struct mpu6000Settings{
	enum gyroRange_t gyroRange;
	enum accRange_t accRange;
	uint8_t sampleRateDivider;
	enum lowPassFilterBandWidth lpfBw;
	};

// MPU6000 accelerometer scaling
#define MPU6000_ACCEL_SCALE_1G					(GRAVITY / 4096.0)
uint8_t mpu6000_readRegister(uint8_t adress);
void mpu6000_writeRegister(uint8_t adress, uint8_t data);
void mpu6000_burstReadRegister(uint8_t startAdress, uint8_t *buffer, uint8_t length);
void mpu6000_init(struct mpu6000Settings *settings);

#endif /* MPU6000_H_ */