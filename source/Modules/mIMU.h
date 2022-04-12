/*
------------------------------------------------------------
Copyright 2003-201x Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name : 	mGyroAccelMag.h	
Author and date :	Monnerat Serge 06.06.20xx

Goal : exploitation du gyroscope/acc�l�rom�tre/magn�toscope INVENSENSE MPU-9250

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/


#ifndef	__MGYROACCELMAG__
#define	__MGYROACCELMAG__

#include "def.h"

void mIMU_Setup(void);
void mIMU_Open(void);

void mIMU_WriteData(char address, char data);
char mIMU_ReadData(char address);

Int16 mIMU_GetAccX(void);
Int16 mIMU_GetAccY(void);
Int16 mIMU_GetAccZ(void);

Int16 mIMU_GetGyroX(void);
Int16 mIMU_GetGyroY(void);
Int16 mIMU_GetGyroZ(void);


// CONSTANTS
// from https://github.com/kriswiner/MPU9250/blob/master/MPU9250BasicAHRS_t3.ino

#define IMU_SELF_TEST_X_ACCEL 0x0D
#define IMU_SELF_TEST_Y_ACCEL 0x0E
#define IMU_SELF_TEST_Z_ACCEL 0x0F

#define IMU_SELF_TEST_A      0x10

#define IMU_XG_OFFSET_H      0x13  // User-defined trim values for gyroscope
#define IMU_XG_OFFSET_L      0x14
#define IMU_YG_OFFSET_H      0x15
#define IMU_YG_OFFSET_L      0x16
#define IMU_ZG_OFFSET_H      0x17
#define IMU_ZG_OFFSET_L      0x18
#define IMU_SMPLRT_DIV       0x19
#define IMU_CONFIG           0x1A
#define IMU_GYRO_CONFIG      0x1B
#define IMU_ACCEL_CONFIG     0x1C
#define IMU_ACCEL_CONFIG2    0x1D
#define IMU_LP_ACCEL_ODR     0x1E
#define IMU_WOM_THR          0x1F

#define IMU_MOT_DUR          0x20  // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
#define IMU_ZMOT_THR         0x21  // Zero-motion detection threshold bits [7:0]
#define IMU_ZRMOT_DUR        0x22  // Duration counter threshold for zero motion interrupt generation, 16 Hz rate, LSB = 64 ms

#define IMU_FIFO_EN          0x23
#define IMU_I2C_MST_CTRL     0x24
#define IMU_I2C_SLV0_ADDR    0x25
#define IMU_I2C_SLV0_REG     0x26
#define IMU_I2C_SLV0_CTRL    0x27
#define IMU_I2C_SLV1_ADDR    0x28
#define IMU_I2C_SLV1_REG     0x29
#define IMU_I2C_SLV1_CTRL    0x2A
#define IMU_I2C_SLV2_ADDR    0x2B
#define IMU_I2C_SLV2_REG     0x2C
#define IMU_I2C_SLV2_CTRL    0x2D
#define IMU_I2C_SLV3_ADDR    0x2E
#define IMU_I2C_SLV3_REG     0x2F
#define IMU_I2C_SLV3_CTRL    0x30
#define IMU_I2C_SLV4_ADDR    0x31
#define IMU_I2C_SLV4_REG     0x32
#define IMU_I2C_SLV4_DO      0x33
#define IMU_I2C_SLV4_CTRL    0x34
#define IMU_I2C_SLV4_DI      0x35
#define IMU_I2C_MST_STATUS   0x36
#define IMU_INT_PIN_CFG      0x37
#define IMU_INT_ENABLE       0x38
#define IMU_DMP_INT_STATUS   0x39  // Check DMP interrupt
#define IMU_INT_STATUS       0x3A
#define IMU_ACCEL_XOUT_H     0x3B
#define IMU_ACCEL_XOUT_L     0x3C
#define IMU_ACCEL_YOUT_H     0x3D
#define IMU_ACCEL_YOUT_L     0x3E
#define IMU_ACCEL_ZOUT_H     0x3F
#define IMU_ACCEL_ZOUT_L     0x40
#define IMU_TEMP_OUT_H       0x41
#define IMU_TEMP_OUT_L       0x42
#define IMU_GYRO_XOUT_H      0x43
#define IMU_GYRO_XOUT_L      0x44
#define IMU_GYRO_YOUT_H      0x45
#define IMU_GYRO_YOUT_L      0x46
#define IMU_GYRO_ZOUT_H      0x47
#define IMU_GYRO_ZOUT_L      0x48
#define IMU_EXT_SENS_DATA_00 0x49
#define IMU_EXT_SENS_DATA_01 0x4A
#define IMU_EXT_SENS_DATA_02 0x4B
#define IMU_EXT_SENS_DATA_03 0x4C
#define IMU_EXT_SENS_DATA_04 0x4D
#define IMU_EXT_SENS_DATA_05 0x4E
#define IMU_EXT_SENS_DATA_06 0x4F
#define IMU_EXT_SENS_DATA_07 0x50
#define IMU_EXT_SENS_DATA_08 0x51
#define IMU_EXT_SENS_DATA_09 0x52
#define IMU_EXT_SENS_DATA_10 0x53
#define IMU_EXT_SENS_DATA_11 0x54
#define IMU_EXT_SENS_DATA_12 0x55
#define IMU_EXT_SENS_DATA_13 0x56
#define IMU_EXT_SENS_DATA_14 0x57
#define IMU_EXT_SENS_DATA_15 0x58
#define IMU_EXT_SENS_DATA_16 0x59
#define IMU_EXT_SENS_DATA_17 0x5A
#define IMU_EXT_SENS_DATA_18 0x5B
#define IMU_EXT_SENS_DATA_19 0x5C
#define IMU_EXT_SENS_DATA_20 0x5D
#define IMU_EXT_SENS_DATA_21 0x5E
#define IMU_EXT_SENS_DATA_22 0x5F
#define IMU_EXT_SENS_DATA_23 0x60
#define IMU_MOT_DETECT_STATUS 0x61
#define IMU_I2C_SLV0_DO      0x63
#define IMU_I2C_SLV1_DO      0x64
#define IMU_I2C_SLV2_DO      0x65
#define IMU_I2C_SLV3_DO      0x66
#define IMU_I2C_MST_DELAY_CTRL 0x67
#define IMU_SIGNAL_PATH_RESET  0x68
#define IMU_MOT_DETECT_CTRL  0x69
#define IMU_USER_CTRL        0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define IMU_PWR_MGMT_1       0x6B // Device defaults to the SLEEP mode
#define IMU_PWR_MGMT_2       0x6C
#define IMU_DMP_BANK         0x6D  // Activates a specific bank in the DMP
#define IMU_DMP_RW_PNT       0x6E  // Set read/write pointer to a specific start address in specified DMP bank
#define IMU_DMP_REG          0x6F  // Register in DMP from which to read or to which to write
#define IMU_DMP_REG_1        0x70
#define IMU_DMP_REG_2        0x71
#define IMU_FIFO_COUNTH      0x72
#define IMU_FIFO_COUNTL      0x73
#define IMU_FIFO_R_W         0x74
#define IMU_WHO_AM_I	 0x75 // Should return 0x71
#define IMU_XA_OFFSET_H      0x77
#define IMU_XA_OFFSET_L      0x78
#define IMU_YA_OFFSET_H      0x7A
#define IMU_YA_OFFSET_L      0x7B
#define IMU_ZA_OFFSET_H      0x7D
#define IMU_ZA_OFFSET_L      0x7E

#define IMU_ADDRESS 0x69


#endif
