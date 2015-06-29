/*
 *	motordriver.c
 *
 *	Created: 2015-06-17 9:50:06 AM
 *  Author: David
 *	PWM truth table:
 *	PB3	PB2	PB1	- Output
 *	1	0	0	- brake (set pwm duty cycle to 100% to get 1) 
 *	PWM	0	0	- fwd	
 *	PWM	1	1	- rvs
 *	PWM	0	1	- Left
 *	PWM 1	0	- right
 */ 
 
 #include <avr/io.h>
#include <avr/interrupt.h>

#define FWD 1
#define RVS 2
#define LEFT 3
#define RIGHT 4
#define BREAK 0

int move(int speed, int direction);

int main(void)
{
	move(1,FWD);							//sample
}

int move(int speed, int direction)
{
	DDRB |=(1<<DDB3);
	OCR0A = 1*speed;					//50% duty cycle
	
	if(direction == FWD)	
	{
		PORTB &= ~(1<<PB1);				//set PB1 and PB2 low
		PORTB &= ~(1<<PB2);	
	}
	
	if(direction == RVS)
	{
		PORTB |= (1<<PB1);				//set PB1 and PB2 high
		PORTB |= (1<<PB2);
	}
	
	if(direction == LEFT)
	{
		PORTB &= ~(1<<PB1);				//set PB1 low and PB2 high
		PORTB |= (1<<PB2);
	}
	
	if(direction == RIGHT)
	{
		PORTB |= (1<<PB1);				//set PB1 high and PB2 low
		PORTB &= ~(1<<PB2);
	}
	
		else								//brake
	{
		OCR0A =	1;						//set 100% duty cycle
		PORTB &= ~(1<<PB1);				//set PB1 and PB2 low
    PORTB &= ~(1<<PB2);
	}	

	TCCR0A |= _BV(COM0A1)| _BV(WGM00);	//set PWM
	TIMSK0 = (1<<OCIE0A);				//enable interrupts
	TCCR0B = 1<<CS01;					//gives pre-scaler of 256
	sei();								//enable global interrupts
	while(1);
}

ISR(TIMER0_COMPA_vect)
{
	OCR0A = 0;						//reset speed
}
	
