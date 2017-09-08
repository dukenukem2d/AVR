/* Pinouts ATTINY2313

                                       + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
                   1    PA2     +   (PCINT10/RESET/dW)                                                                      +    VCC     20
                                       +                                                                                                                      +
                   2    PD0     +   (PCINT11/RxD)                               (USCK/SCL/SCK/PCINT7)  +    PB7      19
                                       +                                                                                                                      +
                   3    PD1     +   (PCINT12/TxD)                                            (MISO/DO/PCINT6)  +   PB6     18
                                       +                                                                                                                      +
                   4    PA1     +   (PCINT9/XTAL2)                                  (MOSI/DI/SDA/PCINT5)  +    PB5    17
                                       +                                                                                                                      +
                   5    PA0     +   (PCINT8/CLKI/XTAL1 )                                      (OC1B/PCINT4)  +   PB4    16
                                       +                                                                                                                       +
                   6    PD2     +   (PCINT13/CKOUT/XCK/INT0 )                       (OC1A/PCINT3)  +   PB3    15
                                       +                                                                                                                       +
                   7    PD3     +   (PCINT14/INT1)                                                   (OC0A/PCINT2)  +   PB2    14
                                       +                                                                                                                       +
                   8    PD4     +   (PCINT15/T0)                                                        (AIN1/PCINT1)   +   PB1    13
                                       +                                                                                                                       +
                   9    PD5     +   (PCINT16/OC0B/T1)                                            (AIN0/PCINT0)  +   PB0    12
                                       +                                                                                                                       +
                  10   GND   +                                                                                       (ICPI/PCINT17)  +   PD6     11
                                       + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +




 */

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
