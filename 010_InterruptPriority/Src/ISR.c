/*
 * ISR.c
 *
 *  Created on: Jun 27, 2024
 *      Author: akshay
 */

#include <stdio.h>
#include <stdint.h>

extern uint8_t pend_in_irq;

extern void config_IRQx_pend(uint8_t irqno); // only for demo!!

void DMA1_Stream0_IRQHandler()
{
	printf("ISR: DMA1_Stream0_IRQHandler begin\n");

	printf("ISR: DMA1_Stream0_IRQHandler end\n");
}

void ADC_IRQHandler()
{
	printf("ISR: ADC_IRQHandler begin\n");

	if (pend_in_irq)
	{
		/*
		 * This will lead to preemption,
		 * if pended ISR has higher prio than current
		 */
		config_IRQx_pend(11); // DMA
		//*((uint32_t*)0XE000E200U) |= 1 << 11;
	}
	// lets wait for sometime
	for (uint32_t i = 0; i < 100000; ++i)
		;

	printf("ISR: ADC_IRQHandler end\n");
}

void HardFault_Handler()
{
	printf("ISR: HardFault_Handler\n");
	for(;;);
}

void MemManage_Handler()
{
	printf("ISR: MemManage_Handler\n");
	for(;;);
}

void BusFault_Handler()
{
	printf("ISR: MemManage_Handler\n");
	for(;;);
}

void UsageFault_Handler()
{
	printf("ISR: UsageFault_Handler\n");
	for(;;);
}
