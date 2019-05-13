/*
 * quadrupede.c
 *
 * Created: 29.04.2019 21:20:19
 * Author : Bastien Piguet
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

void delay_us();
void servo(char angl, char pin);
char alpha(int x, int y);
char beta(int x, int y);
void senduart(char txt);

int baud = 9600;
char txt;

int main(void)
{
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
	UCSR0C = (3<<UCSZ00);
	UBRR0L = F_CPU/(16*baud)+1;
	DDRD |= (1<<PIND1);
	
	/*char posBase[1][4][2] = {{{0, 7}, {0, 7}, {0, 7}, {0, 7}}};
	char walk[4][4][2] = {{{7, 7}, {0, 7}, {0, 7}, {7, 7}},
						  {{0, 7}, {-7, 7}, {-7, 7}, {0, 7}},
						  {{0, 7}, {7, 7}, {7, 7}, {0, 7}},
						  {{-7, 7}, {0, 7}, {0, 7}, {-7, 7}}};
	char pin[4][2] = {{PINB0, PINB1}, {PINB2, PINB3}, {PINB4, PINB5}, {PINB6, PINB7}};
	int speed;*/
	char cmd;
	
    while (1) 
    {
		/*while(!(UCSR0A & (1<<UDRE0))){}
		UDR0 = 'L';
		while(!(UCSR0A & (1<<UDRE0))){}
		UDR0 = 'O';
		while(!(UCSR0A & (1<<UDRE0))){}
		UDR0 = 'L';
		while(!(UCSR0A & (1<<UDRE0))){}
		UDR0 = '\n';
		_delay_ms(1000);*/
		if((UCSR0A & (1<<UDRE0))){
			cmd = UDR0;
		}
		switch(cmd)
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
			break;
		}
		cmd = 0;
		_delay_ms(1000);
		/*if(cmd == '1'){
			for(char i; i<sizeof(walk); i++){
				for(char j; j<sizeof(walk[i]); j++){
					servo(alpha(walk[i][j][0], walk[i][j][1]), pin[j][0]);
					servo(beta(walk[i][j][0], walk[i][j][1]), pin[j][1]);
				}
				delay_us(10000);
			}
			delay_us(speed);
		}*/
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
