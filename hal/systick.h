#ifndef __SYSTICK_H
#define __SYSTICK_H

extern void systick_config(void);
extern void timing_delay_decrement(void);
extern void systick_process(void);

#endif
