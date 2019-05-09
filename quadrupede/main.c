/*
 * quadrupede.c
 *
 * Created: 29.04.2019 21:20:19
 * Author : Bastien
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

void delay_us();
void servo(char angl, char pin);

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
	int speed;
	char cmd;
	
    while (1) 
    {
		UDR = 'L';
		while(!(UCSRA & (1<<TXC)));
		UDR = 'O';
		while(!(UCSRA & (1<<TXC)));
		UDR = 'L';
		while(!(UCSRA & (1<<TXC)));
		_delay_ms(1000);
		/*switch(UDR)
		{
		case '1' :
			cmd = '1';
			break;
		}
		if(cmd == '1'){
			for(char i; i<sizeof(walk); i++){
				for(char j; j<sizeof(walk[i]); j++){
					servo(walk[i][j], j);
				}
				_delay_ms(10);
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
