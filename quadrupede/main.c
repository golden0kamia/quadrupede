/*
 * quadrupede.c
 *
 * Created: 29.04.2019 21:20:19
 * Author : Bastien Piguet
 */

#define F_CPU 1000000UL
#define CLK PINC0
#define CE PINC1
#define DATA PINC2
#define RESET PINC3
#define RS PINC4

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

void delay_us();
void servo(char angl, char pin);
char alpha(int x, int y);
char beta(int x, int y);
void senduart(char txt);
void writeLED(char led);

int baud = 9600;
char txt;

int main(void)
{
	DDRC |= (1<<CLK)|(1<<CE)|(1<<DATA)|(1<<RESET)|(1<<RS);
	PINC |= (1<<CE)|(1<<RESET);
	DDRD |= (1<<PIND1)|(1<<PIND7);
	UCSR0A = 1<<U2X0;
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
	UCSR0C = (3<<UCSZ00);
	UBRR0L = 12;
	//DDRD |= (1<<PIND1);
	
	char cmd;
	PINC |= 1<<RESET;
	_delay_us(100);
	PINC |= 1<<RESET;
	
    while (1) 
    {
		if((UCSR0A & (1<<RXC0))){
			switch(UDR0)
			{
			case '0' :
				senduart('V');
				break;
			case '1' :
				senduart('U');
				break;
			case '2' :
				senduart('D');
				break;
			case '3' :
				senduart('L');
				break;
			case '4' :
				senduart('R');
				break;
			default:
				senduart('#');
				break;
			}
		}
		writeLED(0b01111110);
    }
}

void servo(char angl, char pin){
	int t = angl;
	PINB |= 1<<pin;
	_delay_us(500);
	delay_us(t);
	PINB |= 1<<pin;
}

void delay_us(int n){
	for(int i; i<n; i++){
		_delay_us(1);
	}
}

char alpha(int x, int y){
	return -acos(x*sqrt(x*x+y*y)/x*x+y*y)*2*M_PI+asin(sqrt(x*x+y*y)/10)*2*M_PI+90;
}

char beta(int x, int y){
	return -2*asin(sqrt(x*x+y*y)/10)*2*M_PI+180;
}

void senduart(char txt){
	while(!(UCSR0A & (1<<UDRE0))){}
	UDR0 = txt;
}

void writeLED(char led){
	PINC |= 1<<CE;
	for(int i; i<8; i++){
		if(led & (1<<i)){
			PINC |= 1<<DATA;
		}
		_delay_us(100);
		PINC |= 1<<CLK;
		_delay_us(100);
		if(led & (1<<i)){
			PINC |= 1<<DATA;
		}
		_delay_us(100);
		PINC |= 1<<CLK;
		_delay_us(100);
	}
	PINC |= 1<<CE;
	_delay_us(1);
}
