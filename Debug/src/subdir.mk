################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Control_Panel.c \
../src/EXTI_program.c \
../src/GPIO_program.c \
../src/NVIC_program.c \
../src/RCC_program.c \
../src/SYS_program.c \
../src/Timer.c \
../src/UART_program.c \
../src/_write.c \
../src/main.c 

OBJS += \
./src/Control_Panel.o \
./src/EXTI_program.o \
./src/GPIO_program.o \
./src/NVIC_program.o \
./src/RCC_program.o \
./src/SYS_program.o \
./src/Timer.o \
./src/UART_program.o \
./src/_write.o \
./src/main.o 

C_DEPS += \
./src/Control_Panel.d \
./src/EXTI_program.d \
./src/GPIO_program.d \
./src/NVIC_program.d \
./src/RCC_program.d \
./src/SYS_program.d \
./src/Timer.d \
./src/UART_program.d \
./src/_write.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


