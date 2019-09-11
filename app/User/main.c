#include "stm32f10x.h"
#include "bsp_led.h"
#include "stmflash.h"
#include "key.h"
#include "delay.h"
#define FLASH_Running_APP_Flag       0x08075000
int main()
{
	u8 key;
	u16 update_flag=0;
	SCB->VTOR = FLASH_BASE | 0x10000; //设置中断向量表起始地址
	LED_GPIO_Config();
	KEY_Init();
	delay_init();
	LED0_OFF;
	while(1)
	{
		key=KEY_Scan(0);
		LED0_ON;
		if(key==KEY0_PRES)
		{
			STMFLASH_Write(FLASH_Running_APP_Flag,&update_flag,1);
		}
	}
}

