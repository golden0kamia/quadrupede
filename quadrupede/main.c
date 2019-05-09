/*
 * quadrupede.c
 *
 * Created: 29.04.2019 21:20:19
 * Author : Bastien
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

void delay_us();
void servo(char angl, char pin);
char alpha(int x, int y);
char beta(int x, int y);

int main(void)
{
	UCSRA = 0;
	UCSRB = (1<<TXEN);
	UCSRC = (1<<UCSZ1)|(1<<UCSZ0);
	UBRRL = 0b00011001;
	UBRRH = 0;
	DDRB = 255;
	DDRD |= (1<<PIND1);
	
	char posBase[1][4][2] = {{{0, 7}, {0, 7}, {0, 7}, {0, 7}}};
	char walk[4][4][2] = {{{7, 7}, {0, 7}, {0, 7}, {7, 7}},
						  {{0, 7}, {-7, 7}, {-7, 7}, {0, 7}},
						  {{0, 7}, {7, 7}, {7, 7}, {0, 7}},
						  {{-7, 7}, {0, 7}, {0, 7}, {-7, 7}}};
	char pin[4][2] = {{PINB0, PINB1}, {PINB2, PINB3}, {PINB4, PINB5}, {PINB6, PINB7}};
	int speed;
	char cmd = '1';
	
    while (1) 
    {
		/*UDR = 'L';
		while(!(UCSRA & (1<<TXC)));
		UDR = 'O';
		while(!(UCSRA & (1<<TXC)));
		UDR = 'L';
		while(!(UCSRA & (1<<TXC)));
		_delay_ms(1000);
		switch(UDR)
		{
		case '1' :
			cmd = '1';
			break;
		}*/
		if(cmd == '1'){
			for(char i; i<sizeof(walk); i++){
				for(char j; j<sizeof(walk[i]); j++){
					servo(alpha(walk[i][j][0], walk[i][j][1]), pin[j][0]);
					servo(beta(walk[i][j][0], walk[i][j][1]), pin[j][1]);
				}
				delay_us(10000);
			}
			delay_us(speed);
		}
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
