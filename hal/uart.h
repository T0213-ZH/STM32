#ifndef __UART_H
#define __UART_H

extern void uart_config(void);
extern void uart_send_ch(unsigned char ch);
extern void uart_send_str(unsigned char *p, unsigned int len);
extern void uart_read_recv_len(void);
extern void uart_recv_process(void);

#endif
