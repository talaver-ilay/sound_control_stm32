#ifndef _OLED_H_
#define	_OLED_H_
#include "stm32f4xx_hal.h"
#include "stdbool.h"

typedef enum{
	sPLAY = 0x01,
	sPAUSE = 0x02,
	sSTOP = 0x03,
	sMUTE = 0x01,
	sUNMUTE = 0x00,
}media_state;

void volume_print(bool* flag, char* volume, uint8_t* packet, bool* mute);
void title_print(bool* flag, uint8_t* packet, uint8_t* title);
void state_print(bool* flag, media_state state);
void logo_print(void);
void oled_update(void);
void oled_init(void);
void volume_mute(void);
void clean_oled(void);
#endif
