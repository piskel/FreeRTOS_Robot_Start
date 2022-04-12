################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/freertos/source/croutine.c \
../source/freertos/source/event_groups.c \
../source/freertos/source/list.c \
../source/freertos/source/queue.c \
../source/freertos/source/runTimeStatsTimer.c \
../source/freertos/source/tasks.c \
../source/freertos/source/timers.c 

OBJS += \
./source/freertos/source/croutine.o \
./source/freertos/source/event_groups.o \
./source/freertos/source/list.o \
./source/freertos/source/queue.o \
./source/freertos/source/runTimeStatsTimer.o \
./source/freertos/source/tasks.o \
./source/freertos/source/timers.o 

C_DEPS += \
./source/freertos/source/croutine.d \
./source/freertos/source/event_groups.d \
./source/freertos/source/list.d \
./source/freertos/source/queue.d \
./source/freertos/source/runTimeStatsTimer.d \
./source/freertos/source/tasks.d \
./source/freertos/source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
source/freertos/source/%.o: ../source/freertos/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Gestionnaires" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Utils\LibString" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\include" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\portable" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\source" -I../board -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Interfaces" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Modules" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Utils" -I../source -I../ -I../startup -I../CMSIS -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


