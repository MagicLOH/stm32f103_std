#include "delay.h"
#include "stm32f10x.h"

static __IO uint32_t ticks_per_us = 0;
static __IO uint32_t ticks_per_ms = 0;

/**
 * @brief 延时功能初始化
 *
 * @param sysclk_mhz 芯片主频(Hz)
 */
void delay_init(uint32_t sysclk)
{
    // 将 SysTick 定时器的时钟源设置为 HCLK（系统时钟）的八分频
#if defined(USE_STDPERIPH_DRIVER)
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
#elif defined(USE_HAL_DRIVER)
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
#endif
    
    // 计算每微秒的滴答数
    ticks_per_us = sysclk / 8 / 1000000;
    // 计算每毫秒的滴答数
    ticks_per_ms = (uint32_t)ticks_per_us * 1000;
}

/**
 * @brief 微秒级延时函数
 * - 最长延时167772微秒
 * @param n_us 延时n微秒
 */
void delay_us(uint32_t n_us)
{
    uint32_t temp = 0;
    SysTick->LOAD = n_us * ticks_per_us;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //启动定时
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

/**
 * @brief 毫秒级延时函数（不对外）
 * - 最长延时167毫秒
 * @param n_ms 延时n毫秒(不能大于167毫秒)
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
 * @brief 毫秒级延时函数
 * - 由于滴答定时器计数器是24位的，所以延时不能大于167毫秒
 * - 所以对毫秒延时函数重新封装，可以任意时间的毫秒延时
 * - 延时超过一秒可以使用delay_s秒级延时函数
 * @param n_ms 延时的毫秒数
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
