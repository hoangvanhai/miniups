################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Inverter/inverter.obj: ../Inverter/inverter.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --include_path="E:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/Application" --include_path="E:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/Hardware/common/include" --include_path="E:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/Hardware/headers/include" --include_path="E:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/Hardware" --include_path="E:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/Lib" --include_path="E:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/Console" --include_path="E:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/Bsp" --include_path="E:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/Booster" --include_path="E:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/Inverter" --include_path="E:/1.WORK/1.MiniUPS/software/workspace/MiniUPS/Hardware/libraries/math/IQmath/c28/include" --advice:performance=all -g --diag_suppress=232 --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --preproc_with_compile --preproc_dependency="Inverter/inverter.d_raw" --obj_directory="Inverter" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


