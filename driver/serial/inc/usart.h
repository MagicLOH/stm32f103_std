#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x_usart.h"

int usartx_init(USART_TypeDef *usartx, const uint32_t baudrate);

#endif // _USART_H_

