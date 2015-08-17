################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
sensors/%.o: ../sensors/%.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/Program Files/energia-0101E0015/hardware/tools/lm4f/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -fno-exceptions -DF_CPU=80000000L -DARDUINO=101 -DENERGIA=13 -I"C:/Program Files/energia-0101E0015/hardware/cc3200/cores/cc3200" -I"C:/Users/francis-ccs/workspace_v6_1/node" -I"C:/Program Files/energia-0101E0015/hardware/cc3200/variants/launchpad" -I"C:/Program Files/energia-0101E0015/hardware/tools/lm4f/arm-none-eabi/include" -I"C:/Users/francis-ccs/workspace_v6_1/node" -I"C:/Program Files/energia-0101E0015/hardware/cc3200/libraries/WiFi" -I"C:/Program Files/energia-0101E0015/hardware/cc3200/libraries/MQTTClient" -I"C:/Program Files/energia-0101E0015/hardware/cc3200/libraries/Wire" -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -fno-rtti -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


