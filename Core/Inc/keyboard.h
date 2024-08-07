#ifndef _KEYBOARD_H_
#define	_KEYBOARD_H_
#include "stm32f4xx_hal.h"
#include "usb_device.h"

typedef struct{
	uint8_t ReportID;
	uint8_t KEYCODE;
}consumerHID;
typedef struct{
	uint8_t ReportID;
	uint8_t MODIFIER;
	uint8_t RESERVED;
	uint8_t KEYCODE1;
	uint8_t KEYCODE2;
	uint8_t KEYCODE3;
	uint8_t KEYCODE4;
	uint8_t KEYCODE5;
	uint8_t KEYCODE6;
}keyboardHID;

void PressKey(uint8_t key, consumerHID* keystruct, USBD_HandleTypeDef* usbdev);
void PressKeyboard(uint8_t key, keyboardHID* keystruct,USBD_HandleTypeDef* usbdev);
void ReleaseKey(consumerHID* keystruct,USBD_HandleTypeDef* usbdev);
void ReleaseKeyboard(keyboardHID* keystruct,USBD_HandleTypeDef* usbdev);

#endif
