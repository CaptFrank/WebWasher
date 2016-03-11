################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/Users/Francis Papineau/Documents/energia-0101E0017/hardware/tools/lm4f

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./platform/sensor/sensor/math/fixed_t.o" \
"./platform/sensor/sensor/physics/physics.o" \
"./node.o" \
"./platform/bus/i2c/bus_i2c.o" \
"./platform/iface/mqtt.o" \
"./platform/iface/wifi.o" \
"./platform/sensor/drivers/bosch/bma222.o" \
"./platform/sensor/drivers/ti/tmp006.o" \
"./platform/sensor/sensor/i2c/sensor_i2c.o" \
"./platform/sensor/sensor/math/fixed.o" \
"./platform/sensor/sensor/math/matrix.o" \
"./platform/sensor/sensor/physics/press_altitude.o" \
"./platform/sensor/sensor/sensor.o" \
"./service/services/coms/coms.o" \
"./service/services/formatter/formatter.o" \
"./service/services/system/system.o" \
"./task/scheduler.o" \
"./task/task.o" \
"./task/tasks/daq.o" \
"./task/tasks/idle.o" \
"./task/tasks/publish.o" \
"./task/tasks/update.o" \
$(GEN_CMDS__FLAG) \
-Wl,--start-group \
-Wl,-T"C:/Users/Francis Papineau/Documents/energia-0101E0017/hardware/cc3200/cores/cc3200/cc3200.ld" \
-l"lpcc3200_MQTTClient" \
-l"lpcc3200_WiFi" \
-l"lpcc3200_core" \
-l"lpcc3200_Wire" \
-l"m" \
-l"c" \
-l"gcc" \
-Wl,--end-group \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include task/tasks/subdir_vars.mk
-include task/subdir_vars.mk
-include service/services/system/subdir_vars.mk
-include service/services/formatter/subdir_vars.mk
-include service/services/coms/subdir_vars.mk
-include platform/sensor/sensor/subdir_vars.mk
-include platform/sensor/sensor/physics/subdir_vars.mk
-include platform/sensor/sensor/math/subdir_vars.mk
-include platform/sensor/sensor/i2c/subdir_vars.mk
-include platform/sensor/drivers/ti/subdir_vars.mk
-include platform/sensor/drivers/bosch/subdir_vars.mk
-include platform/iface/subdir_vars.mk
-include platform/bus/i2c/subdir_vars.mk
-include subdir_rules.mk
-include task/tasks/subdir_rules.mk
-include task/subdir_rules.mk
-include service/services/system/subdir_rules.mk
-include service/services/formatter/subdir_rules.mk
-include service/services/coms/subdir_rules.mk
-include platform/sensor/sensor/subdir_rules.mk
-include platform/sensor/sensor/physics/subdir_rules.mk
-include platform/sensor/sensor/math/subdir_rules.mk
-include platform/sensor/sensor/i2c/subdir_rules.mk
-include platform/sensor/drivers/ti/subdir_rules.mk
-include platform/sensor/drivers/bosch/subdir_rules.mk
-include platform/iface/subdir_rules.mk
-include platform/bus/i2c/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
node.out \

EXE_OUTPUTS__QUOTED += \
"node.out" \

BIN_OUTPUTS += \
node.bin \

BIN_OUTPUTS__QUOTED += \
"node.bin" \


# All Target
all: node.out secondary-outputs

# Tool invocations
node.out: $(OBJS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Linker'
	"C:/Users/Francis Papineau/Documents/energia-0101E0017/hardware/tools/lm4f/bin/arm-none-eabi-gcc.exe" -march=armv7e-m -mthumb -fno-exceptions -DF_CPU=80000000L -DARDUINO=101 -DENERGIA=13 -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --entry ResetISR -nostartfiles -nostdlib -Wl,--gc-sections -L"/Debug" -L"/Debug" -L"/Debug" -L"/Debug" -o"node.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

node.bin: $(EXE_OUTPUTS)
	@echo 'Invoking: GNU Objcopy Utility'
	"C:/Users/Francis Papineau/Documents/energia-0101E0017/hardware/tools/lm4f/bin/arm-none-eabi-objcopy.exe" -O binary $(EXE_OUTPUTS__QUOTED) "node.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "node.d" "task\tasks\daq.d" "task\tasks\idle.d" "task\tasks\publish.d" "task\tasks\update.d" "task\scheduler.d" "task\task.d" "service\services\system\system.d" "service\services\formatter\formatter.d" "service\services\coms\coms.d" "platform\sensor\sensor\sensor.d" "platform\sensor\sensor\physics\press_altitude.d" "platform\sensor\sensor\math\fixed.d" "platform\sensor\sensor\math\matrix.d" "platform\sensor\sensor\i2c\sensor_i2c.d" "platform\sensor\drivers\ti\tmp006.d" "platform\sensor\drivers\bosch\bma222.d" "platform\iface\mqtt.d" "platform\iface\wifi.d" "platform\bus\i2c\bus_i2c.d" 
	-$(RM) "platform\sensor\sensor\physics\physics.d" "platform\sensor\sensor\math\fixed_t.d" 
	-$(RM) "node.o" "task\tasks\daq.o" "task\tasks\idle.o" "task\tasks\publish.o" "task\tasks\update.o" "task\scheduler.o" "task\task.o" "service\services\system\system.o" "service\services\formatter\formatter.o" "service\services\coms\coms.o" "platform\sensor\sensor\sensor.o" "platform\sensor\sensor\physics\physics.o" "platform\sensor\sensor\physics\press_altitude.o" "platform\sensor\sensor\math\fixed.o" "platform\sensor\sensor\math\fixed_t.o" "platform\sensor\sensor\math\matrix.o" "platform\sensor\sensor\i2c\sensor_i2c.o" "platform\sensor\drivers\ti\tmp006.o" "platform\sensor\drivers\bosch\bma222.o" "platform\iface\mqtt.o" "platform\iface\wifi.o" "platform\bus\i2c\bus_i2c.o" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

