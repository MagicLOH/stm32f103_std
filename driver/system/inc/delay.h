#ifndef _DELAY_H_
#define _DELAY_H_

#include <stdint.h>

void delay_init(uint32_t sysclk);

void delay_us(uint32_t n_us);

void delay_ms(uint32_t n_ms);

#endif /* _DELAY_H_ */
