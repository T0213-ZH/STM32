#include "stm32f4xx.h"

#define TX_BUF_MAX  (125)
#define RX_BUF_MAX  (25)
uint8_t tx_buf[TX_BUF_MAX];
uint8_t rx_buf[RX_BUF_MAX];

uint8_t rx_head = 0;
uint8_t rx_tail = 0;

void uart_config(void){
	
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,   ENABLE);
	
	
	//GPIO Configure
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	

    //USART Configure	
	USART_OverSampling8Cmd(USART1, ENABLE);
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
	
	USART_ClearFlag(USART1, USART_FLAG_TC); //×¢£º±ÜÃâÊ××Ö·û¶ªµôÏÖÏó
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//	
//	//NVIC Configure
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	
	//DMA Configures	
	DMA_DeInit(DMA2_Stream7);
	while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)tx_buf;
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_BufferSize = TX_BUF_MAX;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream7, &DMA_InitStruct);

	//DMA NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/*Configure RX DMA*/
	DMA_DeInit(DMA2_Stream5);
	while(DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
	DMA_InitStruct.DMA_BufferSize = RX_BUF_MAX;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)rx_buf;
	DMA_Init(DMA2_Stream5, &DMA_InitStruct);
	DMA_Cmd(DMA2_Stream5, ENABLE);
	
	//DMA NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
}

void uart_send_ch(unsigned char ch){

	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void uart_send_str(unsigned char *p, uint16_t len){

	uint16_t i;
	for(i=0; i<len; i++){
		tx_buf[i] = *p++;
	}
	
	while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}
	DMA_SetCurrDataCounter(DMA2_Stream7, len);
	DMA_Cmd(DMA2_Stream7, ENABLE);
}


void uart_read_recv_len(void){

	rx_head = RX_BUF_MAX - DMA_GetCurrDataCounter(DMA2_Stream5);
}

void uart_recv_process(void){

	if(rx_tail != rx_head){
		uint8_t i = 0;
		uint8_t buf[RX_BUF_MAX];
		do{
			buf[i++] = rx_buf[rx_tail++];
			if(rx_tail == RX_BUF_MAX)
				rx_tail = 0;
		}while(rx_tail != rx_head);
		
		uart_send_str(buf, i);
	}
}
