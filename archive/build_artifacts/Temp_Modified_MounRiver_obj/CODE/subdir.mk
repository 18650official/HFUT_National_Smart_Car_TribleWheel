################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/board_led.c \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_adc.c \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_init.c \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_keys.c \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_ligth.c \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_motion.c \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_reed.c \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_uart.c \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_value_process.c 

OBJS += \
./CODE/board_led.o \
./CODE/car_adc.o \
./CODE/car_init.o \
./CODE/car_keys.o \
./CODE/car_ligth.o \
./CODE/car_motion.o \
./CODE/car_reed.o \
./CODE/car_uart.o \
./CODE/car_value_process.o 

C_DEPS += \
./CODE/board_led.d \
./CODE/car_adc.d \
./CODE/car_init.d \
./CODE/car_keys.d \
./CODE/car_ligth.d \
./CODE/car_motion.d \
./CODE/car_reed.d \
./CODE/car_uart.d \
./CODE/car_value_process.d 


# Each subdirectory must supply rules for building sources it contributes
CODE/board_led.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/board_led.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
CODE/car_adc.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_adc.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
CODE/car_init.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_init.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
CODE/car_keys.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_keys.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
CODE/car_ligth.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_ligth.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
CODE/car_motion.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_motion.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
CODE/car_reed.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_reed.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
CODE/car_uart.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_uart.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
CODE/car_value_process.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/CODE/car_value_process.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

