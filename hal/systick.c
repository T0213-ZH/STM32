#include "stm32f4xx.h"
#include "header.h"

#define SYSTICK_PER_CNT (128)
static unsigned long long g_systick_count = 0;

void systick_config(void){

	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency / SYSTICK_PER_CNT );
}

void timing_delay_decrement(void){

	if(++g_systick_count == SYSTICK_PER_CNT){
		g_systick_count = 0;		
	}
}


//unsigned char temp[]="uart send ok\r\n";
void systick_process(void){
		static unsigned char systick_flag = 0;
		
		if(!g_systick_count){
			if(!systick_flag){
				//uart_send_str(temp, 14);
				gpio_toggle();
				systick_flag = 1;
			}
		}else{
			if(systick_flag) systick_flag = 0;
		}
}
