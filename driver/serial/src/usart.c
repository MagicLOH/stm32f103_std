#include <stdio.h>
#include "usart.h"

#include "stm32f10x_gpio.h"

#if !defined(__MICROLIB)  

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm (".global __use_no_semihosting\n\t");

FILE __stdout;

/* __use_no_semihosting was requested, but _sys_exit was */
void _sys_exit(int x)
{
    x = x;
}
/* __use_no_semihosting was requested, but _ttywrch was */
void _ttywrch(int ch)
{
    ch = ch;
}
#elif defined(__CC_ARM)
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
};
FILE __stdout;

/* __use_no_semihosting was requested, but _sys_exit was */
void _sys_exit(int x)
{
    x = x;
}
#endif /* __ARMCC_VERSION */

#endif /* __MICROLIB */

#if defined ( __GNUC__ ) && !defined (__clang__) 
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
    USART1->DR = (uint8_t)ch;

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	return ch;
}

static int usart_gpio_init(USART_TypeDef *usartx)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    uint32_t usartx_base = (uint32_t)usartx;
    if (usartx_base != USART1_BASE && usartx_base != USART2_BASE &&
        usartx_base != USART3_BASE)
        return -1;
    
    switch (usartx_base) {
    case USART1_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        break;
    case USART2_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        break;
    case USART3_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        break;
    default:
        return -1;
    }
    
    return 0;
}

static int usart_msp_init(USART_TypeDef *usartx, const uint32_t baudrate)
{
    uint32_t usartx_base = (uint32_t)usartx;
    if (usartx_base != USART1_BASE && usartx_base != USART2_BASE &&
        usartx_base != USART3_BASE)
        return -1;
    /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitTypeDef USART_InitStructure = {0};
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(usartx, &USART_InitStructure);
    USART_Cmd(usartx, ENABLE);
    return 0;
}

static int usart_nvic_config(USART_TypeDef *usartx)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    
    uint32_t usartx_base = (uint32_t)usartx;
    if (usartx_base != USART1_BASE && usartx_base != USART2_BASE &&
        usartx_base != USART3_BASE)
        return -1;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    switch (usartx_base) {
    case USART1_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        break;
    case USART2_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        break;
    case USART3_BASE:
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        break;
    default:
        return -1;
    }
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 开启串口接收缓冲区非空中断 */
    USART_ITConfig(usartx, USART_IT_RXNE, ENABLE);
    
    return 0;
}

int usartx_init(USART_TypeDef *usartx, const uint32_t baudrate)
{
    uint32_t usartx_base = (uint32_t)usartx;
    
    switch (usartx_base) {
    case USART1_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        break;
    case USART2_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        break;
    case USART3_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        break;
    default:
        return -1;
    }
    
    if (usart_msp_init(usartx, baudrate))
        return 1;
    
    if (usart_gpio_init(usartx))
        return 2;
    
    if (usart_nvic_config(usartx))
        return 3;
    
    return 0;
}
