/*
 * ISR.c
 *
 *  Created on: Jun 27, 2024
 *      Author: akshay
 */

#include <stdint.h>
#include <stdio.h>

/* SCB registers */

#define SHPR1_REG				(0xE000ED18) //System Handler Priority Register 1
#define SHPR2_REG				(0xE000ED1C) //System Handler Priority Register 2
#define SHPR3_REG				(0xE000ED20) //System Handler Priority Register 3
#define SHCSR_REG				(0xE000ED24) //System Handler Control and State Register : Enable faults


#define CCR_REG					(0xE000ED14) //	Configuration and Control Register
#define CCR_BIT_DIV_0_TRP		(4) // 0 = do not trap divide by 0
#define CCR_BIT_STKALIGN		(9) //Indicates stack alignment on exception entry: 0 = 4-byte aligned, 1 = 8-byte aligned.


/*
 * Fault status registers
 */

#define HFSR_REG				(0xE000ED2C) //HardFault Status register

#define HFSR_BIT_FORCED			(30)
#define HFSR_BIT_VECTTBL		(1)
#define HFSR_BIT_DEBUGEVT		(31)


#define MMSR_REG				(0xE000ED28) //MemManage Fault Status Register
#define BFSR_REG				(0xE000ED29) //BusFault Status Register
#define UFSR_REG				(0xE000ED2A) //Usage Fault Status Register
#define CFSR_REG				(MMSR_REG) //Configurable Fault Status Register, word access

/*
 * Fault address registers
 */

#define MMFAR_REG				(0xE000ED34) //MemManage Address Register
#define BFAR_REG				(0xE000ED38) //BusFault Address Register


#define SCB_AIRCR			(0xE000ED0CU) // Application Interrupt and Reset Control Register
#define VECTKEY				(0x5FA << 16)
#define AIRCR_BIT_SYSRESETREQ				(2U) // for system reset
/*
 * Here, all the fault handlers and their init config.
 */

#define USGFAULTENA 		(18)
#define BUSFAULTENA			(17)
#define MEMFAULTENA			(16)

#define MEMFAULTACT			(0)
#define BUSFAULTACT			(1)
#define USGFAULTACT			(3)

#define BUSFAULTPENDED		(14)
#define MEMFAULTPENDED		(13)
#define USGFAULTPENDED		(12)

extern uint32_t _estack;
extern uint32_t _Min_Stack_Size;

/* Stack dump functions */

uint32_t get_stack_base(uint32_t exc_val, uint32_t PSP, uint32_t MSP)
{
	return (exc_val == 0xFFFFFFFD) ? PSP : MSP;
}

uint8_t stack_dump(uint32_t *pStackBase)
{
	if (!pStackBase || (uint32_t)pStackBase > (uint32_t)&_estack)
	{ // all checks are not included..
		printf("Invalid Stack Frame base!\n");
		return -1;
	}

	printf("*****************Stack Frame Dump*****************\n");

	printf("xPSR: 0x%lX\t PC:0x%lX\t LR: 0x%lX\t 12:0x%lX\n",
		   pStackBase[7], pStackBase[6], pStackBase[5],
		   pStackBase[4]);

	printf("R3: 0x%lX\t R2:0x%lX\t R1: 0x%lX\t R0:0x%lX\n",
		   pStackBase[3], pStackBase[2], pStackBase[1],
		   pStackBase[0]);

	printf("**************************************************\n");
	return 0;
}

/* Request system reset */

void request_system_reset()
{

	for (int i = 3; i >= 0; --i)
	{
		printf("Resetting in %u\r", i);
		for (int i = 0; i < 160000; ++i)
			;
	}
	printf("\n");

	uint32_t *pSCB_AIRCR = (uint32_t *)SCB_AIRCR;

	uint32_t AIRCR_temp = *pSCB_AIRCR;
	// VECTKEY must be cleared
	AIRCR_temp &= ~((0xFFFF << 16));
	// write back with VECTKEY and set reset.
	*pSCB_AIRCR = AIRCR_temp | VECTKEY | (1 << AIRCR_BIT_SYSRESETREQ);
}

/* All Fault Handlers implementation */

__attribute__((naked)) void HardFault_Handler()
{

	__asm volatile("MOV R0, LR");
	__asm volatile("MRS R1, PSP");
	__asm volatile("MRS R2, MSP");
	__asm volatile("BL get_stack_base");
	__asm volatile("BL stack_dump");

	__asm volatile("BL handle_hardfault");

	__asm volatile("B request_system_reset");
	// never reach
	for (;;)
		;
}

__attribute__((naked)) void MemManage_Handler()
{

	__asm volatile("MOV R0, LR");
	__asm volatile("MRS R1, PSP");
	__asm volatile("MRS R2, MSP");
	__asm volatile("BL get_stack_base");
	__asm volatile("BL stack_dump");

	__asm volatile("BL handle_memmanagefault");

	__asm volatile("B request_system_reset");
	// never reach
	for (;;)
		;
}

__attribute__((naked)) void BusFault_Handler()
{

	__asm volatile("MOV R0, LR");
	__asm volatile("MRS R1, PSP");
	__asm volatile("MRS R2, MSP");
	__asm volatile("BL get_stack_base");
	__asm volatile("BL stack_dump");

	__asm volatile("BL handle_busfault");

	__asm volatile("B request_system_reset");
	// never reach
	for (;;)
		;
}

__attribute__((naked)) void UsageFault_Handler()
{

	__asm volatile("MOV R0, LR");
	__asm volatile("MRS R1, PSP");
	__asm volatile("MRS R2, MSP");
	__asm volatile("BL get_stack_base");
	__asm volatile("BL stack_dump");

	__asm volatile("BL handle_usagefault");

	__asm volatile("B request_system_reset");
	// never reach
	for (;;)
		;
}

void handle_hardfault()
{
	printf("Hard Fault occurred");

	uint32_t *pHFSR_REG = (uint32_t *)HFSR_REG;

	if ((*pHFSR_REG >> HFSR_BIT_FORCED) & 0x1)
	{ // FORCED?
		printf(", Forced: \n");

		printf("MMFSR: 0x%X\t BFSR: 0x%X\t UFSR: 0x%X\t CFSR: 0x%lX\n",
			   *(uint8_t *)MMSR_REG,
			   *(uint8_t *)BFSR_REG,
			   *(uint16_t *)UFSR_REG,
			   *(uint32_t *)CFSR_REG); // ideally CFSR is sufficient

		if ((*(uint8_t *)BFSR_REG >> 7) & 0x1)
		{ // Bus fault address valid
			printf("BFAR: 0x%lX\n", *((uint32_t *)BFAR_REG));
		}
		if ((*(uint8_t *)MMSR_REG >> 7) & 0x1)
		{ // Bus fault address valid
			printf("MMFAR: 0x%lX\n", *((uint32_t *)MMFAR_REG));
		}
	}
}

void handle_memmanagefault()
{
	printf("MemManage Fault occurred\n");
	printf("MMFSR: 0x%X\t", *((uint8_t *)MMSR_REG));
	if ((*(uint8_t *)BFSR_REG >> 7) & 0x1)
	{ // Bus fault address valid
		printf("MMFAR: 0x%lX\n", *((uint32_t *)MMFAR_REG));
	}
	printf("\n");
}

void handle_busfault()
{
	printf("Bus Fault occurred\n");
	printf("BFSR: 0x%X\t", *(uint8_t *)BFSR_REG);
	if ((*(uint8_t *)BFSR_REG >> 7) & 0x1)
	{ // Bus fault address valid
		printf("BFAR: 0x%lX\n", *((uint32_t *)BFAR_REG));
	}
	printf("\n");
}

void handle_usagefault()
{
	printf("Usage Fault occurred\n");
	printf("UFSR: 0x%X\n", *(uint16_t *)UFSR_REG);
}

#define ESCLATE_USAGE_FAULT
void init_all_fault_handlers()
{
	uint32_t *pSHCSR_REG = (uint32_t *)SHCSR_REG;

	*pSHCSR_REG |= (1 << BUSFAULTENA) | (1 << MEMFAULTENA);

#ifndef ESCLATE_USAGE_FAULT
	*pSHCSR_REG |= (1 << USGFAULTENA);
#endif
}

void enable_exception_zerodivide()
{
	uint32_t *pCCR_REG = (uint32_t *)CCR_REG;
	*pCCR_REG |= (1 << CCR_BIT_DIV_0_TRP);
	return;
}
