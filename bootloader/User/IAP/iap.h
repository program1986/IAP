#ifndef _IAP_H
#define _IAP_H

#include "stm32f10x.h"

typedef void (*iapfun)(void);//定义一个函数类型的参数，函数指针，返回值和形参都为空

#define FLASH_APP1_ADDR		0x08010000 

void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize);
void iap_load_app(u32 appxaddr);

#endif

