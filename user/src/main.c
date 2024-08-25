#include <stdio.h>

#include "svc.h"

#include "delay.h"
#include "usart.h"

#include "led.h"

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

static void version_display(void)
{
    printf("ARMCC_VERSION: %d\r\n", __ARMCC_VERSION);
    printf("SCM_REV: %s\r\n", SCM_REV);
    printf("SCM_BRANCH: %s\r\n", SCM_BRANCH);
    printf("SCM_STEP: %d\r\n", SCM_STEP);
}

static void system_init(void)
{
    delay_init();
    
    usartx_init(USART1, 115200);
}

static void bsp_init(void)
{
    led_init();
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	__disable_irq();            /*!< 关闭全局中断 */
    system_init();
	__enable_irq();             /*!< 使能全局中断 */

	bsp_init();
	led_set_state(0);
    
    version_display();

	/* Infinite loop */
	while (1) {
		led_set_state(1);
		delay_ms(1000);
		led_set_state(0);
		delay_ms(1000);
	}
}
