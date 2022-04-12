################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Utils/LibString/cpu_core.c \
../source/Utils/LibString/lib_ascii.c \
../source/Utils/LibString/lib_math.c \
../source/Utils/LibString/lib_mem.c \
../source/Utils/LibString/lib_str.c 

OBJS += \
./source/Utils/LibString/cpu_core.o \
./source/Utils/LibString/lib_ascii.o \
./source/Utils/LibString/lib_math.o \
./source/Utils/LibString/lib_mem.o \
./source/Utils/LibString/lib_str.o 

C_DEPS += \
./source/Utils/LibString/cpu_core.d \
./source/Utils/LibString/lib_ascii.d \
./source/Utils/LibString/lib_math.d \
./source/Utils/LibString/lib_mem.d \
./source/Utils/LibString/lib_str.d 


# Each subdirectory must supply rules for building sources it contributes
source/Utils/LibString/%.o: ../source/Utils/LibString/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Gestionnaires" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Utils\LibString" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\include" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\portable" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\freertos\source" -I../board -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Interfaces" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Modules" -I"C:\Users\luc.froideva\Documents\Cours\RTOS\workspace\FreeRTOS_Robot_Start\source\Utils" -I../source -I../ -I../startup -I../CMSIS -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


