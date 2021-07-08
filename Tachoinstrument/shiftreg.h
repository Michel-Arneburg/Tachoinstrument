#ifndef ShiftRegister_h
#define ShiftRegister_h

#include<inttypes.h>

class ShiftRegister
{
	public:
	ShiftRegister(
	volatile uint8_t *ddr_addr,
	volatile uint8_t *port_addr,
	uint8_t ds_pin,
	uint8_t st_cp_pin,
	uint8_t sh_cp_pin
	);
	//address of the uC Data Direction Register
	volatile uint8_t *DDR_addr;
	//address of the uC PORT Register
	volatile uint8_t *PORT_addr;
	//Pin number to which the DS pin is connected
	uint8_t DS_pin;
	//Pin number to which the ST_CP pin is connected
	uint8_t ST_CP_pin;
	//Pin number to which the SH_CP pin is connected
	uint8_t SH_CP_pin;
	void out(unsigned char byte);
};

#endif