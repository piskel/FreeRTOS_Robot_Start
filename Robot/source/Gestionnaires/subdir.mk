################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Gestionnaires/tBTTask.c \
../source/Gestionnaires/tDriveTask.c \
../source/Gestionnaires/tHMITask.c \
../source/Gestionnaires/tInputTask.c \
../source/Gestionnaires/tInterpretorTask.c \
../source/Gestionnaires/tPilotTask.c 

OBJS += \
./source/Gestionnaires/tBTTask.o \
./source/Gestionnaires/tDriveTask.o \
./source/Gestionnaires/tHMITask.o \
./source/Gestionnaires/tInputTask.o \
./source/Gestionnaires/tInterpretorTask.o \
./source/Gestionnaires/tPilotTask.o 

C_DEPS += \
./source/Gestionnaires/tBTTask.d \
./source/Gestionnaires/tDriveTask.d \
./source/Gestionnaires/tHMITask.d \
./source/Gestionnaires/tInputTask.d \
./source/Gestionnaires/tInterpretorTask.d \
./source/Gestionnaires/tPilotTask.d 


# Each subdirectory must supply rules for building sources it contributes
source/Gestionnaires/%.o: ../source/Gestionnaires/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Gestionnaires" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Utils\LibString" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\include" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\portable" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\source" -I../board -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Interfaces" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Modules" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Utils" -I../source -I../ -I../startup -I../CMSIS -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


