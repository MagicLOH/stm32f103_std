#include "led.h"

#include "stm32f10x.h"
#include "sys.h"

void led_init(void)
{
    RCC_APB2PeriphClockCmd(LED0_PERIPH_CLK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode    = LED0_GPIO_MODE;
    GPIO_InitStructure.GPIO_Pin     = LED0_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed   = LED0_GPIO_SPEED;
    GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);
}

void led_set_state(uint8_t state)
{
    GPIO_WriteBit(LED0_GPIO_PORT, LED0_GPIO_PIN, (BitAction)state);
    // PBout(2) = state;
}


