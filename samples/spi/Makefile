CFLAGS= -I../../frdm-k64f/freescale/ -I../../frdm-k64f/freescale/MK64F12/include/  -I../../frdm-k64f/freescale/MK64F12/startup/  -I../../frdm-k64f/net/picotcp/include -I../../frdm-k64f/net/picotcp/modules -I../../frdm-k64f/net -I../../frdm-k64f/drivers/

CROSS=arm-none-eabi
# GCC_BASE = $(shell which arm-none-eabi-gcc)
GCC_BASE = /usr/arm-none-eabi
GCC = $(CROSS)-gcc
# Remember don't use ld directly, use gcc to call it!
LD = $(GCC)
GCC_LIB  = $(GCC_BASE)/lib/
GCC_INC  = $(GCC_BASE)/include/
AS = $(CROSS)-as
AR = $(CROSS)-ar
SIZE = $(CROSS)-size
OBJCOPY = $(CROSS)-objcopy
OBJDUMP = $(CROSS)-objdump
NM = $(CROSS)-nm




BUILD_DIR=build
PROJECT_OBJECTS   = main.o
LINKER_SCRIPT     = ./MK64FN1M0xxx12_flash.ld
PROJECT           = SpiExample
PROJECT_INC_PATHS = -I.
PROJECT_LIB_PATHS = -L.
PROJECT_LIBRARIES =
PROJECT_SYMBOLS   = -DTOOLCHAIN_GCC_ARM -DNO_RELOC='0'


CORTEX_M4_NOFP_CC_FLAGS = -mthumb -mcpu=cortex-m4
CORTEX_M4_NOFP_LIB_PATH = $(GCC_LIB)armv7e-m
CORTEX_M4_SWFP_CC_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16
CORTEX_M4_SWFP_LIB_PATH = $(GCC_LIB)armv7e-m/softfp
CORTEX_M4_HWFP_CC_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
CORTEX_M4_HWFP_LIB_PATH = $(GCC_LIB)armv7e-m/fpu

MCU_CC_FLAGS      = $(CORTEX_M4_HWFP_CC_FLAGS)
MCU_LIB_PATH      = $(CORTEX_M4_HWFP_LIB_PATH)

SYS_OBJECTS = 
SYS_INC_PATHS = -I. -I$(GCC_INC) 
SYS_LIB_PATHS = -L$(MCU_LIB_PATH)

CC_SYMBOLS = -DDEBUG $(PROJECT_SYMBOLS)

INCLUDE_PATHS = $(PROJECT_INC_LIB) $(SYS_INC_PATHS)
LIBRARY_PATHS = $(PROJECT_LIB_LIB) $(SYS_LIB_PATHS) 
CFLAGS += $(MCU_CC_FLAGS) -c $(CC_OPTIM_FLAGS) $(CC_DEBUG_FLAGS) -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections 
LD_FLAGS = $(MCU_CC_FLAGS) -Wl,--gc-sections $(SYS_LD_FLAGS) -Wl,-Map=$(PROJECT).map 
LD_SYS_LIBS = $(SYS_LIBRARIES)


BUILD_TARGET=spiexample


HEADERS=$(wildcard *.h)
HEADERS+=$(wildcard ../../drivers/*.h)
HEADERS+=$(wildcard ../../freescale/*.h)
HEADERS+=$(wildcard ../../net/picotcp/include/*.h)
HEADERS+=$(wildcard ../../net/picotcp/modules/*.h)



all: $(BUILD_TARGET).bin

clean:
	rm -f $(BUILD_TARGET).bin $(BUILD_TARGET).elf $(PROJECT_OBJECTS)

.s.o:
	$(AS) $(MCU_CC_FLAGS) -o $@ $<

.c.o:
	$(GCC)  $(CFLAGS) $(CC_SYMBOLS)   -std=gnu99   $(HEADERS) $(INCLUDE_PATHS) -o $@ $< -L. -lFRDM-Mosquito



$(BUILD_TARGET).elf: $(PROJECT_OBJECTS) 
	$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) $(LIBRARY_PATHS) -o $@ $^ $(PROJECT_LIBRARIES) $(SYS_LIBRARIES) 

$(BUILD_TARGET).bin: $(BUILD_TARGET).elf
	$(OBJCOPY) -O binary $< $@

