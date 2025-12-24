################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src/EXTI_program.c \
../src/src/Flash_EEPROM_Emulator_Program.c \
../src/src/GPIO_program.c \
../src/src/NVIC_program.c \
../src/src/RCC_program.c \
../src/src/SYS_program.c \
../src/src/UART_program.c \
../src/src/_write.c \
../src/src/main.c 

OBJS += \
./src/src/EXTI_program.o \
./src/src/Flash_EEPROM_Emulator_Program.o \
./src/src/GPIO_program.o \
./src/src/NVIC_program.o \
./src/src/RCC_program.o \
./src/src/SYS_program.o \
./src/src/UART_program.o \
./src/src/_write.o \
./src/src/main.o 

C_DEPS += \
./src/src/EXTI_program.d \
./src/src/Flash_EEPROM_Emulator_Program.d \
./src/src/GPIO_program.d \
./src/src/NVIC_program.d \
./src/src/RCC_program.d \
./src/src/SYS_program.d \
./src/src/UART_program.d \
./src/src/_write.d \
./src/src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/src/%.o: ../src/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


