#include <stdio.h>

#include "delay.h"
#include "usart.h"

#include "led.h"

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	// USART_SendData(EVAL_COM1, (uint8_t)ch);

	/* Loop until the end of transmission */
	// while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);

	return ch;
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1) 
    {
	}
}
#endif /* USE_FULL_ASSERT */

static void system_clock_init(void)
{
	RCC_ClocksTypeDef rcc_clock;
	RCC_GetClocksFreq(&rcc_clock);
	delay_init(rcc_clock.SYSCLK_Frequency);
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	__disable_irq();            /*!< 关闭全局中断 */

	system_clock_init();

	usart1_init(115200);

	__enable_irq();             /*!< 使能全局中断 */

	led_init();
	led_set_state(LED_TURNOFF);

	/* Infinite loop */
	while (1) {
		led_set_state(LED_TURNON);
		delay_ms(1000);
		led_set_state(LED_TURNOFF);
		delay_ms(1000);
	}
}
