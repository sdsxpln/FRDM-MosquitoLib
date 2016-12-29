#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define SYSTEM_CLOCK 120000000

void system_route_flexbusclk();
uint32_t system_get_busclk();
uint32_t system_get_clk();
void system_init();

#endif
