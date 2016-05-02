################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include DMemWin/png/emWin_V5.26/GUI/PNG/subdir.mk
-include DMemWin/subdir.mk
-include DMSupport/mbed-src/targets/hal/TARGET_NXP/TARGET_LPC408X/TARGET_LPC4088_DM/subdir.mk
-include DMSupport/mbed-src/targets/hal/TARGET_NXP/TARGET_LPC408X/subdir.mk
-include DMSupport/mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC408X/TOOLCHAIN_GCC_CR/subdir.mk
-include DMSupport/mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC408X/subdir.mk
-include DMSupport/mbed-src/common/subdir.mk
-include DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_M4/TOOLCHAIN_GCC/subdir.mk
-include DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/subdir.mk
-include DMSupport/mbed-rtos/rtos/subdir.mk
-include DMSupport/mbed-rpc/subdir.mk
-include DMSupport/USBDevice/USBSerial/subdir.mk
-include DMSupport/USBDevice/USBMSD/subdir.mk
-include DMSupport/USBDevice/USBMIDI/subdir.mk
-include DMSupport/USBDevice/USBHID/subdir.mk
-include DMSupport/USBDevice/USBDevice/subdir.mk
-include DMSupport/USBDevice/USBAudio/subdir.mk
-include DMSupport/Memory/subdir.mk
-include DMSupport/FileSystems/subdir.mk
-include DMSupport/EthernetInterface/lwip/netif/ppp/subdir.mk
-include DMSupport/EthernetInterface/lwip/netif/subdir.mk
-include DMSupport/EthernetInterface/lwip/core/snmp/subdir.mk
-include DMSupport/EthernetInterface/lwip/core/ipv4/subdir.mk
-include DMSupport/EthernetInterface/lwip/core/subdir.mk
-include DMSupport/EthernetInterface/lwip/api/subdir.mk
-include DMSupport/EthernetInterface/lwip-sys/arch/subdir.mk
-include DMSupport/EthernetInterface/lwip-eth/arch/TARGET_NXP/subdir.mk
-include DMSupport/EthernetInterface/Socket/subdir.mk
-include DMSupport/EthernetInterface/subdir.mk
-include DMSupport/Display/subdir.mk
-include DMSupport/DM_USBHost/USBHostSerial/subdir.mk
-include DMSupport/DM_USBHost/USBHostMSD/subdir.mk
-include DMSupport/DM_USBHost/USBHostMIDI/subdir.mk
-include DMSupport/DM_USBHost/USBHostHub/subdir.mk
-include DMSupport/DM_USBHost/USBHostHID/subdir.mk
-include DMSupport/DM_USBHost/USBHost3GModule/subdir.mk
-include DMSupport/DM_USBHost/USBHost/subdir.mk
-include DMSupport/DM_HttpServer/Handler/subdir.mk
-include DMSupport/DM_HttpServer/subdir.mk
-include DMSupport/DM_FATFileSystem/ChaN/subdir.mk
-include DMSupport/DM_FATFileSystem/subdir.mk
-include DMSupport/Bios/subdir.mk
-include DMSupport/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: mainController.axf

# Tool invocations
mainController.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -L"C:\Users\Garth\Documents\senior-design\mainController" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\Bios" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\Display" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\FileSystems" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\Memory" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_FATFileSystem" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_FATFileSystem\ChaN" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_HttpServer" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_HttpServer\Handler" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHost" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHost3GModule" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostHID" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostHub" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostMIDI" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostMSD" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostSerial" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\Socket" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-eth" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-eth\arch" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-eth\arch\TARGET_NXP" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-sys" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-sys\arch" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\ipv4" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\ipv4\lwip" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\lwip" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\netif" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\netif" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\netif\ppp" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\api" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\core" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\core\ipv4" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\core\snmp" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBAudio" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBDevice" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBHID" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBMIDI" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBMSD" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBSerial" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rpc" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtos" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M\TARGET_M4" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M\TARGET_M4\TOOLCHAIN_GCC" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\api" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\hal" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC408X" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC408X\TOOLCHAIN_GCC_CR" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC408X" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC408X\TARGET_LPC4088_DM" -L"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\common" -L"C:\Users\Garth\Documents\senior-design\mainController\DMemWin" -L"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\emwin" -L"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\emwin\emWin_header" -L"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\png" -L"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\emwin\emWin_library\LPCXpresso" -L"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\emwin\emWin_library\LPCXpresso" -Xlinker -Map="mainController.map" -Xlinker --gc-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -T "C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC408X\TOOLCHAIN_GCC_CR\LPC407x_8x.ld" -o "mainController.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) mainController.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "mainController.axf" ; arm-none-eabi-objcopy -O binary "mainController.axf" "mainController.bin" ; arm-none-eabi-objcopy -O ihex "mainController.axf" "mainController.hex" ; # checksum -p LPC4088 -d "mainController.bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
