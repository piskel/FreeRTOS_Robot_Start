################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include source/freertos/source/subdir.mk
-include source/freertos/portable/subdir.mk
-include source/Utils/LibString/subdir.mk
-include source/Utils/subdir.mk
-include source/Modules/subdir.mk
-include source/Interfaces/subdir.mk
-include source/Gestionnaires/subdir.mk
-include source/subdir.mk
-include board/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Robot.axf

# Tool invocations
Robot.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="Robot.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0plus -mthumb -T FreeRTOS_Robot_Start_Robot.ld -o "Robot.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Robot.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Robot.axf"; # arm-none-eabi-objcopy -v -O binary "Robot.axf" "Robot.bin" ; # checksum -p MKL46Z256xxx4 -d "Robot.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
