CFLAGS= -Ifreescale/ -Ifreescale/MK64F12/include/  -Ifreescale/MK64F12/startup/ -DCPU_MK64FN1M0VDC12 -DMOSQUITO_CONFIG_SPI_ENABLED
LIB=FRDM-MosquitoLib.a

CFILES=$(wildcard *.c)
CFILES+=$(wildcard drivers/*.c)
CFILES+=$(wildcard freescale/*.c)
COBJS=$(CFILES:.c=.o)
HEADERS=$(wildcard *.h)
HEADERS+=$(wildcard drivers/*.h)
HEADERS+=$(wildcard freescale/*.h)


.PHONY: check_config

all: check_config $(LIB)


$(LIB): $(COBJS)
		$(AR) rs $@ $^

clean:  rm -f  $(COBJS)  

