#include "stm32f4xx.h"

void gpio_config(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void gpio_write(int val){

	GPIO_WriteBit(GPIOG, GPIO_Pin_13, (val != 0) ? Bit_SET: Bit_RESET);
}

void gpio_toggle(void){

	GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
}
