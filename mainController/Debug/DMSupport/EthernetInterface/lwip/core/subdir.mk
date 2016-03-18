################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DMSupport/EthernetInterface/lwip/core/def.c \
../DMSupport/EthernetInterface/lwip/core/dhcp.c \
../DMSupport/EthernetInterface/lwip/core/dns.c \
../DMSupport/EthernetInterface/lwip/core/init.c \
../DMSupport/EthernetInterface/lwip/core/mem.c \
../DMSupport/EthernetInterface/lwip/core/memp.c \
../DMSupport/EthernetInterface/lwip/core/netif.c \
../DMSupport/EthernetInterface/lwip/core/pbuf.c \
../DMSupport/EthernetInterface/lwip/core/raw.c \
../DMSupport/EthernetInterface/lwip/core/stats.c \
../DMSupport/EthernetInterface/lwip/core/tcp.c \
../DMSupport/EthernetInterface/lwip/core/tcp_in.c \
../DMSupport/EthernetInterface/lwip/core/tcp_out.c \
../DMSupport/EthernetInterface/lwip/core/timers.c \
../DMSupport/EthernetInterface/lwip/core/udp.c 

OBJS += \
./DMSupport/EthernetInterface/lwip/core/def.o \
./DMSupport/EthernetInterface/lwip/core/dhcp.o \
./DMSupport/EthernetInterface/lwip/core/dns.o \
./DMSupport/EthernetInterface/lwip/core/init.o \
./DMSupport/EthernetInterface/lwip/core/mem.o \
./DMSupport/EthernetInterface/lwip/core/memp.o \
./DMSupport/EthernetInterface/lwip/core/netif.o \
./DMSupport/EthernetInterface/lwip/core/pbuf.o \
./DMSupport/EthernetInterface/lwip/core/raw.o \
./DMSupport/EthernetInterface/lwip/core/stats.o \
./DMSupport/EthernetInterface/lwip/core/tcp.o \
./DMSupport/EthernetInterface/lwip/core/tcp_in.o \
./DMSupport/EthernetInterface/lwip/core/tcp_out.o \
./DMSupport/EthernetInterface/lwip/core/timers.o \
./DMSupport/EthernetInterface/lwip/core/udp.o 

C_DEPS += \
./DMSupport/EthernetInterface/lwip/core/def.d \
./DMSupport/EthernetInterface/lwip/core/dhcp.d \
./DMSupport/EthernetInterface/lwip/core/dns.d \
./DMSupport/EthernetInterface/lwip/core/init.d \
./DMSupport/EthernetInterface/lwip/core/mem.d \
./DMSupport/EthernetInterface/lwip/core/memp.d \
./DMSupport/EthernetInterface/lwip/core/netif.d \
./DMSupport/EthernetInterface/lwip/core/pbuf.d \
./DMSupport/EthernetInterface/lwip/core/raw.d \
./DMSupport/EthernetInterface/lwip/core/stats.d \
./DMSupport/EthernetInterface/lwip/core/tcp.d \
./DMSupport/EthernetInterface/lwip/core/tcp_in.d \
./DMSupport/EthernetInterface/lwip/core/tcp_out.d \
./DMSupport/EthernetInterface/lwip/core/timers.d \
./DMSupport/EthernetInterface/lwip/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
DMSupport/EthernetInterface/lwip/core/%.o: ../DMSupport/EthernetInterface/lwip/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -D__CODE_RED -DCPP_USE_HEAP -DTARGET_RTOS_M4_M7 -DTOOLCHAIN_GCC -DMBED_BUILD_TIMESTAMP=1456087336.46 -DTARGET_CORTEX_M -D__FPU_PRESENT=1 -DTARGET_NXP -DTOOLCHAIN_GCC_CR -DTARGET_M4 -D__MBED__=1 -DTARGET_LIKE_CORTEX_M4 -D__CORTEX_M4 -DTARGET_LPC4088_DM -DARM_MATH_CM4 -DTARGET_LIKE_MBED -DTARGET_LPC408X -I"C:\Users\Garth\Documents\senior-design\mainController" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\Bios" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\Display" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\FileSystems" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\Memory" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_FATFileSystem" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_FATFileSystem\ChaN" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_HttpServer" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_HttpServer\Handler" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHost" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHost3GModule" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostHID" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostHub" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostMIDI" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostMSD" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostSerial" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\Socket" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-eth" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-eth\arch" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-eth\arch\TARGET_NXP" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-sys" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-sys\arch" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\ipv4" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\ipv4\lwip" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\lwip" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\netif" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\netif" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\netif\ppp" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\api" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\core" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\core\ipv4" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\core\snmp" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBAudio" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBDevice" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBHID" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBMIDI" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBMSD" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBSerial" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rpc" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtos" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M\TARGET_M4" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M\TARGET_M4\TOOLCHAIN_GCC" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\api" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\hal" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC408X" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC408X\TOOLCHAIN_GCC_CR" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC408X" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC408X\TARGET_LPC4088_DM" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\common" -I"C:\Users\Garth\Documents\senior-design\mainController\DMemWin" -I"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\emwin" -I"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\emwin\emWin_header" -I"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\png" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


