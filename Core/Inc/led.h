#ifndef _LED_H_
#define	_LED_H_
#include "stm32f4xx_hal.h"
#include <stdbool.h>
void led(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void stat_led(uint8_t* state);
void mute_led(bool* mute);
void full_led(void);

#define MUTE_LED GPIOA,GPIO_PIN_15
#define OFF GPIO_PIN_RESET
#define ON GPIO_PIN_SET
#define PLAY_LED GPIOB,GPIO_PIN_9
#define PAUSE_LED GPIOB,GPIO_PIN_5
#define STOP_LED GPIOA,GPIO_PIN_10

#endif
