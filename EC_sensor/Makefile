#====================================================================#
#Output files
EXECUTABLE=firmware.elf
BIN_IMAGE=firmware.bin

#======================================================================#
#Cross Compiler
HOST_CC=gcc
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb
#============================================================================#
#Stm32F10x Standard Libraries
CMSIS=../STM_Lib/CMSIS
ST=../STM_Lib/STM32F10x_StdPeriph_Driver
STARTUP=../STM_Lib/Startup/startup_stm32f10x_md.s
#Source Files
SRC+=	./system_stm32f10x.c \
		$(ST)/src/misc.c \
		$(ST)/src/stm32f10x_rcc.c \
		$(ST)/src/stm32f10x_gpio.c \
		$(ST)/src/stm32f10x_usart.c \
		$(ST)/src/stm32f10x_iwdg.c \
		$(ST)/src/stm32f10x_tim.c \
		$(ST)/src/stm32f10x_adc.c \
		$(ST)/src/stm32f10x_dma.c \
		$(ST)/src/stm32f10x_can.c \
		$(ST)/src/stm32f10x_rtc.c \
		$(ST)/src/stm32f10x_pwr.c 


#Included Header Files
CFLAGS_INCLUDE=-I./ \
		-I$(CMSIS)/CM3/CoreSupport \
		-I$(CMSIS)/CM3/DeviceSupport/ST/STM32F10x \
		-I$(ST)/inc 
#============================================================================#
#User's Application

USER_LIB=../User_Lib

#User Source Files
SRC+=	./mainlib.c \
		./CircularBuffer.c \
		./libEC/EC_rs485.c \
		./ATparser.c \
		./stm32f10x_it.c 


#User Header Files
CFLAGS_INCLUDE+=-I./ \
		-I$(USER_LIB)/myprintf
#======================================================================#
#Flags
CFLAGS_DEFINE= \
        -D USE_STDPERIPH_DRIVER \
		-D STM32F10X_MD \
#__CC_ARM
CFLAGS_OPTIMIZE= \
		-O
CFLAGS_NEW_LIB_NANO= \
		--specs=nano.specs --specs=nosys.specs -u _printf_float
#warning
CFLAGS_WARNING= \
		-Wall \
		-Wno-unused-parameter \
		-Wextra \
		-Wdouble-promotion \
		-Werror=array-bounds \
		-Wfatal-errors \
		-Wmissing-prototypes \
		-Wbad-function-cast \
		-Wmissing-parameter-type
		
#Linker
CFLAGS = -Wl,-T,stm32_flash.ld \
		 -Wall -g -mlittle-endian -mthumb\
		 -mcpu=cortex-m3
CFLAGS+=${CFLAGS_INCLUDE} \
        ${CFLAGS_DEFINE} \
        ${CFLAGS_NEW_LIB_NANO} \
        ${CFLAGS_WARNING}

LDFLAGS=-lm
#============================================================================#
#Make rules

#Make all
all:$(BIN_IMAGE)

$(BIN_IMAGE):$(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo '    OBJCOPY $(BIN_IMAGE)'

STARTUP_OBJ = startup_stm32f10x_md.o

$(STARTUP_OBJ): $(STARTUP)
	@$(CC) $(CFLAGS) $^ -c $(STARTUP)
	@echo '    CC $(STARTUP_OBJ)'

$(EXECUTABLE):$(SRC) $(STARTUP_OBJ)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo '    CC $(EXECUTABLE)'
PC_SIM:$(TEST_EXE)

$(TEST_EXE):$(HOST_SRC)
	$(HOST_CC) $(HOST_CFLAG) $^ -o $@
#Make clean
clean:
	rm -rf $(STARTUP_OBJ)
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
#Make flash with ST-Link
#flash_stlink:
#	st-flash write $(BIN_IMAGE) 0x8000000
#Make flash with Black Magic Probe(BMP)
#flash:
#	$(GDB) firmware.elf -x ./bmp_flash.gdb

#Start CGDB with Black Magic Probe(BMP)
cgdb:
	cgdb -d $(GDB) firmware.elf -x ./bmp_gdb.gdb
#Make flash with openocd
flash_openocd:
	make
	openocd -f interface/stlink-v2.cfg \
	-c "transport list" \
	-c "transport select hla_swd" \
	-f target/stm32f1x.cfg \
	-c "init" \
	-c "reset init" \
	-c "halt" \
	-c "flash write_image erase $(EXECUTABLE)" \
	-c "verify_image $(EXECUTABLE)" \
	-c "reset run" -c shutdown
#automatically formate
#Make gdbtui
gdbtui:
	$(GDB) -tui -x ../st_util_init.gdb
	
astyle: 
	astyle -r --exclude=lib  *.c *.h
#============================================================================#

.PHONY:all clean cgdb astyle
