/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define SRAM_START 		0x20000000U
#define SRAM_SIZE 		(96*1024U) // 96kb ram

#define MSP_STACK_START SRAM_START + SRAM_SIZE // High memory
#define STACK_SIZE 		(2*1024U) // Stack size = 2Kb

#define MSP_STACK_END 	MSP_STACK_START - STACK_SIZE
#define PSP_STACK_START MSP_STACK_END
#define PSP_STACK_END 	PSP_STACK_START - STACK_SIZE

int hello_sum(int x, int y)
{
	int a = 0;

	for (int i = 0; i < x; i++)
		a += x + y;

	return a;
}

uint32_t psp_address = PSP_STACK_START; // just for the test purpose only

__attribute__((naked)) void switch_to_psp(void)
{

#if 0
	__asm volatile("MOV R0, %[PSP_ADDR_ASM]": : [PSP_ADDR_ASM] "r" (psp_address));
#else

	__asm volatile(".equ MSP_START, (0x20000000U + (96*1024U))");
	__asm volatile(".equ MSP_END,MSP_START - (2 *1024U)");
	__asm volatile(".equ PSP_START, MSP_END");
	__asm volatile("LDR R0, =PSP_START");
#endif
	__asm volatile("MSR PSP, R0");
	__asm volatile("MRS R0, CONTROL");
	__asm volatile("ORR R0, R0, 0x02");
	__asm volatile("MSR CONTROL, R0");
	__asm volatile("BX LR");
}

/*
 * __attribute__ ((naked))
 *  A function compiled by a compiler has Prologue and Epilogue which are decibed as below:.
 *  1. Prologue:  instr. at the beginning of function to prepare function exec.
 *  			- save return addr,
 *  			- setting SP for old and new,
 *  			- stack allocation etc
 *  2. Epilogue :  instr. at the end of function, for function exit
 *  			- Restoring old SP, regs
 *  			- Returning to caller function (BX LR)
 *
 * If the function has naked attribute, it is expected that Prologue and Epilogue must be manually taken care.
 *
 */
//

void generate_svc_exeception()
{
	__asm volatile("SVC #0x20"); // SVC
}

void SVC_Handler()
{
	// Here, it must use MSP
	__asm volatile("MOV R0, R0"); // NOP
	__asm volatile("MOV R0, R0"); // NOP
	__asm volatile("MOV R0, R0"); // NOP
}

extern void initialise_monitor_handles();

int main(void)
{

	initialise_monitor_handles();
	int h = 55;
	int x = hello_sum(1, 2); // This uses MSP
	// switch_to_psp();
	switch_to_psp();

	int y = hello_sum(1, 2); // This uses PSP
	h = 60;					 // this is still valid and on MSP stack

	int g = 99; // This is local variable, still uses MSP

	generate_svc_exeception();
	/* Loop forever */
	g = 100;
	for (;;)
		;
}
