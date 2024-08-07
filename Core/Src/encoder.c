#include "encoder.h"
#include "stm32f4xx_hal.h"

void TIM3_Encoder_init(void){
	//разрешаем тактирование таймера TIM3
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

       //конфигурируем выводы к которым подключается энкодер как входы с подтяжкой к питанию
	GPIOA->MODER &= ~(GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
	GPIOA->MODER |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7); 
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL7_1);	
	
         //настраиваем фильтр
  TIM3->CCMR1 |= TIM_CCMR1_IC1F | TIM_CCMR1_IC2F;
        //настраиваем мультиплексор
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
        //сигнал TIxFP1 появиться по возрастающему фронту
	TIM3->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
			  //счет по одному фронту 
	TIM3->SMCR |= TIM_SMCR_SMS_0;
         //максимальное значение счетчика 
	TIM3->ARR = 1000;
        //включаем счетчик
	TIM3->CR1 |= TIM_CR1_CEN ;
	//обнуляем счетный регистр
	TIM3->CNT = 0;
}
void TIM2_Encoder_init(void){
//разрешаем тактирование таймера TIM3
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

       //конфигурируем выводы к которым подключается энкодер как входы с подтяжкой к питанию
	GPIOA->MODER &= ~(GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0);
	GPIOA->MODER |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1);
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1);
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL0 | GPIO_AFRL_AFSEL1); 
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL0_0| GPIO_AFRL_AFSEL1_0);	
	
         //настраиваем фильтр
  TIM2->CCMR1 |= TIM_CCMR1_IC1F | TIM_CCMR1_IC2F;
        //настраиваем мультиплексор
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
        //сигнал TIxFP1 появиться по возрастающему фронту
	TIM2->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
			  //счет по одному фронту 
	TIM2->SMCR |= TIM_SMCR_SMS_0;
         //максимальное значение счетчика 
	TIM2->ARR = 1000;
        //включаем счетчик
	TIM2->CR1 |= TIM_CR1_CEN ;
	//обнуляем счетный регистр
	TIM2->CNT = 0;
}


void Encoder_init(void){
	TIM3_Encoder_init();
	TIM2_Encoder_init();
}
