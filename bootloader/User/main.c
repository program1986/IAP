#include "stm32f10x.h"
#include "bsp_usart.h"
#include "delay.h"
#include "iap.h"
#include <stdio.h>
#include "stmflash.h"

#define FLASH_Running_APP_Flag       0x08075000
int main()
{
	u16 update_flag=0;
	u16 oldcount=0;				//�ϵĴ��ڽ�������ֵ
	u16 applenth=0;				//���յ���app���볤��
	update_flag = 0xAA;
	STMFLASH_Write(FLASH_Running_APP_Flag,&update_flag,1);
	update_flag =0xBB;
	update_flag=STMFLASH_ReadHalfWord(FLASH_Running_APP_Flag);
	update_flag =0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART1_Config();
	delay_init();
	while(1)
	{
		if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
			}else oldcount=USART_RX_CNT;			
		}
		delay_ms(10);

		if(applenth)
		{
			printf("��ʼ���¹̼�...\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//����FLASH���� 
				printf("�̼��������!\r\n");	
				update_flag=1;
				STMFLASH_Write(FLASH_Running_APP_Flag,&update_flag,1);
				applenth=0;
			}
			
		}

		if(update_flag)
		{
			printf("��ʼִ��FLASH�û�����!!\r\n");
			
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}  
		}		
	}
}

