#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>

#define UP '0'
#define DOWN '1'
#define R_LEFT '2'
#define R_RIGHT '3'
#define FRONT '4'
#define BACK '5'
#define LEFT '6'
#define RIGHT '7'
#define START '8'
#define TERMINATE '9'

int current_speed=50;
int modification=10;


void setup_pwm()
{
	TCCR1A=0;
	TCCR1B=0;
	TCCR1A |= (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1) ;        //non-inverting mode
	TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS10);          // fast PWM(14)         prescal 1 ,,,,,   1MHz
	ICR1=19999;         //20ms periodic pulse
}

void calibrate_motors()
{
	OCR1A=1999;
	_delay_ms(3000);
	OCR1A=1000;
	_delay_ms(5000);
}

void speed(int x)
{
	OCR1A=1000+x;
}

void init_bluetooth()
{
	// double speed, disable multi-proc
	UCSRA = 0b00000010;
	// Enable Tx and Rx, disable interrupts
	UCSRB = 0b00011000;
	// Asynchronous mode, no parity, 1 stop bit, 8 data bits
	UCSRC = 0b10000110;
	// Baud rate 9600bps, assuming 1MHz clock
	UBRRL = 0x0C;
	UBRRH = 0x00;
}

void SEND(unsigned char data)
{
	// checking if UDRE flag is set to logic 1
	if(UCSRA & (1<<UDRE))  UDR = data; // Write character to UDR for trnsmsn
}
unsigned char RECEIVE()
{
	// checking if RXC flag is set to logic 1
	if(UCSRA & (1<<RXC))  return UDR; // Read the received character from UDR
	return 'p';
}


void UP_Throttle()
{
	if(current_speed<=999) current_speed = current_speed + modification;
	speed(current_speed);
}

void DOWN_Throttle()
{
	if(current_speed>=0) current_speed = current_speed - modification;
	speed(current_speed);
}

/*void Rotate_Left()
{
	unsigned long long int i=0;
	while(i<delay_time)
	{
		speed(1,current_speed1);
		speed(2,current_speed2+rotation_modification);
		speed(3,current_speed3);
		speed(4,current_speed4+rotation_modification);
		i++;
	}
}

void Rotate_Right()
{
	unsigned long long int i=0;
	while(i<delay_time)
	{
		speed(1,current_speed1+rotation_modification);
		speed(2,current_speed2);
		speed(3,current_speed3+rotation_modification);
		speed(4,current_speed4);
		i++;
	}
}

void Move_Front()
{
	unsigned long long int i=0;
	while(i<delay_time)
	{
		speed(1,current_speed1);
		speed(2,current_speed2);
		speed(3,current_speed3+movement_modification);
		speed(4,current_speed4+movement_modification);
		i++;
	}
	
}

void Move_Back()
{
	unsigned long long int i=0;
	while(i<delay_time)
	{
		speed(1,current_speed1+movement_modification);
		speed(2,current_speed2+movement_modification);
		speed(3,current_speed3);
		speed(4,current_speed4);
		i++;
	}
}

void Move_Left()
{
	unsigned long long int i=0;
	while(i<delay_time)
	{
		speed(1,current_speed1+movement_modification);
		speed(2,current_speed2);
		speed(3,current_speed3);
		speed(4,current_speed4+movement_modification);
		i++;
	}
}

void Move_Right()
{
	unsigned long long int i=0;
	while(i<delay_time)
	{
		speed(1,current_speed1);
		speed(2,current_speed2+movement_modification);
		speed(3,current_speed3+movement_modification);
		speed(4,current_speed4);
		i++;
	}
}
*/

int main(void)
{
	DDRD=0xff;
	setup_pwm();
	calibrate_motors();
	init_bluetooth();
	UDR=0;
	unsigned char x;
	
	while(RECEIVE()!=START);

	while(1)
	{
		x=RECEIVE();
		if(x==UP) 
		{
			UP_Throttle();
		}
		else if (x==DOWN)
		{
			DOWN_Throttle();
		}
		else
		{
			speed(current_speed);	
		}
	}
}