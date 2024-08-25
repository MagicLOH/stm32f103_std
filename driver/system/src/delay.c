/**
 * @brief bit[23:0] 0xFFFFFF = 16,777,215?
 *
 * SysTick->CTRL 
 * bit[16]R COUNTFLAG ������ϴζ�ȡ���Ĵ�����SysTick �Ѿ��Ƶ�
 *                    �� 0�����λΪ 1�������ȡ��λ����λ���Զ�����
 * bit[ 2]R/W CLKSOURCE 0=�ⲿʱ��Դ(STCLK) 
 *                      1=�ں�ʱ��(FCLK)
 * bit[ 1]R/W TICKINT   0=���� 0 ʱ�޶���
 *                      1=SysTick ���������� 0 ʱ���� SysTick �쳣����
 * bit[ 0]R/W ENABLE    SysTick ��ʱ����ʹ��λ
 *
 * SysTick->LOAD
 * bit[23:0]R/W RELOAD ��������������ʱ��������װ�ص�ֵ
 *
 * SysTick->VAL
 * bit[23:0]R/Wc CURRENT ��ȡʱ���ص�ǰ��������ֵ��д����ʹ֮���㣬
 *                       ͬʱ���������SysTick���Ƽ�״̬�Ĵ����е�
 *                       COUNTFLAG ��־
 * Systick->CALIB
 * bit[31]R NOREF 0=�ⲿ�ο�ʱ�ӿ���
 *                1=û���ⲿ�ο�ʱ�ӣ�STCLK �����ã�
 * bit[30]R SKEW  0=У׼ֵ��׼ȷ�� 10ms
 *                1=У׼ֵ����׼ȷ�� 10ms
 * bit[23:0]R/W TENMS �� 10ms �ļ���е������ĸ�����оƬ�����Ӧ
 *                    ��ͨ�� Cortex-M3 �������ź��ṩ����ֵ������
 *                    ֵ�����㣬���ʾ�޷�ʹ��У׼����
 */

#include "delay.h"
#include "stm32f10x.h"
//#include "stm32f4xx.h"

static volatile uint32_t ticks_per_us;
static volatile uint32_t ticks_per_ms;

static void systick_tick(volatile uint32_t ticks)
{
    SysTick->LOAD = ticks;
    SysTick->VAL = 0UL;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    /* systick ������Ϊ0ʱ COUNTFLAGλ��һ */
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0UL; // ���õ���ʱֵ ��� COUNTFLAG λ
}

static void delay(volatile uint32_t ticks)
{
    uint32_t repeat = ticks / SysTick_LOAD_RELOAD_Msk;
    uint32_t remain = ticks % SysTick_LOAD_RELOAD_Msk;
    
    if (ticks <= SysTick_LOAD_RELOAD_Msk) {
        systick_tick(ticks);
    } else {
        while (repeat) {
            systick_tick(SysTick_LOAD_RELOAD_Msk);
            repeat--;
        }
        if (remain) {
            systick_tick(remain);
        }
    }
}

/**
 * @brief ΢�뼶��ʱ����
 *
 * @param n_us ��ʱn΢��
 * @retval none
 */
void delay_us(volatile uint32_t n_us)
{
    if (!ticks_per_us)
        return;
    delay((n_us * ticks_per_us) - 1UL);
}

/**
 * @brief ���뼶��ʱ����
 *
 * @param n_ms ��ʱn����
 * @retval none
 */
void delay_ms(volatile uint32_t n_ms)
{
    if (!ticks_per_ms)
        return;
    delay((n_ms * ticks_per_ms) - 1UL);
}

/**
 * @brief system delay function init
 *
 * @retval none
 */
void delay_init(void)
{
    /* SysTick ClockSource = HCLK / 8 */
    SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
    
    ticks_per_us = SystemCoreClock / 8UL / 1000000UL;
    ticks_per_ms = ticks_per_us * 1000UL;
}

