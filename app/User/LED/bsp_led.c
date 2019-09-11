#include "bsp_led.h"

void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    LED0_APBxClock_FUN(LED0_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(LED0_GPIO_PORT,&GPIO_InitStructure);
    
    GPIO_SetBits(LED0_GPIO_PORT, LED0_GPIO_PIN);
}



