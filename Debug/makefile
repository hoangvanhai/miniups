################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Application/Booster/booster.obj" \
"./Application/Inverter/inverter.obj" \
"./Application/PIDCtrl/pid.obj" \
"./Application/SineGen.obj" \
"./Application/app.obj" \
"./Application/isr.obj" \
"./Application/main.obj" \
"./Application/template.obj" \
"./Bsp/BSP.obj" \
"./Console/printf.obj" \
"./Hardware/common/source/f2802x_adc.obj" \
"./Hardware/common/source/f2802x_asmfuncs.obj" \
"./Hardware/common/source/f2802x_codestartbranch.obj" \
"./Hardware/common/source/f2802x_comp.obj" \
"./Hardware/common/source/f2802x_cputimers.obj" \
"./Hardware/common/source/f2802x_defaultisr.obj" \
"./Hardware/common/source/f2802x_ecap.obj" \
"./Hardware/common/source/f2802x_epwm.obj" \
"./Hardware/common/source/f2802x_gpio.obj" \
"./Hardware/common/source/f2802x_i2c.obj" \
"./Hardware/common/source/f2802x_osccomp.obj" \
"./Hardware/common/source/f2802x_piectrl.obj" \
"./Hardware/common/source/f2802x_pievect.obj" \
"./Hardware/common/source/f2802x_sci.obj" \
"./Hardware/common/source/f2802x_spi.obj" \
"./Hardware/common/source/f2802x_sysctrl.obj" \
"./Hardware/headers/source/F2802x_GlobalVariableDefs.obj" \
"./Lib/Timer.obj" \
"./Lib/utils.obj" \
"../F2802x_Headers_nonBIOS.cmd" \
"../F2802x_generic_flash.cmd" \
"../Hardware/libraries/math/IQmath/c28/lib/IQmath.lib" \
"../Hardware/libraries/math/IQmath/c28/lib/IQmath_fpu32.lib" \
$(GEN_CMDS__FLAG) \
-lrts2800_ml.lib \
-lIQmath.lib \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Application/Booster/subdir_vars.mk
-include Application/Inverter/subdir_vars.mk
-include Application/PIDCtrl/subdir_vars.mk
-include Application/subdir_vars.mk
-include Bsp/subdir_vars.mk
-include Console/subdir_vars.mk
-include Hardware/common/source/subdir_vars.mk
-include Hardware/headers/source/subdir_vars.mk
-include Hardware/libraries/math/IQmath/c28/lib/subdir_vars.mk
-include Lib/subdir_vars.mk
-include subdir_rules.mk
-include Application/Booster/subdir_rules.mk
-include Application/Inverter/subdir_rules.mk
-include Application/PIDCtrl/subdir_rules.mk
-include Application/subdir_rules.mk
-include Bsp/subdir_rules.mk
-include Console/subdir_rules.mk
-include Hardware/common/source/subdir_rules.mk
-include Hardware/headers/source/subdir_rules.mk
-include Hardware/libraries/math/IQmath/c28/lib/subdir_rules.mk
-include Lib/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
MiniUPS.out \

EXE_OUTPUTS__QUOTED += \
"MiniUPS.out" \

BIN_OUTPUTS += \
MiniUPS.hex \

BIN_OUTPUTS__QUOTED += \
"MiniUPS.hex" \


# All Target
all: MiniUPS.out secondary-outputs

# Tool invocations
MiniUPS.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --advice:performance=all -g --diag_suppress=232 --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet -z -m"MiniUPS.map" --heap_size=0x100 --stack_size=0x200 --warn_sections -i"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/lib" -i"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" -i"D:/1.WORK/1.MiniUPS/software/workspace/MiniUPS" -i"D:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/hardware/libraries/math/IQmath/c28/lib" --reread_libs --issue_remarks --verbose_diagnostics --xml_link_info="MiniUPS.xml" --entry_point=code_start --rom_model -o "MiniUPS.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

MiniUPS.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/hex2000" --intel --binary -o "MiniUPS.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Application\Booster\booster.obj" "Application\Inverter\inverter.obj" "Application\PIDCtrl\pid.obj" "Application\SineGen.obj" "Application\app.obj" "Application\isr.obj" "Application\main.obj" "Application\template.obj" "Bsp\BSP.obj" "Console\printf.obj" "Hardware\common\source\f2802x_adc.obj" "Hardware\common\source\f2802x_asmfuncs.obj" "Hardware\common\source\f2802x_codestartbranch.obj" "Hardware\common\source\f2802x_comp.obj" "Hardware\common\source\f2802x_cputimers.obj" "Hardware\common\source\f2802x_defaultisr.obj" "Hardware\common\source\f2802x_ecap.obj" "Hardware\common\source\f2802x_epwm.obj" "Hardware\common\source\f2802x_gpio.obj" "Hardware\common\source\f2802x_i2c.obj" "Hardware\common\source\f2802x_osccomp.obj" "Hardware\common\source\f2802x_piectrl.obj" "Hardware\common\source\f2802x_pievect.obj" "Hardware\common\source\f2802x_sci.obj" "Hardware\common\source\f2802x_spi.obj" "Hardware\common\source\f2802x_sysctrl.obj" "Hardware\headers\source\F2802x_GlobalVariableDefs.obj" 
	-$(RM) "Lib\Timer.obj" "Lib\utils.obj" 
	-$(RM) "Application\Booster\booster.d" "Application\Inverter\inverter.d" "Application\PIDCtrl\pid.d" "Application\SineGen.d" "Application\app.d" "Application\isr.d" "Application\main.d" "Application\template.d" "Bsp\BSP.d" "Console\printf.d" "Hardware\common\source\f2802x_adc.d" "Hardware\common\source\f2802x_comp.d" "Hardware\common\source\f2802x_cputimers.d" "Hardware\common\source\f2802x_defaultisr.d" "Hardware\common\source\f2802x_ecap.d" "Hardware\common\source\f2802x_epwm.d" "Hardware\common\source\f2802x_gpio.d" "Hardware\common\source\f2802x_i2c.d" "Hardware\common\source\f2802x_osccomp.d" "Hardware\common\source\f2802x_piectrl.d" "Hardware\common\source\f2802x_pievect.d" "Hardware\common\source\f2802x_sci.d" "Hardware\common\source\f2802x_spi.d" "Hardware\common\source\f2802x_sysctrl.d" "Hardware\headers\source\F2802x_GlobalVariableDefs.d" "Lib\Timer.d" "Lib\utils.d" 
	-$(RM) "Hardware\common\source\f2802x_asmfuncs.d" "Hardware\common\source\f2802x_codestartbranch.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

