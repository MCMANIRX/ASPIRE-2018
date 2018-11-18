#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


const char* command[] = {"blink"};

void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Enable receiver and transmitter 
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}
void dPuts(char* str) {
	int i =0;
	while(str[i] != '\0') {
		USART_Transmit(str[i]);
		i++;
	}
}


char* dGets(char* str, int length) {
	int i = 0;
	do {
		str[i] = USART_Receive();
		++i;
	
	
	}while(i < length-1 && str[i-1] != '\n' && str[i-1] != '\r');
	
str[i] = '\0';

	
	return str; 
}

int matchFind(char* str) {
	int cmdSize = strlen(command[0]);
	int uInSize = 0;
	int i = 0;
	int commandNumber = 0;
	
			while(1) {
				if(str[i] == command[0][i]){
					
					++uInSize;	
					}
				else break;
				++i;
			}
				++i;
	for(int j = uInSize; j < cmdSize; ++j)
			USART_Transmit(command[0][j]);
			return commandNumber;
}
void parseCommand(char* str) {
	int i = 0;
	while(str[i] != '\n' || str[i] != '\r'){
		
		if(str[i] == command[0][i]){
			snprintf(str, sizeof(str), "char is %c %x\n", str[i], str[i]);
			dPuts(str);
		}else break;
			++i;
	}
	
}
char* dGetsCom(char* str) {
	bool loop = true;
	bool print = true;
	int i = 0;
	while(loop){
		print = true;
		str[i] = USART_Receive();
		
		switch(str[i]){
			
			case '\r':
				loop = false;
				parseCommand(str);
				USART_Transmit('\n');
				break;
				
			case '\t':
				matchFind(str);
				break;
			
			case 0x7f:
				break;
				
			
		}
			USART_Transmit(str[i]);
	
		
		++i;
	}
	
	str[i] = '\0';

	
	return str;
}


int main(void)
{	
	
	char String[100]; 
	char byte;
	DDRB |= 1<<PB5;
	USART_Init(MYUBRR);
	dPuts("Initialized\n");
	
	for(;;){
		

		//byte = USART_Receive();
	//snprintf(String, sizeof(String), "Character %c written (%x)\n", byte, byte);
		//dPuts(String);
		
		dGetsCom(String);
	
	
		

		PORTB |= 1<<PB5;
		//_delay_ms(1000); 
		PORTB &= ~(1<<5);
		//_delay_ms(1000);
	
	}
	

}
