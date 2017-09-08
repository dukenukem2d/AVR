#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iotn2313.h>
#include <util/delay.h>

//#define

unsigned char PWM_val = 0;
unsigned char timecounter = 0;
ISR(TIMER1_OVF_vect)
{

	if(timecounter<1)
	{
		timecounter++;
	}
	else
	{

		while(PWM_val>0)
		{
			_delay_ms(5);
			OCR0A = PWM_val;
			PWM_val--;
		}
		OCR0A = 0;
		_delay_ms(2000);
		GIMSK |=(1<<INT0);
	}

}

ISR(INT0_vect)
{
	GIMSK &= ~(1<<INT0);

	while(PWM_val < 255)
	{
		_delay_ms(5);
		OCR0A = PWM_val;
		PWM_val++;
	}
	OCR0A = 255;

	timecounter = 0;
	TCNT1L = 0;
	TCNT1H = 0;
	TCCR1B |=(1<<CS12)|(0<<CS11)|(1<<CS10); //  Timer1 Prescaler 1024
}






int main(void)
{

	// --------------------- Init  Ports----------------------------------

	DDRD &= ~(1<<INT0); //Swith on INT0 as input
	DDRB |= (1<< PB2)|(1<<PB0); //Put  as output PB2(OCA0)
	PORTB = 0x00;

	//---------------------External Interrupt Init----------------------

	GIMSK |=(1<<INT0); //Switch External Interrupt  INT0
	MCUCR |=(1<<ISC01)|(1<<ISC00); //Rising Edge Init

	//--------------------- Fast PWM init--------------------------------------

	TCCR0A |=(1<<COM0A1)|(1<<WGM00); //Timer0 Fast PWM Mode
	TCCR0B |=(1<<CS02); // Timer0 Prescaler  256
	OCR0A = 0x00;

	//---------------------Timer 1 Interrupt Enable---------------------

	TIMSK |= (1<<TOIE1); //Timer1 Interrupt Enable

	sei();

	while(1)
	{

	}
	return 0;
}
