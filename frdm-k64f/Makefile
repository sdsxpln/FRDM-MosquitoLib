CFLAGS= -Ifreescale/ -Ifreescale/MK64F12/include/  -Ifreescale/MK64F12/startup/  -Inet/picotcp/include -Inet/picotcp/modules -Inet

CROSS=arm-none-eabi
GCC = $(CROSS)-gcc
AS = $(CROSS)-as
AR = $(CROSS)-ar
SIZE = $(CROSS)-size
OBJCOPY = $(CROSS)-objcopy
OBJDUMP = $(CROSS)-objdump
NM = $(CROSS)-nm
BUILD_DIR=build


###############
# DEFINES
###############
CFLAGS+=	-DCPU_MK64FN1M0VDC12 \
					-DMOSQUITO_CONFIG_SPI_ENABLED \
					-DPICO_SUPPORT_PING -DPICO_SUPPORT_ICMP4 \
					-DPICO_SUPPORT_IPV4 \
					-DPICO_SUPPORT_TFTP \
					-DPICO_SUPPORT_ETH \
					-DPICO_SUPPORT_IPV4 \
					-DPICO_SUPPORT_IPFRAG \
					-DPICO_SUPPORT_ICMP4 \
					-DPICO_SUPPORT_PING \
					-DPICO_SUPPORT_TCP \
					-DPICO_SUPPORT_UDP \
					-DPICO_SUPPORT_MCAST \
					-DPICO_SUPPORT_IGMP \
					-DPICO_SUPPORT_NAT \
					-DPICO_SUPPORT_DEVLOOP \
					-DPICO_SUPPORT_DHCPC \
					-DPICO_SUPPORT_DHCPD \
					-DPICO_SUPPORT_DNS_CLIENT \
					-DPICO_SUPPORT_MDNS \
					-DPICO_SUPPORT_IPFILTER \
					-DPICO_SUPPORT_CRC \
					-DPICO_SUPPORT_OLSR \
					-DPICO_SUPPORT_SLAACV4 \
					-DPICO_SUPPORT_IPV6 \
					-DPICO_SUPPORT_ICMP6 \
					-DPICO_SUPPORT_SNTP_CLIENT \
					-DPICO_SUPPORT_IPFRAG \
					-DPICO_SUPPORT_IPV4FRAG \
					-DPICO_SUPPORT_IPV6FRAG

LIB=./FRDM-MosquitoLib.a

CFILES=$(wildcard *.c)
CFILES+=$(wildcard drivers/*.c)
CFILES+=$(wildcard freescale/*.c)
CFILES+=$(wildcard net/picotcp/stack/*.c)
CFILES+=$(wildcard net/picotcp/modules/*.c)
CFILES:=$(filter-out net/picotcp/modules/pico_dev_tap_windows.c, $(CFILES))
CFILES:=$(filter-out net/picotcp/modules/pico_dev_vde.c, $(CFILES))
CFILES:=$(filter-out net/picotcp/modules/pico_dev_pcap.c, $(CFILES))
CFILES:=$(filter-out net/picotcp/modules/pico_dev_tun.c, $(CFILES))
CFILES:=$(filter-out net/picotcp/modules/pico_dev_tap.c, $(CFILES))
HEADERS=$(wildcard *.h)
HEADERS+=$(wildcard drivers/*.h)
HEADERS+=$(wildcard freescale/*.h)
HEADERS+=$(wildcard net/picotcp/include/*.h)
HEADERS+=$(wildcard net/picotcp/modules/*.h)
COBJS = $(addprefix $(BUILD_DIR)/, $(CFILES:.c=.o))

all: $(LIB)

$(COBJS): $(BUILD_DIR)/%.o : %.c
		@-mkdir -p $(@D)
		$(GCC) $(CFLAGS) -c $< -o $@

$(LIB): $(COBJS)
		$(AR) rs $(BUILD_DIR)/$@ $^


clean:
		rm -rf $(BUILD)
