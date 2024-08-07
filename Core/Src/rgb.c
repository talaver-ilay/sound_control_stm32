#include "rgb.h"
#include "ws2812.h"

extern uint16_t BUF_DMA [ARRAY_LEN];
extern TIM_HandleTypeDef htim4;
void rgb_init(void){
	ws2812_init();}
void rgb_red(uint8_t* count){
			for(uint8_t i = 0;i<4;i++) ws2812_pixel_rgb_to_buf_dma(*count, 0, 0, i);
			HAL_TIM_PWM_Start_DMA(&htim4,TIM_CHANNEL_3,(uint32_t*)&BUF_DMA,((ARRAY_LEN)*2)-4 );		
}
void rgb_green(uint8_t* count){
			for(uint8_t i = 0;i<4;i++) ws2812_pixel_rgb_to_buf_dma(0, *count, 0, i);
			HAL_TIM_PWM_Start_DMA(&htim4,TIM_CHANNEL_3,(uint32_t*)&BUF_DMA,((ARRAY_LEN)*2)-4 );		
}
void rgb_blue(uint8_t* count){
			for(uint8_t i = 0;i<4;i++) ws2812_pixel_rgb_to_buf_dma(0, 0, *count, i);
			HAL_TIM_PWM_Start_DMA(&htim4,TIM_CHANNEL_3,(uint32_t*)&BUF_DMA,((ARRAY_LEN)*2)-4 );		
}
void rgb_wight(uint8_t* count){
			for(uint8_t i = 0;i<4;i++) ws2812_pixel_rgb_to_buf_dma(*count, *count, *count, i);
			HAL_TIM_PWM_Start_DMA(&htim4,TIM_CHANNEL_3,(uint32_t*)&BUF_DMA,((ARRAY_LEN)*2)-4 );		
}
void rgb_orange(uint8_t* count){
			for(uint8_t i = 0;i<4;i++) ws2812_pixel_rgb_to_buf_dma(*count, *count/4, 0, i);
			HAL_TIM_PWM_Start_DMA(&htim4,TIM_CHANNEL_3,(uint32_t*)&BUF_DMA,((ARRAY_LEN)*2)-4 );		
}
void rgb_violet(uint8_t* count){
			for(uint8_t i = 0;i<4;i++) ws2812_pixel_rgb_to_buf_dma(*count, 0, *count, i);
			HAL_TIM_PWM_Start_DMA(&htim4,TIM_CHANNEL_3,(uint32_t*)&BUF_DMA,((ARRAY_LEN)*2)-4 );		
}
void rgb_cyan(uint8_t* count){
			for(uint8_t i = 0;i<4;i++) ws2812_pixel_rgb_to_buf_dma(0, *count, *count, i);
			HAL_TIM_PWM_Start_DMA(&htim4,TIM_CHANNEL_3,(uint32_t*)&BUF_DMA,((ARRAY_LEN)*2)-4 );		
}