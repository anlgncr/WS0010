#define INSTRUCTION_WRITE 0
#define READ_BUSY_FLAG 1
#define DATA_WRITE 2
#define DATA_READ 3

#define DATA   		PORTD
#define ENABLE 		(PORTB |= B00000010)
#define DISABLE		(PORTB &= B11111101)

void sendCommand(uint8_t command)
{
  checkBusy();
  
  DDRD = 0xFF;
  DATA = command;
  setMode(INSTRUCTION_WRITE);
  ENABLE;
  DISABLE;
}

uint8_t sendData(uint8_t data)
{
  checkBusy();
  
  DDRD = 0xFF;
  DATA = data;
  setMode(DATA_WRITE);
  ENABLE;
  DISABLE;
}

void setMode(byte mode){
	if(mode == INSTRUCTION_WRITE){
		PORTB &= B11111010;
	}
	else if(mode == READ_BUSY_FLAG){
		PORTB &= B11111110; 			// Select instruction register
		PORTB |= B00000100;			// Select read mode
	}
	else if(mode == DATA_WRITE){
		PORTB |= B00000001; 			// Select data register
		PORTB &= B11111011;			// Select write mode
	}
	else if(mode == DATA_READ){
		PORTB |= B00011000; 			// Select data register, Select read mode
	}
}

void checkBusy()
{
  DDRD = 0x00;
  
  setMode(READ_BUSY_FLAG);
  ENABLE;
  while (PIND & 0x80);
  DISABLE;
}

void writeString(char* s)
{
  byte i = 0;
  
  while (s[i] != NULL)
  {
    sendData(s[i]);
    i++;
  } 
}


void setup()
{
	DDRD = 0xFF;
	DDRB = 0x07;
	PORTB = 0x00;
	
	delay(500);
	
	sendCommand(0x3B); // function set (8 bit, 2 line, 5x8, western european)
	sendCommand(0x08); // turn off display
	sendCommand(0x01); // clear display
	sendCommand(0x02); // return home
	sendCommand(0x06); // entry mode (increment, no shift)
	
	sendCommand(0x17); // character mode, power on
	sendCommand(0x0C); // turn on display
	
	
	sendCommand(0x80);
	writeString("Hello World!");
	sendCommand(0x80 | 0x40);
	writeString("I am OLED");
	
}

void loop()
{
 
}
