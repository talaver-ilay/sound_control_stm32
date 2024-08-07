/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "encoder.h"
#include "gpio.h"
#include "i2c.h"
#include "dma.h"
#include "tim.h"
#include "rgb.h"
#include "led.h"
#include "keyboard.h"
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
I2C_HandleTypeDef hi2c1;
enum {UP = 0,DOWN = 0x10}direction;
enum {wPLAY = 0x1B,	   // X		Play/Restart/Unpause
			wNEXT = 0x05,    // B		Next Track
			wPREVIOUS = 0x1D,// Z		Previous Track
			wSTOP = 0x19,		 // V		Stop
			wPAUSE = 0x06,   // C		Pause/Unpause
	}winampKey;		   // кнопки клавиатуры для управления Winamp плеером
		 
extern uint8_t volume_packet;
extern bool volume_flag;
extern bool state_flag;
extern bool title_flag;
extern bool prog_name_flag;
extern bool time9_flag;
extern const uint8_t char_width[];
extern TIM_HandleTypeDef htim9;
extern uint8_t getpacket[USBD_CUSTOMHID_OUTREPORT_BUF_SIZE];
extern bool connect_state;
extern bool connect_flag;
	
media_info win_media = {0};
uint8_t count = 4;
uint16_t count_color = 0;
bool rgb_way = false;

extern USBD_HandleTypeDef hUsbDeviceFS;
consumerHID consumer = {2,0};
keyboardHID keyboardhid = {4,0,0,0,0,0,0,0,0};
uint16_t newEncoderValue = 0;
uint16_t oldEncoderValue_2 = 0;
uint16_t oldEncoderValue_3 = 0;
uint8_t KeyState = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void encoder(uint16_t newEncoderValue, uint16_t* oldEncoderValue, uint8_t num, uint32_t CR,progName Name,keyboardHID* key);
void press_key(uint8_t* Key,progName Name,keyboardHID* key);
void oled_print(void);
void program_connect(void);
void rgb_pulse(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	 
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
		
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
	MX_I2C1_Init(&hi2c1);
	Encoder_init();
	oled_init();
	MX_DMA_Init();
  MX_TIM4_Init();
	MX_TIM9_Init();
	rgb_init();
	logo_print();
	HAL_TIM_Base_Start_IT(&htim9);
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {	
		encoder(TIM2->CNT/2,&oldEncoderValue_2,2,TIM2->CR1,win_media.Name,&keyboardhid);
		encoder(TIM3->CNT/2,&oldEncoderValue_3,3,TIM3->CR1,win_media.Name,&keyboardhid);
		press_key(&KeyState,win_media.Name,&keyboardhid);
		rgb_pulse();
		program_connect();
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }		
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}
/* USER CODE BEGIN 4 */

///////////////////////////////////////////////////////////////////////////////////
void encoder(uint16_t newEncoderValue, uint16_t* oldEncoderValue, uint8_t num, uint32_t CR,progName Name,keyboardHID* key){
	switch(num){
		case 2:if(newEncoderValue != *oldEncoderValue){
						direction =  CR & 0x10;
						if(direction) {
								PressKey(DECREMENT, &consumer,&hUsbDeviceFS);
								ReleaseKey(&consumer,&hUsbDeviceFS);
						}
						else {
							PressKey(INCREMENT, &consumer,&hUsbDeviceFS);
							ReleaseKey(&consumer,&hUsbDeviceFS);							
						}
						*oldEncoderValue = newEncoderValue;}
		break;
		case 3:if(newEncoderValue != *oldEncoderValue){
						direction =  CR & 0x10;
						if(direction) {
								if(Name != pWinamp){PressKey(NEXT,&consumer,&hUsbDeviceFS);
																		ReleaseKey(&consumer,&hUsbDeviceFS);}
								else{PressKeyboard(wNEXT,key,&hUsbDeviceFS);																	// управление Winamp NEXT
										 ReleaseKeyboard(key,&hUsbDeviceFS);}
						}
						else {
								if(Name != pWinamp){PressKey(PREVIOUS,&consumer,&hUsbDeviceFS);
																		ReleaseKey(&consumer,&hUsbDeviceFS);}
								else{PressKeyboard(wPREVIOUS,key,&hUsbDeviceFS);															// управление Winamp PREVIOUS
										 ReleaseKeyboard(key,&hUsbDeviceFS);}
						}
						*oldEncoderValue = newEncoderValue;}
						delay_ms(50);
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////
void press_key(uint8_t* Key,progName Name,keyboardHID* key){
			switch(*Key){
			case PLAY: if(Name != pWinamp){PressKey(PLAY,&consumer,&hUsbDeviceFS);
																		 ReleaseKey(&consumer,&hUsbDeviceFS);}
								 else{PressKeyboard(wPLAY,key,&hUsbDeviceFS);																	// управление Winamp PLAY
									    ReleaseKeyboard(key,&hUsbDeviceFS);}
			break;
			case MUTE:	PressKey(MUTE,&consumer,&hUsbDeviceFS);
									ReleaseKey(&consumer,&hUsbDeviceFS);
			break;
			case PREVIOUS:	if(Name != pWinamp){PressKey(PREVIOUS,&consumer,&hUsbDeviceFS);
																					ReleaseKey(&consumer,&hUsbDeviceFS);}
											else{PressKeyboard(wPREVIOUS,key,&hUsbDeviceFS);                        // управление Winamp PREVIOUS
												   ReleaseKeyboard(key,&hUsbDeviceFS);}
			break;
			case NEXT:	if(Name != pWinamp){PressKey(NEXT,&consumer,&hUsbDeviceFS);
																			ReleaseKey(&consumer,&hUsbDeviceFS);}
									else{PressKeyboard(wNEXT,key,&hUsbDeviceFS);																// управление Winamp NEXT
											 ReleaseKeyboard(key,&hUsbDeviceFS);}
			break;	
			case STOP: if(Name != pWinamp){PressKey(STOP,&consumer,&hUsbDeviceFS);
																		 ReleaseKey(&consumer,&hUsbDeviceFS);}
								 else{PressKeyboard(wSTOP,key,&hUsbDeviceFS);																	// управление Winamp STOP
											ReleaseKeyboard(key,&hUsbDeviceFS);}
			break;
			case PAUSE: if(Name != pWinamp){PressKey(PLAY,&consumer,&hUsbDeviceFS);
																		 ReleaseKey(&consumer,&hUsbDeviceFS);}
								 else{PressKeyboard(wPAUSE,key,&hUsbDeviceFS);																// управление Winamp PAUSE
											ReleaseKeyboard(key,&hUsbDeviceFS);}
			break;
								 
			case DECREMENT: PressKey(DECREMENT,&consumer,&hUsbDeviceFS);		
			break;
		  case INCREMENT: PressKey(INCREMENT,&consumer,&hUsbDeviceFS);						
			break;	
		}	
		*Key = 0;
}
///////////////////////////////////////////////////////////////////////////////////
void oled_print(void){
			volume_print(&volume_flag, win_media.volume, &volume_packet, &win_media.mute);
			state_print(&state_flag,win_media.state);
			title_print(&title_flag, getpacket, win_media.title);
			oled_update();
}
///////////////////////////////////////////////////////////////////////////////////
void rgb_pulse(void){
	if(time9_flag){	
			if(count == 128)  rgb_way = true;
			else if(!count) rgb_way = false;
			
			if(count_color<64) 														  rgb_red(&count);
			else if(count_color > 63 && count_color <=127)  rgb_violet(&count);
			else if(count_color > 127 && count_color <=191) rgb_blue(&count);
			else if(count_color > 191 && count_color <=255) rgb_cyan(&count);
			else if(count_color > 255 && count_color <=319) rgb_green(&count);
			else if(count_color > 319 && count_color <=383) rgb_cyan(&count);
			else if(count_color > 383 && count_color <=447) rgb_wight(&count);
//		rgb_orange(&count);
		
			if(!rgb_way)    count+=4; 
			else if(rgb_way)count-=4;
		
			count_color++;
			if(count_color > 447) count_color = 0; 
			time9_flag = false;
		}
}
///////////////////////////////////////////////////////////////////////////////////
void program_connect(void){
		if(connect_state){
			oled_print();
			stat_led(&win_media.state);
			mute_led(&win_media.mute);
		}else if(!connect_state && connect_flag){
			full_led();
			clean_oled();
			logo_print();
			connect_flag = false;
		}
}
///////////////////////////////////////////////////////////////////////////////////
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
