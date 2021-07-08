#include <avr/io.h>
#include <avr/interrupt.h>
#include "shiftreg.h"

//tire circumference per signal pulse
const double meterPerPulse = 0.838;
//configured counter clock interval
const double secondsPerCount = 0.002048;

volatile double kmPerh;

ShiftRegister reg(&DDRB,&PORTB,PINB0,PINB1,PINB2);

//digit numbers like index for register
const uint8_t digits[10] = {
	//PGFEDCBA
	0b11000000,
	0b11111001,
	0b10100100,
	0b10110000,
	0b10011001,
	0b10010010,
	0b10000010,
	0b11111000,
	0b10000000,
	0b10010000
};

void calcSpeed()
{
	//set to 0 if sensor timeout
	if (TCNT0>=0xFF) 
	{
		kmPerh = 0;
		return;
	}
	kmPerh = (meterPerPulse/(secondsPerCount*TCNT0))*3.6;
	//limit to display size
	if (kmPerh > 999)
	{
		kmPerh = 999;
		return;
	}
}

//interrupts on logic state change from hall effect sensor
ISR(PCINT0_vect)
{
	//return if rising edge
	if (PINA & (1<<PA0)) return;
	calcSpeed();
	//clear counts
	TCNT0 = 0;
	//reenable counting
	TCCR0B = (1<<CS02)|(0<<CS01)|(1<<CS00);
}

//interrupts on timer0 overflow
ISR(TIM0_OVF_vect)
{
	//stop counting
	TCCR0B = kmPerh = 0;
	//set maximum count
	TCNT0 = 0xFF;
}

//interrupts on compare match A
ISR(TIM0_COMPA_vect)
{
	/*countdown
	clks = TCNT0;
	OCR0A --;
	*/	
}

//sets up all io registers
void ioinit(void)
{
	/*init pwm
	DDRB = (1<<PINB2);
	//set timer/counter control register to "fast pwm" wave genration mode
	TCCR0A = (1<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0);
	*/
	//set outputs
	DDRA = (1<<PINA1)|(1<<PINA2)|(1<<PINA3);
	//assign clock source with 1/1024 prescaler, effects timer0A too
	TCCR0B = (1<<CS02)|(0<<CS01)|(1<<CS00);
	//enable interrupt
	TIMSK0 = (1<<TOIE0)|(1<<OCF0A);
	//assign pin to pin change mask for monitoring
	PCMSK0 = (1<<PCINT0);
	//enable pin change mask interruption
	GIMSK = (1<<PCIE0);
	//enable global interrupts
	sei();
}

int main(void)
{
	//clock prescaler above 1/32 too slow for programmer -> softbricked after programming
	//enable clk prescaler editing
	CLKPR = (1<<CLKPCE);
	//set 1/16 prescaling
	CLKPR = (0<<CLKPS3)|(1<<CLKPS2)|(0<<CLKPS1)|(0<<CLKPS0);
	ioinit();
	TCNT0 = 0xFF;
	calcSpeed();
	//keep updating register and enabling corresponding 7seg display
	while (1)
	{
		reg.out(digits[((uint8_t)kmPerh/100)%10]);
		PORTA = (1<<PINA1);
		reg.out(digits[((uint8_t)kmPerh/10)%10]);
		PORTA = (1<<PINA2);
		reg.out(digits[(uint8_t)kmPerh%10]);
		PORTA = (1<<PINA3);
	}
	return(0);}
