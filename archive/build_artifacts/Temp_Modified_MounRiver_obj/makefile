################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include user_c/subdir.mk
-include hefei_peripheral/subdir.mk
-include hefei_libraries/board/subdir.mk
-include hefei_libraries/subdir.mk
-include CODE/subdir.mk
-include CH32V30x/Core/subdir.mk
-include CH32V30x/CH32V30x_p/Startup/subdir.mk
-include CH32V30x/CH32V30x_p/CH32V30x_Lib/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Temp_Modified.hex \

SECONDARY_LIST += \
Temp_Modified.lst \

SECONDARY_SIZE += \
Temp_Modified.siz \


# All Target
all: Temp_Modified.elf secondary-outputs

# Tool invocations
Temp_Modified.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -T "D:\Dev\智能车竞赛\项目文件夹\Temp_Modified\MounRiver\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"Temp_Modified.map" --specs=nano.specs --specs=nosys.specs -o "Temp_Modified.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
Temp_Modified.hex: Temp_Modified.elf
	@	riscv-none-embed-objcopy -O ihex "Temp_Modified.elf"  "Temp_Modified.hex"
	@	@
Temp_Modified.lst: Temp_Modified.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble "Temp_Modified.elf" > "Temp_Modified.lst"
	@	@
Temp_Modified.siz: Temp_Modified.elf
	@	riscv-none-embed-size --format=berkeley "Temp_Modified.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Temp_Modified.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
