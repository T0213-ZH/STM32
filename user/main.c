#include "stm32f4xx.h"
#include "../hal/header.h"

#include <stdio.h>

//#include "../os/include/queue.h"
//void list_test(void);
//void queue_test(void);
void mempool_test(void);

int main(void){
	
	systick_config();
	uart_config();	
	gpio_config();
	timer_config();
	
	printf("printf func ok\r\n");
	
//	queue_test();
	mempool_test();
	
	while(1){
	
		systick_process();		
		uart_recv_process();
	}
}

int fputc(int ch, FILE *f){
	uart_send_ch((unsigned char)ch);
	return ch;
}

#include "../os/include/os_mempool.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct os_mempool event_pool;
static os_membuf_t os_event_buf[
	OS_MEMPOOL_SIZE(5, 10)
];

void mempool_test(void){

	int rc;
	void *p = NULL, *q = NULL;
	
	rc = os_mempool_init(&event_pool, 5, 10, os_event_buf, "name_event_pol");
	
	p = os_memblock_get(&event_pool);
	
	memset(p, 0x55, 10);
	
	q = os_memblock_get(&event_pool);
	
	memset(q, 0xAA, 10);
	
	os_memblock_put(&event_pool, p);
	os_memblock_put(&event_pool, q);
}

