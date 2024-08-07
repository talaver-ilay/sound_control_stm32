#include "keyboard.h"
#include "tim.h"


void PressKey(uint8_t key, consumerHID* keystruct,USBD_HandleTypeDef* usbdev){
	keystruct->KEYCODE = key;
	USBD_CUSTOM_HID_SendReport(usbdev, keystruct, 2);// sizeof (keystruct)
	delay_ms(50);
}

void PressKeyboard(uint8_t key, keyboardHID* keystruct,USBD_HandleTypeDef* usbdev){
	keystruct->KEYCODE1 = key;
	USBD_CUSTOM_HID_SendReport(usbdev, keystruct, 9);// sizeof (keystruct)
	delay_ms(50);
}

void ReleaseKey(consumerHID* keystruct,USBD_HandleTypeDef* usbdev){
	keystruct->KEYCODE = 0;
	USBD_CUSTOM_HID_SendReport(usbdev, keystruct, 2);
	delay_ms(10);
}
void ReleaseKeyboard(keyboardHID* keystruct,USBD_HandleTypeDef* usbdev){
	keystruct->KEYCODE1 = 0;
	USBD_CUSTOM_HID_SendReport(usbdev, keystruct, 9);
	delay_ms(10);
}