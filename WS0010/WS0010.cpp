#include "WS0010.h"
#include "Arduino.h"


WS0010::WS0010(){
	PORTB |= B00101100;
	DDRB |= B00101100;	// 10, 11, 13 OUTPUT
	DDRB &= B11101111;	// 12 INPUT
}

void WS0010::init(){
	sendCmd(CLEAR_DISPLAY);
	sendCmd(FUNCTION_SET);
	sendCmd(CHAR_MODE_PWR_ON);
	sendCmd(TURN_ON_DISPLAY);
	sendCmd(ENTRY_MODE);
	sendCmd(RETURN_HOME);
}

void WS0010::clear(){
	sendCmd(CLEAR_DISPLAY);
	sendCmd(RETURN_HOME);
}

void WS0010::moveCursor(uint8_t row, uint8_t col){
	row = (row > 0)? 1 : 0;
	col = (col > 63)? 63 : col;
	
	uint8_t position = row * 64 + col; 
	sendCmd(0x80 | position);
}

uint8_t WS0010::isBusy(){
	PORTB &= B00111011;														// digitalWrite(10, LOW);
	
	uint8_t data = 0x80;
	while((0x80 & data))
	{
		send(0x40, 2);
		send(0x00, 8);	// Dummy write
		
		for(uint8_t i=0; i<8; i++){
			digitalWrite(13, LOW);
			digitalWrite(13, HIGH);
			data = data << 1;
			data |= digitalRead(12);
		}
	}
	PORTB |= B00000100; 													// digitalWrite(10, HIGH);
	return data;
}

void WS0010::sendCmd(uint8_t cmd){
	isBusy();
	
	PORTB &= B00111011;														// digitalWrite(10, LOW);
	send(0,2);
	send(cmd, 8);
	PORTB |= B00000100; 													// digitalWrite(10, HIGH);
}

void WS0010::sendData(uint8_t *data, uint8_t length){
	PORTB &= B00111011;														// digitalWrite(10, LOW);
	send(0x80,2);
	
	for(uint8_t i=0; i<length; i++){
		send(data[i], 8);
	}
	
	PORTB |= B00000100; 													// digitalWrite(10, HIGH);
}

void WS0010::print(char *string)
{
	PORTB &= B00111011;														// digitalWrite(10, LOW);
	send(0x80,2);
	
	while(*string != '\0'){
		send(*string++, 8);
	}
	
	PORTB |= B00000100; 													// digitalWrite(10, HIGH);
}

/* MSB FIRST */
void WS0010::send(uint8_t data, uint8_t length)
{
	uint8_t mask = 0x80;
	for(uint8_t i=0; i<length; i++)
	{		
		if(data & mask)														// digitalWrite(11, data & mask);
			PORTB |= B00001000;		
		else 						
			PORTB &= B11110111;
		
		PORTB &= B11011111;													// digitalWrite(13, LOW);
		PORTB |= B00100000;													// digitalWrite(13, HIGH);
		mask = mask >> 1;
	}
}
