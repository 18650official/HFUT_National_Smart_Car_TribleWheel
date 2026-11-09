################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
D:/Dev/智能车竞赛/项目文件夹/Libraries/Device/CH32V30x_p/Startup/startup_ch32v30x.S 

OBJS += \
./CH32V30x/CH32V30x_p/Startup/startup_ch32v30x.o 

S_UPPER_DEPS += \
./CH32V30x/CH32V30x_p/Startup/startup_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
CH32V30x/CH32V30x_p/Startup/startup_ch32v30x.o: D:/Dev/智能车竞赛/项目文件夹/Libraries/Device/CH32V30x_p/Startup/startup_ch32v30x.S
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -x assembler -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\Startup" -I"D:\Dev\智能车竞赛\项目文件夹\Libraries\Device\CH32V30x_p\CH32V30x_Lib" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

