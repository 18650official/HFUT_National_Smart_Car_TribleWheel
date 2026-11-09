################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/common.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/delay.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_adc.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_common_font.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_dma.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_dvp.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_encoder.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_exti.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_flash.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_gpio.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_i2c_hardware.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_i2c_software.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_nvic.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_pit.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_pwm.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_spi.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_tim.c \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_uart.c 

OBJS += \
./hefei_libraries/common.o \
./hefei_libraries/delay.o \
./hefei_libraries/hf_adc.o \
./hefei_libraries/hf_common_font.o \
./hefei_libraries/hf_dma.o \
./hefei_libraries/hf_dvp.o \
./hefei_libraries/hf_encoder.o \
./hefei_libraries/hf_exti.o \
./hefei_libraries/hf_flash.o \
./hefei_libraries/hf_gpio.o \
./hefei_libraries/hf_i2c_hardware.o \
./hefei_libraries/hf_i2c_software.o \
./hefei_libraries/hf_nvic.o \
./hefei_libraries/hf_pit.o \
./hefei_libraries/hf_pwm.o \
./hefei_libraries/hf_spi.o \
./hefei_libraries/hf_tim.o \
./hefei_libraries/hf_uart.o 

C_DEPS += \
./hefei_libraries/common.d \
./hefei_libraries/delay.d \
./hefei_libraries/hf_adc.d \
./hefei_libraries/hf_common_font.d \
./hefei_libraries/hf_dma.d \
./hefei_libraries/hf_dvp.d \
./hefei_libraries/hf_encoder.d \
./hefei_libraries/hf_exti.d \
./hefei_libraries/hf_flash.d \
./hefei_libraries/hf_gpio.d \
./hefei_libraries/hf_i2c_hardware.d \
./hefei_libraries/hf_i2c_software.d \
./hefei_libraries/hf_nvic.d \
./hefei_libraries/hf_pit.d \
./hefei_libraries/hf_pwm.d \
./hefei_libraries/hf_spi.d \
./hefei_libraries/hf_tim.d \
./hefei_libraries/hf_uart.d 


# Each subdirectory must supply rules for building sources it contributes
hefei_libraries/common.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/common.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/delay.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/delay.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_adc.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_adc.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_common_font.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_common_font.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_dma.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_dma.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_dvp.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_dvp.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_encoder.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_encoder.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_exti.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_exti.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_flash.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_flash.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_gpio.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_gpio.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_i2c_hardware.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_i2c_hardware.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_i2c_software.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_i2c_software.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_nvic.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_nvic.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_pit.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_pit.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_pwm.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_pwm.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_spi.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_spi.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_tim.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_tim.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
hefei_libraries/hf_uart.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/hf_uart.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

