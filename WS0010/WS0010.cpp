#include "WS0010.h"
#include "Arduino.h"
/*
 * This library does not use SPI Hardware
 * because we need to send 10 bits exactly
 *
 * 10 -> Chip Select
 * 11 -> Serial Data Out
 * 12 -> Serial Data Input
 * 13 -> Serial Clock
*/

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
	
	uploadTurkishChars();
	sendCmd(RETURN_HOME);
}

void WS0010::showCursor(){
	sendCmd(BLINK_AND_CURSOR);
}

void WS0010::hideCursor(){
	sendCmd(TURN_ON_DISPLAY);
}

void WS0010::clear(){	
	sendCmd(CLEAR_DISPLAY);
	sendCmd(RETURN_HOME);
}

void WS0010::moveCursor(uint8_t row, uint8_t col){
	row = (row > 0)? 1 : 0;
	col = (col > 63)? 63 : col;
	
	byte cursorPos = row * 64 + col; 
	sendCmd(SET_DDRAM_ADDR | cursorPos);
}

uint8_t WS0010::isBusy()
{
	PORTB &= B00111011;
	
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
	PORTB |= B00000100;
	return data;
}

void WS0010::sendCmd(uint8_t cmd){
	isBusy();
	
	PORTB &= B00111011;
	send(0,2);
	send(cmd, 8);
	PORTB |= B00000100;
}

void WS0010::sendData(uint8_t *data, uint8_t length){
	PORTB &= B00111011;
	send(0x80,2);
	
	for(uint8_t i=0; i<length; i++){
		send(data[i], 8);
	}
	
	PORTB |= B00000100;
}

/* MSB FIRST */
void WS0010::send(uint8_t data, uint8_t length)
{
	uint8_t mask = 0x80;
	for(uint8_t i=0; i<length; i++)
	{		
		if(data & mask)
			PORTB |= B00001000;		
		else 						
			PORTB &= B11110111;
		
		PORTB &= B11011111;
		PORTB |= B00100000;
		mask = mask >> 1;
	}
}

void WS0010::uploadTurkishChars(){
	sendCmd(SET_CGRAM_ADDR);
	
	PORTB &= B00111011;
	send(0x80,2);
	
	for(uint8_t i=0; i<64; i++){
		send(pgm_read_byte(&turkishChars[i]), 8);
	}
	PORTB |= B00000100;
}

void WS0010::print(char* text)
{
	PORTB &= B00111011;
	send(0x80, 2);
	
	while(*text != '\0')
	{
		uint8_t byteCount = 0;
		for(uint8_t i=0; i<5; i++)
		{
			if(*text & (0x80 >> i))
				byteCount++;
			else
				break;
		}
		
		}
		if(byteCount == 0){
			byteCount = 1;
		}
		
		char newVal = 0x07; // Bilinmeyen karakterler için gülen surat 
		if(byteCount == 2)
		{
			uint16_t unicodedChar = *text & 0x1F;
			unicodedChar <<= 6;
			uint8_t secondByte = text[1] & 0x3F;
			unicodedChar |= secondByte;
			
			switch(unicodedChar){
				case 0x00E7: newVal = 0x87; break; //ç
				case 0x011F: newVal = 0x00; break; //ğ
				case 0x0131: newVal = 0x01; break; //ı
				case 0x00F6: newVal = 0x94; break; //ö
				case 0x015F: newVal = 0x02; break; //ş
				case 0x00FC: newVal = 0x81; break; //ü
				case 0x00C7: newVal = 0x80; break; //Ç
				case 0x0130: newVal = 0x03; break; //İ
				case 0x011E: newVal = 0x04; break; //Ğ
				case 0x00D6: newVal = 0x05; break; //Ö
				case 0x015E: newVal = 0x06; break; //Ş
				case 0x00DC: newVal = 0x9A; break; //Ü
			}                                        
		}
		else{
			newVal = *text;
		}
		send(newVal, 8);
		text += byteCount;
	}
	PORTB |= B00000100;
}
