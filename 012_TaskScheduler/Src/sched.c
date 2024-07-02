/*
 * sched.c
 *
 *  Created on: Jun 29, 2024
 *      Author: akshay
 */

#include <stdint.h>
#include <stdio.h>
#include "task.h"


#define SCB_SYST_CSR_REG 			(0xE000E010U) //SysTick Control and Status Register
#define SCB_SYST_RVR_REG 			(0xE000E014U) //SysTick Reload Value Register
#define SCB_SYST_CVR_REG 			(0xE000E018U) //SysTick Current Value Register
#define SCB_SYST_CALIB_REG 			(0xE000E01CU) //SysTick Calibration Value Register


#define PULSE_1MS					(16000U)

#define SYS_CLK_FRQ					(16000000U) // default HSI clock runs at 16MHZ for STM32F401RE
#define SHPR3_REG 					(0xE000ED20)

#define SBC_ICSR					(0xE000ED04U)
#define ICSR_BIT_PENDSV				(28)

uint32_t global_ticks = 0;

void schedule();

void sw_delay(uint32_t t_ms)
{
	t_ms = t_ms * PULSE_1MS;
	for (uint32_t i = 0; i < t_ms; ++i)
		;
}

void delay(uint32_t t_ms)
{
	// TBD: impl using hardware timer
	if (current_task == (USER_TASK_MAX_NO - 1))
	{
		return;
	}
	tcb[current_task].block_ticks = global_ticks + t_ms;
	tcb[current_task].state = SUSPENDED;

	// TCB got updated, so ask pendsv to schedule next task

	schedule();
}

uint32_t retrive_current_task_stack_pointer(void)
{
	return (uint32_t)tcb[current_task].stack_pointer;
}

void update_current_task_stack_pointer(uint32_t sp)
{
	tcb[current_task].stack_pointer = (uint32_t *)sp;
}

void update_current_task_state(enum task_state state)
{
	tcb[current_task].state = state;
}

__attribute__((naked)) void PendSV_Handler(void)
{ /* implement context switching
// When when switching to handler, the proc already saves basic context
 * i.e xPSR, PC, LR, R12, R0-R3.
 * For complete process state, we need to save R4-R11, and SP (PSP)
 */
	// Get the stack base (MSP or PSP)
	__asm volatile("MOV R0, LR");
	__asm volatile("MRS R1, PSP");
	__asm volatile("MRS R2, MSP");

	__asm volatile("PUSH {LR}");		 // Important to store LR before switching to C functions..
	__asm volatile("BL get_stack_base"); // basically MSP or PSP based on EXC_RETURN value.

	// Push all the R4-R11 regs to the stack base
	//  STMDB: STR Multiple with decrement: R0 decrements(pointer to stack) for each reg STR on stack
	__asm volatile("STMDB R0!, {R4-R11}");

	// update the TCB for current task with updated PSP
	__asm volatile("BL update_current_task_stack_pointer");

	// schedule the next task
	__asm volatile("BL estimate_next_task");

	// Restore context for the scheduled task, pitch in the SP of the task
	__asm volatile("BL retrive_current_task_stack_pointer");

	// restore the process state
	__asm volatile("LDMIA R0!, {R4-R11}");

	// update the PSP (SP) with updated PSP
	__asm volatile("MSR PSP, R0");

	// return
	__asm volatile("POP {LR}");
	__asm volatile("BX LR");
}

__attribute__((naked)) void switch_sp_to_msp()
{
	__asm volatile("MRS R0, CONTROL");
	__asm volatile("AND R0, R0, #0xFFFFFFFE");
	__asm volatile("MSR CONTROL, R0");
	__asm volatile("BX LR");
}

__attribute__((naked)) void switch_sp_to_psp()
{
	__asm volatile("MRS R0, CONTROL");
	__asm volatile("ORR R0, R0, #0x2");
	__asm volatile("MSR CONTROL, R0");
	__asm volatile("BX LR");
}

uint32_t get_sp()
{
	uint32_t sp = 0;

	__asm volatile("MOV %[SP_ASM], SP" ::[SP_ASM] "r"(sp));

	return sp;
}

uint32_t get_psp()
{
	uint32_t psp = 0;

	__asm volatile("MRS %[PSP_ASM], PSP" ::[PSP_ASM] "r"(psp));

	return psp;
}
uint32_t get_msp()
{
	uint32_t msp = 0;

	__asm volatile("MRS %[MSP_ASM], MSP" ::[MSP_ASM] "r"(msp));

	return msp;
}

/* systick timer init */

void SysClkTicker_Init(uint32_t freq)
{
	printf("Init SysClkTicker..\n");
	uint32_t reload_value = SYS_CLK_FRQ / (1 * freq);

	uint32_t *pSCB_SYST_RVR_REG = (uint32_t *)SCB_SYST_RVR_REG;

	/* it fires at every 1ms */
	*pSCB_SYST_RVR_REG = (reload_value - 1);

	uint32_t *pSCB_SYST_CSR_REG = (uint32_t *)SCB_SYST_CSR_REG;
	*pSCB_SYST_CSR_REG |= 1 << 1 | 1 << 2;

	uint32_t *pSHPR3_REG = (uint32_t *)SHPR3_REG;
	// Systick, Pendsv prio
	*pSHPR3_REG |= (0x0 < 24) | (0x1 << 16);

	*pSCB_SYST_CSR_REG |= 1 << 0;
}

void estimate_next_task()
{ // s
	// We must calculate the "next task"
	// if any task with READY is the current task, except the idle task.
	uint8_t prev_task = current_task;
	// Assume IDLE task initially, if none of the task turn out to be READY
	uint8_t next_task = USER_TASK_MAX_NO - 1;

	uint8_t start_index = (prev_task + 1) % USER_TASK_MAX_NO;

	// If the previous task is not suspended and not the idle task, set it to READY
	if (tcb[prev_task].state != SUSPENDED && prev_task != (USER_TASK_MAX_NO - 1))
	{
		tcb[prev_task].state = READY;
	}

	// Iterate over the states, wrapping around if necessary
	for (uint8_t i = 0; i < USER_TASK_MAX_NO; i++)
	{
		uint8_t current_index = (start_index + i) % USER_TASK_MAX_NO;
		if (tcb[current_index].state == READY)
		{
			next_task = current_index;
			break;
		}
	}

	current_task = next_task;
	tcb[current_task].state = RUNNING;
}

void update_blocked_tasks_status()
{
	for (uint8_t i = 0; i < (USER_TASK_MAX_NO - 1); ++i)
	{
		if (tcb[i].state == SUSPENDED && tcb[i].block_ticks == global_ticks)
		{
			// such task must be set to ready
			tcb[i].state = READY;
			tcb[i].block_ticks = 0;
		}
	}
}

void schedule()
{
	// pend the pendsv exception
	uint32_t *pSBC_ICSR = (uint32_t *)SBC_ICSR;
	*pSBC_ICSR |= (1 << ICSR_BIT_PENDSV);
}
/* systick timer */

void SysTick_Handler()
{
	// update global ticks
	global_ticks++;
	if (!(global_ticks % 1000))
		printf("GTC%lu\n", global_ticks);

	update_blocked_tasks_status();

	// Since TCB got update and we are in sys tick,
	//  ask pendSV to do context switch based on the next task
	schedule();
}
