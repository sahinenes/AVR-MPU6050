/*
 * mpu6050.h
 *
 * Created: 23.11.2021 13:56:58
 * Abdulkadir and Enes
 */ 


#ifndef MPU6050_H_
#define MPU6050_H_



#include "I2C_Master_H_file.h"
#include "MPU6050_res_define.h"
#include "atmega8xx.h"

void Gyro_Init();
void MPU_Start_Loc();
void Read_RawValue();

#endif /* MPU6050_H_ */