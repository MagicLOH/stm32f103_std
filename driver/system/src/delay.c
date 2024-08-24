#include "delay.h"
#include "stm32f10x.h"

static __IO uint32_t ticks_per_us = 0;
static __IO uint32_t ticks_per_ms = 0;

/**
 * @brief ��ʱ���ܳ�ʼ��
 *
 * @param sysclk_mhz оƬ��Ƶ(Hz)
 */
void delay_init(uint32_t sysclk)
{
    // �� SysTick ��ʱ����ʱ��Դ����Ϊ HCLK��ϵͳʱ�ӣ��İ˷�Ƶ
#if defined(USE_STDPERIPH_DRIVER)
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
#elif defined(USE_HAL_DRIVER)
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
#endif
    
    // ����ÿ΢��ĵδ���
    ticks_per_us = sysclk / 8 / 1000000;
    // ����ÿ����ĵδ���
    ticks_per_ms = (uint32_t)ticks_per_us * 1000;
}

/**
 * @brief ΢�뼶��ʱ����
 * - ���ʱ167772΢��
 * @param n_us ��ʱn΢��
 */
void delay_us(uint32_t n_us)
{
    uint32_t temp = 0;
    SysTick->LOAD = n_us * ticks_per_us;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //������ʱ
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

/**
 * @brief ���뼶��ʱ�����������⣩
 * - ���ʱ167����
 * @param n_ms ��ʱn����(���ܴ���167����)
 */
static void delay_xms(uint32_t n_ms)
{
    uint32_t temp = 0;
    SysTick->LOAD = (uint32_t)n_ms * ticks_per_ms;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0X00;
}

/**
 * @brief ���뼶��ʱ����
 * - ���ڵδ�ʱ����������24λ�ģ�������ʱ���ܴ���167����
 * - ���ԶԺ�����ʱ�������·�װ����������ʱ��ĺ�����ʱ
 * - ��ʱ����һ�����ʹ��delay_s�뼶��ʱ����
 * @param n_ms ��ʱ�ĺ�����
 */
void delay_ms(uint32_t n_ms)
{
    uint32_t repeat = n_ms / 167;
    uint32_t remain = n_ms % 167;

    while (repeat) {
        delay_xms(167);
        repeat--;
    }
    if (remain) {
        delay_xms(remain);
    }
}
