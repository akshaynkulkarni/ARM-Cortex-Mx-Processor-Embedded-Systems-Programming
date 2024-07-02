#include <stdio.h>
#include <stdint.h>

#include "task.h"

extern uint32_t _estack;

#define STACK_START						((uint32_t)(&_estack))

#define CORE_STACK_START				(STACK_START)
#define CORE_STACK_SIZE					(32 * 1024U) // Core: kernel/sched stack size

#define USER_TASKS_STACK_BASE			(CORE_STACK_START - CORE_STACK_SIZE)
#define USER_TASK_STACK_SIZE			(2 *1024U) // 2KB per user stack

#define USER_TASKX_STACK_BASE(X)		(USER_TASKS_STACK_BASE - ((X) * USER_TASK_STACK_SIZE))

uint32_t initial_psp = USER_TASKS_STACK_BASE;

tcb_t tcb[USER_TASK_MAX_NO];

extern int IDLE_TASK(void *x, void *y);

static uint8_t current_allocated_task_no = 0; // IDLE TASK

void task_init(tcb_t temp_tcb)
{

	tcb[current_allocated_task_no] = temp_tcb;

	uint32_t *sp = (uint32_t *)USER_TASKX_STACK_BASE(current_allocated_task_no);
	*sp-- = 0x01000000;											  // xPSR
	*sp-- = (uint32_t)tcb[current_allocated_task_no].task_handle; // PC
	*sp-- = 0xFFFFFFFD;											  // LR
	*sp-- = 0x00000000;								   // R12
	*sp-- = 0x00000000;								   // R3
	*sp-- = 0x00000000;								   // R2
	*sp-- = 0x00000000;								   // R1
	*sp-- = 0x00000000;								   // R0
	*sp-- = 0x00000000;								   // R11
	*sp-- = 0x00000000;								   // R10
	*sp-- = 0x00000000;								   // R9
	*sp-- = 0x00000000;								   // R8
	*sp-- = 0x00000000;								   // R7
	*sp-- = 0x00000000;								   // R6
	*sp-- = 0x00000000;								   // R5
	*sp = 0x00000000;								   // R4
	tcb[current_allocated_task_no].stack_pointer = sp; // PSP (SP)

	// Finally Mark it as READY
	if (current_allocated_task_no != (USER_TASK_MAX_NO - 1))
	{
		tcb[current_allocated_task_no].state = READY;
	}
}

int task_create(task_handle_t task_handle)
{

	if (current_allocated_task_no > USER_TASK_MAX_NO)
	{
		return -1;
	}

	tcb_t temp_tcb = {
		.task_no = current_allocated_task_no,
		.state = INIT,
		.stack_pointer = 0,
		.block_ticks = 0,
		.task_handle = task_handle,
	};

	if (current_allocated_task_no == (USER_TASK_MAX_NO - 1))
	{
		temp_tcb.task_handle = (task_handle_t)&IDLE_TASK; // IDLE TASK
		temp_tcb.state = RUNNING;						  // IDLE TASK
	}

	task_init(temp_tcb);

	return ++current_allocated_task_no;
}

uint32_t current_task = 0;

extern void switch_sp_to_psp();
void tasks_start()
{
	static uint32_t t_ms = 0;
	static uint32_t x = 0;
	for (uint32_t i = 0; i < current_allocated_task_no; ++i)
	{
		x = i;
		t_ms = 250 * (i + 1);
		switch_sp_to_psp();
		tcb[i].task_handle(&x, &t_ms);
	}
}
