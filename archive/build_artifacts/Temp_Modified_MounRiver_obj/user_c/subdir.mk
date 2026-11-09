################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/USER/src/isr.c \
D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/USER/src/main.c 

OBJS += \
./user_c/isr.o \
./user_c/main.o 

C_DEPS += \
./user_c/isr.d \
./user_c/main.d 


# Each subdirectory must supply rules for building sources it contributes
user_c/isr.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/USER/src/isr.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/main.o: D:/Dev/智能车竞赛/项目文件夹/Temp_Modified/USER/src/main.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

