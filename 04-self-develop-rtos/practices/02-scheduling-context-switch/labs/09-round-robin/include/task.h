#ifndef TASK_H
#define TASK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HR_TASK_STACK_FILL  (0xA5A5A5A5UL)
#define HR_TASK_STACK_GUARD (0xDEADBEEFUL)
#define HR_WAIT_FOREVER     (0xFFFFFFFFUL)
#define HR_DEFAULT_TIME_SLICE_TICKS (5U)

typedef void (*hr_task_entry_t)(void *argument);

typedef enum
{
    HR_TASK_CREATED = 0,
    HR_TASK_READY,
    HR_TASK_RUNNING,
    HR_TASK_BLOCKED
} hr_task_state_t;

typedef struct hr_task
{
    uint32_t *saved_sp;
    uint32_t *stack_low;
    uint32_t *stack_high;
    const char *name;
    hr_task_entry_t entry;
    void *argument;
    struct hr_task *ready_previous;
    struct hr_task *ready_next;
    uint32_t wake_tick;
    uint32_t runtime_ticks;
    uint32_t switch_count;
    uint32_t time_slice_remaining;
    uint16_t stack_word_count;
    uint8_t id;
    uint8_t priority;
    hr_task_state_t state;
    bool in_ready_queue;
} hr_task_t;

bool hr_task_create_static(hr_task_t *task,
                           const char *name,
                           uint8_t id,
                           uint8_t priority,
                           hr_task_entry_t entry,
                           void *argument,
                           uint32_t *stack,
                           size_t stack_word_count);

size_t hr_task_stack_unused_words(const hr_task_t *task);
bool hr_task_stack_guard_ok(const hr_task_t *task);
void hr_task_return_error(void);

#endif
