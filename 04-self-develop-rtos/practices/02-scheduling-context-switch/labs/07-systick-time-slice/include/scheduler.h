#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "compiler.h"
#include "task.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HR_PRIORITY_COUNT    (4U)
#define HR_MAX_TASKS         (8U)

extern hr_task_t *g_current_task;
extern volatile uint32_t g_kernel_tick;
extern volatile uint32_t g_context_switch_count;

void hr_scheduler_init(void);
bool hr_scheduler_add_task(hr_task_t *task);
HR_NORETURN void hr_scheduler_start(void);
void hr_scheduler_commit_switch(void);
void hr_scheduler_on_tick(void);
void hr_task_yield(void);
void hr_task_delay(uint32_t ticks);
void hr_task_block_current(void);
bool hr_task_wake(hr_task_t *task);
bool hr_task_wake_from_isr(hr_task_t *task);
hr_task_t *hr_scheduler_current(void);
size_t hr_scheduler_task_count(void);
hr_task_t *hr_scheduler_task_at(size_t index);
uint32_t hr_scheduler_ready_bitmap(void);

#endif
