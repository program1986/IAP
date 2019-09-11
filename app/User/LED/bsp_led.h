#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "stm32f10x.h"

#define LED0_APBxClock_FUN  RCC_APB2PeriphClockCmd
#define LED0_CLK            RCC_APB2Periph_GPIOB

#define LED0_GPIO_PORT      GPIOB
#define LED0_GPIO_PIN       GPIO_Pin_5 


#define digitalHi(p,i)      {p->BSRR=i;}
#define digitalLo(p,i)      {p->BRR=i;}
#define digitalToggle(p,i)  {p->ODR ^=i;}  //“ÏªÚ

#define LED0_TOOGLE        digitalToggle(LED0_GPIO_PORT,LED0_GPIO_PIN)
#define LED0_OFF           digitalHi(LED0_GPIO_PORT,LED0_GPIO_PIN)
#define LED0_ON            digitalLo(LED0_GPIO_PORT,LED0_GPIO_PIN)

void LED_GPIO_Config(void);

#endif


