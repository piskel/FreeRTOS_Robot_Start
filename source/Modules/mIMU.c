/*
------------------------------------------------------------
Copyright 2003-20xx Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	mGyroAccelMag.c
Auteur et Date :	Monnerat Serge 8.5.20xx

Description dans le fichier mGyroAccelMag.h
------------------------------------------------------------
*/

#include "mIMU.h"
#include "iSpi.h"

// For MPU-9250

void mIMU_Setup(void)
    {
    iSpi_Setup();
    }

void mIMU_Open(void)
    {
    iSpi_Enable();

    mIMU_WriteData(IMU_PWR_MGMT_1, 0b10000000); // H Reset
    char pwr_mgmt2_data = 0x00; // Enabling ACC and GYRO

    mIMU_WriteData(IMU_PWR_MGMT_2, pwr_mgmt2_data);
    }




void mIMU_WriteData(char address, char data)
    {
    address &= ~0b10000000;
    UInt16 frame = (address << 8) | data;

    iSpi_SendData(frame);
    }

char mIMU_ReadData(char address)
    {
    address |= 0b10000000; // MSB : 1 for read
    UInt16 frame = (address << 8) & 0xFF00;

    return iSpi_SendData(frame);
    }

Int16 mIMU_GetAccX(void)
    {
    Int16 result = (Int16)(mIMU_ReadData(IMU_ACCEL_XOUT_H)<<8);
    result += mIMU_ReadData(IMU_ACCEL_XOUT_L);
    return result;
    }
Int16 mIMU_GetAccY(void)
    {
    Int16 result = (Int16)(mIMU_ReadData(IMU_ACCEL_YOUT_H)<<8);
    result += mIMU_ReadData(IMU_ACCEL_YOUT_L);
    return result;
    }
Int16 mIMU_GetAccZ(void)
    {
    Int16 result = (Int16)(mIMU_ReadData(IMU_ACCEL_ZOUT_H)<<8);
    result += mIMU_ReadData(IMU_ACCEL_ZOUT_L);
    return result;
    }

Int16 mIMU_GetGyroX(void)
    {
    Int16 result = (Int16)(mIMU_ReadData(IMU_GYRO_XOUT_H)<<8);
    result += mIMU_ReadData(IMU_GYRO_XOUT_L);
    return result;
    }
Int16 mIMU_GetGyroY(void)
    {
    Int16 result = (Int16)(mIMU_ReadData(IMU_GYRO_YOUT_H)<<8);
    result += mIMU_ReadData(IMU_GYRO_YOUT_L);
    return result;
    }
Int16 mIMU_GetGyroZ(void)
    {
    Int16 result = (Int16)(mIMU_ReadData(IMU_GYRO_ZOUT_H)<<8);
    result += mIMU_ReadData(IMU_GYRO_ZOUT_L);
    return result;
    }





