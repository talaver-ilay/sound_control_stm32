/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device Custom HID interface file.
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
#include "usbd_custom_hid_if.h"
#include <stdbool.h>
#include "oled.h"
/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */
uint8_t getpacket[USBD_CUSTOMHID_OUTREPORT_BUF_SIZE] = {0};
extern media_info win_media;
uint8_t volume_packet = 0;
bool title_flag = false;
bool volume_flag = false;
bool state_flag = false;
bool prog_name_flag = false;
bool connect_state = false;
bool connect_flag = false;
/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{   
	/*обязательно сначало Vendor Defined потом Consumer*/
		0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined) 
		0x09, 0x01,                    // USAGE (Vendor Usage 1)
		0xa1, 0x01,                    // COLLECTION (Application)
		0x85, 0x03,        						 // Report ID (3)
		0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined)
		0x15, 0x00,                    // LOGICAL_MINIMUM (0)
		0x26, 0xff, 0x00,              // LOGICAL_MAXIMUM (255)
		0x75, 0x08,                    // REPORT_SIZE (8)
		0x95, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE,                    // REPORT_COUNT (USBD_CUSTOMHID_OUTREPORT_BUF_SIZE)
		0x09, 0x00,                    // USAGE (Undefined)
		0x81, 0x82,                    // INPUT (Data,Var,Abs,Vol)
		0x09, 0x00,                    // USAGE (Undefined)
		0x91, 0x82,                    // OUTPUT (Data,Var,Abs,Vol)
		0xc0,                           // END_COLLECTION
		
		0x05, 0x0C,        						 // Usage Page (Consumer)
		0x09, 0x01,        						 // Usage (Consumer Control)
		0xA1, 0x01,        						 // Collection (Application)
		0x85, 0x02,        							//   Report ID (2)
		0x05, 0x0C,        							//   Usage Page (Consumer)
		0x15, 0x00,        							//   Logical Minimum (0)
		0x25, 0x01,        							//   Logical Maximum (1)
		0x75, 0x01,        							//   Report Size (1)
		0x95, 0x08,        							//   Report Count (8)
		0x09, 0xB5,        							//   Usage (Scan Next Track)                //0000 0001
		0x09, 0xB6,        							//   Usage (Scan Previous Track)            //0000 0010
		0x09, 0xB7,        							//   Usage (Stop)                           //0000 0100
		0x09, 0xB8,        							//   Usage (Eject)                          //0000 1000
		0x09, 0xCD,        							//   Usage (Play/Pause)                     //0001 0000
		0x09, 0xE2,        							//   Usage (Mute)                           //0010 0000
		0x09, 0xE9,        							//   Usage (Volume Increment)								//0100 0000
		0x09, 0xEA,        							//   Usage (Volume Decrement)               //1000 0000
		0x81, 0x02,        							//   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0xC0,              						  // End Collection
		
		0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
		0x85, 0x04,        						 //   Report ID (4)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0xE7,                    //   LOGICAL_MAXIMUM (231)										//0x65 default keyboard
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0xE7,                    //   USAGE_MAXIMUM (Keyboard Application)		//0x65 default keyboard
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xC0,    											// End Collection  
	/* USER CODE END 0 */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  UNUSED(event_idx);
  UNUSED(state);

  /* Start next USB packet transfer once data processing is completed */
  if (USBD_CUSTOM_HID_ReceivePacket(&hUsbDeviceFS) != (uint8_t)USBD_OK)
  {
    return -1;
  }
	for (int i = 0; i < USBD_CUSTOMHID_OUTREPORT_BUF_SIZE; ++i){
		getpacket[i] = ((uint8_t *)(hUsbDeviceFS.pClassData))[i];// запись полученых USBD_CUSTOMHID_OUTREPORT_BUF_SIZE байт 	
  }
	switch(getpacket[1]){ // 
		case 0x01: volume_flag = true;
							 volume_packet = getpacket[2]; 
		break; // громкость
		case 0x02: state_flag = true;                                     // состояние воспроизведения
			switch (getpacket[2]){case sPLAY:win_media.state = sPLAY;break;																		
														case sPAUSE:win_media.state = sPAUSE;break;				
														case sSTOP:win_media.state = sSTOP;break;}				
		break;
		case 0x03:	title_flag = true;
		break;													 // название песни 
		case 0x04:  prog_name_flag = true;																		 // нзвание плеера
								switch(getpacket[2]){case 0x00:win_media.Name = pSystem;break;
																		 case 0x01:win_media.Name = pWinamp;break;
																		 case 0x02:win_media.Name = pAimp;break;}
		break;
		case 0x05: volume_flag = true;
							 switch (getpacket[2]){case 0x01:win_media.mute = sMUTE;break;
																		 case 0x00:win_media.mute = sUNMUTE;break;}		
		break;
		case 0x06:connect_flag = true;
				switch (getpacket[2]){case 0x00: connect_state = false;break;
															case 0x01: connect_state = true;break;}
		break;
	}	
  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

