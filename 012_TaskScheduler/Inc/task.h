/*
 * task.h
 *
 *  Created on: Jun 30, 2024
 *      Author: akshay
 */

#ifndef TASK_H_
#define TASK_H_

/*
 * TCB
 */
#define USER_TASK_MAX_NO			(5U)

enum task_state {INIT = 0, READY, SUSPENDED, RUNNING};

typedef void (*task_handle_t)(void* , void*);

typedef struct {
	uint8_t task_no;
	enum task_state state;
	uint32_t* stack_pointer;
	uint32_t block_ticks;
	task_handle_t task_handle;
} tcb_t;

extern tcb_t tcb[];



extern uint32_t current_task;

extern int task_create(task_handle_t task_handle);
extern void task(void* param1, void* param2);

#endif /* TASK_H_ */
