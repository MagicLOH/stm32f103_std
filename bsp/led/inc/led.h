#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>

#define LED_TURNON  1U
#define LED_TURNOFF 0U

#define LED0_PERIPH_CLK     RCC_APB2Periph_GPIOB
#define LED0_GPIO_PORT      GPIOB
#define LED0_GPIO_PIN       GPIO_Pin_2
#define LED0_GPIO_MODE      GPIO_Mode_Out_PP
#define LED0_GPIO_SPEED     GPIO_Speed_10MHz

void led_init(void);

void led_set_state(uint8_t state);

#endif // _LED_H_
