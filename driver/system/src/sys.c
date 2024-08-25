#include "sys.h" 

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");  
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i"); 
}
//设置栈顶地址
//addr:栈顶地址
void MSR_MSP(unsigned long addr) 
{
	__ASM volatile
    (
        "MSR MSP, r0 \n\t"
        "BX r14"
    );
}

#elif defined(__CC_ARM)
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(unsigned long addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
#endif /* __ARMCC_VERSION */
