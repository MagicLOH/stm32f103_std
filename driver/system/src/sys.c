#include "sys.h" 

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");  
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i"); 
}
//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(unsigned long addr) 
{
	__ASM volatile
    (
        "MSR MSP, r0 \n\t"
        "BX r14"
    );
}

#elif defined(__CC_ARM)
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(unsigned long addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
#endif /* __ARMCC_VERSION */
