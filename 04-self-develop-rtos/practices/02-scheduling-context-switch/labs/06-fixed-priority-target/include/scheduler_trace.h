#ifndef SCHEDULER_TRACE_H
#define SCHEDULER_TRACE_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
    HR_TRACE_KERNEL_START = 1,
    HR_TRACE_SWITCH,
    HR_TRACE_YIELD,
    HR_TRACE_DELAY,
    HR_TRACE_WAKE,
    HR_TRACE_TICK_ROTATE,
    HR_TRACE_ISR_WAKE
} hr_trace_type_t;

typedef struct
{
    uint32_t tick;
    uint32_t sequence;
    hr_trace_type_t type;
    uint8_t from_task;
    uint8_t to_task;
} hr_trace_record_t;

void hr_trace_reset(void);
void hr_trace_record(hr_trace_type_t type,
                     uint8_t from_task,
                     uint8_t to_task,
                     uint32_t tick);
size_t hr_trace_snapshot(hr_trace_record_t *output, size_t capacity);

#endif
