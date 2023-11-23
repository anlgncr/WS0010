#ifndef WS0010_h
#define WS0010_h
#include "Arduino.h"

#define CLEAR_DISPLAY 			0x01
#define RETURN_HOME 			0x02
#define TURN_OFF_DISPLAY		0x08
#define TURN_ON_DISPLAY			0x0C
#define BLINK					0x0D
#define BLINK_AND_CURSOR		0x0F
#define SHIFT_CURSOR_LEFT		0x10
#define SHIFT_CURSOR_RIGHT		0x14
#define SHIFT_DISPLAY_RIGHT		0x1C
#define SHIFT_DISPLAY_LEFT		0x18
#define CHAR_MODE_PWR_ON		0x17
#define CHAR_MODE_PWR_OFF		0x13
#define GRAPHIC_MODE_PWR_ON		0x1F
#define GRAPHIC_MODE_PWR_OFF	0x1B
#define ENTRY_MODE				0x06
#define FUNCTION_SET			0x39

class WS0010
{	
	public:
		WS0010();
		void init();
		void print(char*);
		void moveCursor(uint8_t, uint8_t);
		void clear();

	private:
		void sendCmd(uint8_t);
		void sendData(uint8_t*, uint8_t);
		void send(uint8_t, uint8_t);
		uint8_t isBusy();
};

#endif