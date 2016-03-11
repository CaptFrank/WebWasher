################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
node.cpp: ../node.ino
	@echo 'Building file: $<'
	@echo 'Invoking: Resource Custom Build Step'
	
	@echo 'Finished building: $<'
	@echo ' '

%.o: ./%.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/Users/Francis Papineau/Documents/energia-0101E0017/hardware/tools/lm4f/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -fno-exceptions -DF_CPU=80000000L -DARDUINO=101 -DENERGIA=13 -I"C:/Users/Francis Papineau/Documents/energia-0101E0017/hardware/cc3200/cores/cc3200" -I"C:/Users/francis-ccs/git/WebWasher/node" -I"C:/Users/Francis Papineau/Documents/energia-0101E0017/hardware/cc3200/variants/launchpad" -I"C:/Users/Francis Papineau/Documents/energia-0101E0017/hardware/cc3200/libraries/WiFi" -I"C:/Users/Francis Papineau/Documents/energia-0101E0017/hardware/cc3200/libraries/MQTTClient" -I"C:/Users/Francis Papineau/Documents/energia-0101E0017/hardware/cc3200/libraries/Wire" -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -fno-rtti -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


