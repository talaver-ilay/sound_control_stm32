#ifndef WS2812_H_

#define WS2812_H_

//--------------------------------------------------

#include "stm32f4xx_hal.h"

#include <string.h>

#include <stdlib.h>

#include <stdint.h>

//--------------------------------------------------
#define DELAY_LEN 50
#define LED_COUNT 4 // количества управляемых светодиодов
#define ARRAY_LEN DELAY_LEN + LED_COUNT*24
#define HIGH 76//3665
#define LOW 29//1526
#define BitIsSet(reg, bit) ((reg & (1<<bit)) != 0)
void ws2812_pixel_rgb_to_buf_dma(uint8_t Rpixel , uint8_t Gpixel, uint8_t Bpixel, uint16_t posX);
void ws2812_init(void);

//--------------------------------------------------

#endif /* WS2812_H_ */
