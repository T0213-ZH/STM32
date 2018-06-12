/*
* AHB_CLK = 180MHz
* APB1 = 45MHz
* APB2 = 90MHz
* TCLK = 90MHz  ; if TIM_ClockDivision == 1  
						TCLK = APB1_CLK * 1 
                  else 
						TCLK = APB1_CLK * 2 
* ÖĞ¶ÏÖÜÆÚ 1MHz  = TCLK / (TIM_Prescaler + 1); TIM_Prescaler = 89
*/

#include "stm32f4xx.h"

#define TIM_PRESCALER   (89)
#define TIM3_4_PERIOD   (0xFFFF)
#define TIM2_5_PERIOD   (0xFFFFFFFF)
#define TIM_PERIOD      TIM2_5_PERIOD //32bit: TIM2 and TIM5 ; 16bit: TIM3 and TIM4

unsigned int oc_val = 1000;

void timer_config(void){

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = TIM_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
		
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = oc_val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
}

void timer_set_oc(unsigned int oc){
	unsigned int val;
	
	if(!oc )
		oc = oc_val;
	else
		oc_val = oc;
	
	val = TIM_GetCapture1(TIM2);
	TIM_SetCompare1(TIM2, val + oc);
	
	//for test
//	static unsigned int cnt = 0;
//	if(++cnt > 1000){
//		cnt = 0;
//		printf("interrupt\r\n");
//	}
}

