################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/board/board.c 

OBJS += \
./hefei_libraries/board/board.o 

C_DEPS += \
./hefei_libraries/board/board.d 


# Each subdirectory must supply rules for building sources it contributes
hefei_libraries/board/board.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/hefei_libraries/board/board.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\CODE" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_libraries\board" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\src" -I"D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\USER\inc" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\hefei_peripheral" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\Core" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

