################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CODE/CSM_camera_deal.c \
../CODE/Test.c \
../CODE/button.c \
../CODE/buzzer.c \
../CODE/display.c \
../CODE/elec.c \
../CODE/elements.c \
../CODE/encoder.c \
../CODE/findtrack.c \
../CODE/founctions.c \
../CODE/motor.c \
../CODE/timer_pit.c 

OBJS += \
./CODE/CSM_camera_deal.o \
./CODE/Test.o \
./CODE/button.o \
./CODE/buzzer.o \
./CODE/display.o \
./CODE/elec.o \
./CODE/elements.o \
./CODE/encoder.o \
./CODE/findtrack.o \
./CODE/founctions.o \
./CODE/motor.o \
./CODE/timer_pit.o 

C_DEPS += \
./CODE/CSM_camera_deal.d \
./CODE/Test.d \
./CODE/button.d \
./CODE/buzzer.d \
./CODE/display.d \
./CODE/elec.d \
./CODE/elements.d \
./CODE/encoder.d \
./CODE/findtrack.d \
./CODE/founctions.d \
./CODE/motor.d \
./CODE/timer_pit.d 


# Each subdirectory must supply rules for building sources it contributes
CODE/%.o: ../CODE/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"C:\Users\Administrator\Desktop\RT-Thread_Example\CH32V103_RTThread_Library-master\Smart_Car_Demo\Libraries\rtthread_libraries\bsp" -I"C:\Users\Administrator\Desktop\RT-Thread_Example\CH32V103_RTThread_Library-master\Smart_Car_Demo\Libraries\rtthread_libraries\components\finsh" -I"C:\Users\Administrator\Desktop\RT-Thread_Example\CH32V103_RTThread_Library-master\Smart_Car_Demo\Libraries\rtthread_libraries\include" -I"C:\Users\Administrator\Desktop\RT-Thread_Example\CH32V103_RTThread_Library-master\Smart_Car_Demo\Libraries\rtthread_libraries\include\libc" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

