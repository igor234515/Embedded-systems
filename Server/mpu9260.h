/*
 * mpu6050.h
 *
 *  Created on: Nov 13, 2019
 *      Author: Bulanov Konstantin
 */

#ifndef INC_GY521_H_
#define INC_GY521_H_

#endif /* INC_GY521_H_ */

#include <stdint.h>
#include "i2c.h"

// MPU9250 structure
typedef struct
{

    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;
    double Gx;
    double Gy;
    double Gz;

    float Temperature;

    double magnetX;
    double magnetY;
    double magnetZ;

    double KalmanAngleX;
    double KalmanAngleY;
} MPU9250_t;

// Kalman structure
typedef struct
{
    double Q_angle;
    double Q_bias;
    double R_measure;
    double angle;
    double bias;
    double P[2][2];
} Kalman_t;

uint8_t MPU9250_Init(I2C_HandleTypeDef *I2Cx);

uint8_t AK8963_Init(I2C_HandleTypeDef *I2Cx);//MAGNETOMETER INIT

void MPU9250_Read_Accel(I2C_HandleTypeDef *I2Cx, MPU9250_t *DataStruct);

void MPU9250_Read_Gyro(I2C_HandleTypeDef *I2Cx, MPU9250_t *DataStruct);

void MPU9250_Read_Temp(I2C_HandleTypeDef *I2Cx, MPU9250_t *DataStruct);

void MPU9250_Read_All(I2C_HandleTypeDef *I2Cx, MPU9250_t *DataStruct);

void AK8963_Read_Magnet(I2C_HandleTypeDef *I2Cx, MPU9250_t *DataStruct);//MAGNETOMETER READ

double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt);
