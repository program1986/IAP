#include "bsp_usart.h"
#include "stm32f10x_usart.h"
#include <stdio.h>


void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}



void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


void Usart_SendString(USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k = 0;
    do{
        Usart_SendByte(pUSARTx, *(str + k));
        k++;
    }
    while(*(str + k)!= '\0');
    
    while(USART_GetFlagStatus( pUSARTx, USART_FLAG_TC)==RESET){
    }
}


u8 USART_RX_BUF[USART_REC_LEN]  __attribute__ ((at(0X20001000)));//接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_CNT=0;			//接收的字节数	
void DEBUG_USART_IRQHandler(void)
{
	u8 res;
	if(USART1->SR&(1<<5))//接收到数据
	{	 
		res=USART1->DR; 
		if(USART_RX_CNT<USART_REC_LEN)
		{
			USART_RX_BUF[USART_RX_CNT]=res;
			USART_RX_CNT++;			 									     
		}
	}
}



//重定向C库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    USART_SendData(DEBUG_USARTx, (uint8_t)ch);
    
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
    
    return(ch);
}

int fgetc(FILE *f)
{
     while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(DEBUG_USARTx);
}

