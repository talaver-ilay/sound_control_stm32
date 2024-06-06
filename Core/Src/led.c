#include "led.h"
#include "oled.h"
void led(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState){
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin, PinState);
}
void stat_led(uint8_t* state){
	switch(*state){
			case sPLAY:
				led(PLAY_LED,ON);
				led(STOP_LED,OFF);
				led(PAUSE_LED,OFF);
			break;
			case sPAUSE:
				led(PLAY_LED,OFF);
				led(STOP_LED,OFF);
				led(PAUSE_LED,ON);
			break;
			case sSTOP:
				led(PLAY_LED,OFF);
				led(STOP_LED,OFF);
				led(PAUSE_LED,ON);
			break;}
}
void mute_led(bool* mute){
	if(*mute)	led(MUTE_LED,ON);
	else led(MUTE_LED,OFF);
}
void full_led(void){
	led(PLAY_LED,ON);
	led(STOP_LED,ON);
	led(PAUSE_LED,ON);
}