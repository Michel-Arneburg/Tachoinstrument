#include "shiftreg.h"

#define BIT_MASK		0x80
#define cbi(addr,bit)	addr &= ~(1 << bit)
#define sbi(addr,bit)	addr |= (1 << bit)

#define shift_in_pulse(port,reg_pin)				\
	sbi(port,reg_pin);								\
	cbi(port,reg_pin)								\

#define parallel_out_pulse(port,reg_pin)			\
	sbi(port,reg_pin);								\
	cbi(port,reg_pin)								\

ShiftRegister::ShiftRegister(
volatile uint8_t *ddr_addr,
volatile uint8_t *port_addr,
uint8_t ds_pin,
uint8_t st_cp_pin,
uint8_t sh_cp_pin
)
{
	this->DDR_addr = ddr_addr;
	this->PORT_addr = port_addr;
	this->DS_pin = ds_pin;
	this->ST_CP_pin= st_cp_pin;
	this->SH_CP_pin= sh_cp_pin;

	sbi(*(DDR_addr),DS_pin);				
	sbi(*(DDR_addr),ST_CP_pin);		
	sbi(*(DDR_addr),SH_CP_pin);		
}

void ShiftRegister::out(unsigned char byte)
{
		unsigned char mask,i;

		mask = BIT_MASK;

		for (i = 0; i< 8; i++)
		{
			if (byte & mask)
			{
				sbi(*(PORT_addr),DS_pin);
				shift_in_pulse(*(PORT_addr),SH_CP_pin);
			}
			else
			{
				cbi(*(PORT_addr),DS_pin);
				shift_in_pulse(*(PORT_addr),SH_CP_pin);

			}
			mask = (mask >> 1);
		}
		parallel_out_pulse(*(PORT_addr),ST_CP_pin);
}
