################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Modules/mBT.c \
../source/Modules/mBluetooth.c \
../source/Modules/mCpu.c \
../source/Modules/mDelay.c \
../source/Modules/mIMU.c \
../source/Modules/mIcOc.c \
../source/Modules/mLcd.c \
../source/Modules/mLeds.c \
../source/Modules/mLine.c \
../source/Modules/mMeteo.c \
../source/Modules/mRs232.c \
../source/Modules/mSwitch.c 

OBJS += \
./source/Modules/mBT.o \
./source/Modules/mBluetooth.o \
./source/Modules/mCpu.o \
./source/Modules/mDelay.o \
./source/Modules/mIMU.o \
./source/Modules/mIcOc.o \
./source/Modules/mLcd.o \
./source/Modules/mLeds.o \
./source/Modules/mLine.o \
./source/Modules/mMeteo.o \
./source/Modules/mRs232.o \
./source/Modules/mSwitch.o 

C_DEPS += \
./source/Modules/mBT.d \
./source/Modules/mBluetooth.d \
./source/Modules/mCpu.d \
./source/Modules/mDelay.d \
./source/Modules/mIMU.d \
./source/Modules/mIcOc.d \
./source/Modules/mLcd.d \
./source/Modules/mLeds.d \
./source/Modules/mLine.d \
./source/Modules/mMeteo.d \
./source/Modules/mRs232.d \
./source/Modules/mSwitch.d 


# Each subdirectory must supply rules for building sources it contributes
source/Modules/%.o: ../source/Modules/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Gestionnaires" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Utils\LibString" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\include" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\portable" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\source" -I../board -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Interfaces" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Modules" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Utils" -I../source -I../ -I../startup -I../CMSIS -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


