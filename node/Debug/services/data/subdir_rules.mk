################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
services/data/%.o: ../services/data/%.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv6/tools/compiler/gcc-arm-none-eabi-4_8-2014q3/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -fno-exceptions -DF_CPU=80000000L -DARDUINO=101 -DENERGIA=13 -I"C:/ti/energia-0101E0014/hardware/cc3200/cores/cc3200" -I"C:/Users/fpapinea/Documents/Coding/washing-machine-fw/node" -I"C:/ti/energia-0101E0014/hardware/cc3200/variants/launchpad" -I"C:/ti/ccsv6/tools/compiler/gcc-arm-none-eabi-4_8-2014q3/arm-none-eabi/include" -I"C:/Users/fpapinea/Documents/Coding/washing-machine-fw/node" -I"C:/ti/energia-0101E0014/hardware/cc3200/libraries/WiFi" -I"C:/ti/energia-0101E0014/hardware/cc3200/libraries/MQTTClient" -I"C:/ti/energia-0101E0014/hardware/cc3200/libraries/Wire" -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -fno-rtti -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


