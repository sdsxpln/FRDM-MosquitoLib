#include "MK64F12.h"
#include "system.h"

/**
 * Busy loop
 * Use timers for an exact amount of time!
 */
void system_delay_us(uint32_t us)
{
  volatile uint32_t i = us*system_get_clk()/1000/1000;
  for(; i>0; i--);
}

/**
 * FOR DEBUG PURPOSE
 * route flexbus clock to PCR3 so you can validate
 * the system clock
 */
void system_route_flexbusclk()
{
  /* enable GORTC gated clock */
  PORTC_PCR(3) = PORT_PCR_MUX(5);
  /*select FlexBus clock*/
  SIM_SOPT2 &= ~(uint32_t)SIM_SOPT2_CLKOUTSEL_MASK;
  /*SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(0);*/
}

/**
 * Get the bus clock
 */
uint32_t system_get_clk()
{
  uint8_t div = (SIM_CLKDIV1&SIM_CLKDIV1_OUTDIV1_MASK)>>SIM_CLKDIV1_OUTDIV1_SHIFT;
  return SYSTEM_CLOCK/(div+1);
}

/**
 * Get the bus clock
 */
uint32_t system_get_busclk()
{
  uint8_t div = (SIM_CLKDIV1&SIM_CLKDIV1_OUTDIV2_MASK)>>SIM_CLKDIV1_OUTDIV2_SHIFT;
  return SYSTEM_CLOCK/(div+1);
}

/**
 * Initiales a minimal system
 */
void system_init()
{
  SIM_CLKDIV1 = (0 | SIM_CLKDIV1_OUTDIV1(0) |  /* Core/system 120 Mhz */
                     SIM_CLKDIV1_OUTDIV2(1) |  /* Busclk  60 Mhz */
                     SIM_CLKDIV1_OUTDIV3(2) |  /* FlexBus  40 Mhz */
                     SIM_CLKDIV1_OUTDIV4(4));  /* Flash  24 Mhz */
  /* Select high frequency range for the crystal oscillator */
  MCG->C2 = MCG_C2_RANGE(0x02);
  /* Enable external reference clock */
  OSC->CR = OSC_CR_ERCLKEN_MASK;
  /* Select OSCCLK0 */
  MCG->C7 &= ~(uint8_t)(MCG_C7_OSCSEL_MASK);
  /* MCG->C1: CLKS=2,FRDIV=5,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
  MCG->C1 = (MCG_C1_CLKS(0x02) | MCG_C1_FRDIV(0x05) | MCG_C1_IRCLKEN_MASK);
  /* MCG->C4: DMX32=0,DRST_DRS=0 */
  MCG->C4 &= (uint8_t)~(uint8_t)((MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x03)));
  /* MCG->C5: ??=0,PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=0x13 */
  MCG->C5 = MCG_C5_PRDIV0(0x13);
  /* MCG->C6: LOLIE0=0,PLLS=0,CME0=0,VDIV0=0x18 */
  MCG->C6 = MCG_C6_VDIV0(0x18);
  /* Check that the source of the FLL reference clock is the external reference clock. */
  while((MCG->S & MCG_S_IREFST_MASK) != 0x00U);
  /* Wait until external reference clock is selected as MCG output */
  while((MCG->S & 0x0CU) != 0x08U);

  /* Switch to PBE Mode */
  /* MCG->C6: LOLIE0=0,PLLS=1,CME0=0,VDIV0=0x18 */
  MCG->C6 = (MCG_C6_PLLS_MASK | MCG_C6_VDIV0(0x18));
  /* Wait until external reference clock is selected as MCG output */
  while((MCG->S & 0x0CU) != 0x08U);
  /* Wait until locked */
  while((MCG->S & MCG_S_LOCK0_MASK) == 0x00U);
  /* Switch to PEE Mode */
  /* MCG->C1: CLKS=0,FRDIV=5,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
  MCG->C1 = (MCG_C1_CLKS(0x00) | MCG_C1_FRDIV(0x05) | MCG_C1_IRCLKEN_MASK);
  /* Wait until output of the PLL is selected */
  while((MCG->S & 0x0CU) != 0x0CU);
}


