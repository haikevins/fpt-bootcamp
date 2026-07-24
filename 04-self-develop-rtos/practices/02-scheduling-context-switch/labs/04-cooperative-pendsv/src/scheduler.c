#include "scheduler.h"
#include "cortex_m3_port.h"
#include "critical_section.h"
#include "panic.h"
#include "scheduler_trace.h"

#include <stdint.h>

typedef struct
{
    hr_task_t *head;
    hr_task_t *tail;
    uint8_t count;
} hr_ready_queue_t;

static hr_ready_queue_t g_ready_queues[HR_PRIORITY_COUNT];
static hr_task_t *g_all_tasks[HR_MAX_TASKS];
static size_t g_task_count;
static uint32_t g_ready_bitmap;

hr_task_t *g_current_task;
volatile uint32_t g_kernel_tick;
volatile uint32_t g_context_switch_count;

static bool tick_due(uint32_t now, uint32_t wake_tick)
{
    return (int32_t)(now - wake_tick) >= 0;
}

static uint8_t highest_ready_priority(void)
{
    for (uint8_t priority = 0U;
         priority < (uint8_t)HR_PRIORITY_COUNT;
         ++priority)
    {
        if ((g_ready_bitmap & (1UL << priority)) != 0U)
        {
            return priority;
        }
    }

    hr_panic(HR_PANIC_NO_READY_TASK, g_ready_bitmap);
}

static void ready_set_bitmap(uint8_t priority)
{
    g_ready_bitmap |= (1UL << priority);
}

static void ready_clear_bitmap_if_empty(uint8_t priority)
{
    if (g_ready_queues[priority].count == 0U)
    {
        g_ready_bitmap &= ~(1UL << priority);
    }
}

static void ready_enqueue_tail(hr_task_t *task)
{
    hr_ready_queue_t *queue;

    HR_ASSERT(task != (hr_task_t *)0);
    HR_ASSERT(task->priority < HR_PRIORITY_COUNT);
    HR_ASSERT(!task->in_ready_queue);

    queue = &g_ready_queues[task->priority];
    task->ready_next = (hr_task_t *)0;
    task->ready_previous = queue->tail;

    if (queue->tail != (hr_task_t *)0)
    {
        queue->tail->ready_next = task;
    }
    else
    {
        queue->head = task;
    }

    queue->tail = task;
    ++queue->count;
    task->in_ready_queue = true;
    ready_set_bitmap(task->priority);
}

static void ready_remove(hr_task_t *task)
{
    hr_ready_queue_t *queue;

    HR_ASSERT(task != (hr_task_t *)0);
    HR_ASSERT(task->priority < HR_PRIORITY_COUNT);
    HR_ASSERT(task->in_ready_queue);

    queue = &g_ready_queues[task->priority];

    if (task->ready_previous != (hr_task_t *)0)
    {
        task->ready_previous->ready_next = task->ready_next;
    }
    else
    {
        queue->head = task->ready_next;
    }

    if (task->ready_next != (hr_task_t *)0)
    {
        task->ready_next->ready_previous = task->ready_previous;
    }
    else
    {
        queue->tail = task->ready_previous;
    }

    task->ready_previous = (hr_task_t *)0;
    task->ready_next = (hr_task_t *)0;
    task->in_ready_queue = false;
    HR_ASSERT(queue->count > 0U);
    --queue->count;
    ready_clear_bitmap_if_empty(task->priority);
}

static void ready_rotate(uint8_t priority)
{
    hr_ready_queue_t *queue = &g_ready_queues[priority];
    hr_task_t *first;

    if (queue->count <= 1U)
    {
        return;
    }

    first = queue->head;
    HR_ASSERT(first != (hr_task_t *)0);

    queue->head = first->ready_next;
    queue->head->ready_previous = (hr_task_t *)0;

    first->ready_next = (hr_task_t *)0;
    first->ready_previous = queue->tail;
    queue->tail->ready_next = first;
    queue->tail = first;
}

static hr_task_t *select_next(void)
{
    const uint8_t priority = highest_ready_priority();
    hr_task_t *next = g_ready_queues[priority].head;

    HR_ASSERT(next != (hr_task_t *)0);
    HR_ASSERT(next->in_ready_queue);
    HR_ASSERT(hr_task_stack_guard_ok(next));
    HR_ASSERT(next->saved_sp >= next->stack_low);
    HR_ASSERT(next->saved_sp < next->stack_high);

    return next;
}

void hr_scheduler_init(void)
{
    for (size_t index = 0U; index < HR_PRIORITY_COUNT; ++index)
    {
        g_ready_queues[index].head = (hr_task_t *)0;
        g_ready_queues[index].tail = (hr_task_t *)0;
        g_ready_queues[index].count = 0U;
    }

    for (size_t index = 0U; index < HR_MAX_TASKS; ++index)
    {
        g_all_tasks[index] = (hr_task_t *)0;
    }

    g_task_count = 0U;
    g_ready_bitmap = 0U;
    g_current_task = (hr_task_t *)0;
    g_kernel_tick = 0U;
    g_context_switch_count = 0U;
    hr_trace_reset();
}

bool hr_scheduler_add_task(hr_task_t *task)
{
    hr_irq_state_t irq_state;

    if ((task == (hr_task_t *)0) ||
        (task->priority >= HR_PRIORITY_COUNT) ||
        (task->state != HR_TASK_CREATED) ||
        (g_task_count >= HR_MAX_TASKS))
    {
        return false;
    }

    irq_state = hr_critical_enter();
    task->state = HR_TASK_READY;
    ready_enqueue_tail(task);
    g_all_tasks[g_task_count] = task;
    ++g_task_count;
    hr_critical_exit(irq_state);
    return true;
}

void hr_scheduler_start(void)
{
    hr_port_init();
    g_current_task = select_next();
    g_current_task->state = HR_TASK_RUNNING;
    ++g_current_task->switch_count;
    hr_trace_record(HR_TRACE_KERNEL_START,
                    0xFFU,
                    g_current_task->id,
                    g_kernel_tick);
    hr_port_start_first_task();
}

void hr_scheduler_commit_switch(void)
{
    hr_task_t *old_task = g_current_task;
    hr_task_t *next_task = select_next();

    if ((old_task != (hr_task_t *)0) &&
        (old_task != next_task) &&
        (old_task->state == HR_TASK_RUNNING))
    {
        old_task->state = HR_TASK_READY;
    }

    next_task->state = HR_TASK_RUNNING;
    g_current_task = next_task;

    if (old_task != next_task)
    {
        ++g_context_switch_count;
        ++next_task->switch_count;
        hr_trace_record(HR_TRACE_SWITCH,
                        (old_task != (hr_task_t *)0) ? old_task->id : 0xFFU,
                        next_task->id,
                        g_kernel_tick);
    }
}

void hr_scheduler_on_tick(void)
{
    bool switch_required = false;
    const hr_irq_state_t irq_state = hr_critical_enter();

    ++g_kernel_tick;

    for (size_t index = 0U; index < g_task_count; ++index)
    {
        hr_task_t *task = g_all_tasks[index];

        if ((task != (hr_task_t *)0) &&
            (task->state == HR_TASK_BLOCKED) &&
            (task->wake_tick != HR_WAIT_FOREVER) &&
            tick_due(g_kernel_tick, task->wake_tick))
        {
            task->wake_tick = HR_WAIT_FOREVER;
            task->state = HR_TASK_READY;
            ready_enqueue_tail(task);
            hr_trace_record(HR_TRACE_WAKE,
                            (g_current_task != (hr_task_t *)0) ?
                                g_current_task->id : 0xFFU,
                            task->id,
                            g_kernel_tick);

            if ((g_current_task == (hr_task_t *)0) ||
                (task->priority < g_current_task->priority))
            {
                switch_required = true;
            }
        }
    }

    if ((g_current_task != (hr_task_t *)0) &&
        (g_current_task->state == HR_TASK_RUNNING))
    {
        ++g_current_task->runtime_ticks;

        if (g_current_task->time_slice_remaining > 0U)
        {
            --g_current_task->time_slice_remaining;
        }

        if (g_current_task->time_slice_remaining == 0U)
        {
            g_current_task->time_slice_remaining =
                HR_DEFAULT_TIME_SLICE_TICKS;

            if (g_ready_queues[g_current_task->priority].count > 1U)
            {
                const uint8_t old_id = g_current_task->id;
                ready_rotate(g_current_task->priority);
                g_current_task->state = HR_TASK_READY;
                hr_trace_record(HR_TRACE_TICK_ROTATE,
                                old_id,
                                g_ready_queues[g_current_task->priority].head->id,
                                g_kernel_tick);
                switch_required = true;
            }
        }
    }

    if (switch_required)
    {
        hr_port_request_context_switch();
    }

    hr_critical_exit(irq_state);
}

void hr_task_yield(void)
{
    const hr_irq_state_t irq_state = hr_critical_enter();

    HR_ASSERT(g_current_task != (hr_task_t *)0);
    HR_ASSERT(g_current_task->in_ready_queue);

    if (g_ready_queues[g_current_task->priority].count > 1U)
    {
        const uint8_t old_id = g_current_task->id;
        ready_rotate(g_current_task->priority);
        g_current_task->state = HR_TASK_READY;
        g_current_task->time_slice_remaining =
            HR_DEFAULT_TIME_SLICE_TICKS;
        hr_trace_record(HR_TRACE_YIELD,
                        old_id,
                        g_ready_queues[g_current_task->priority].head->id,
                        g_kernel_tick);
        hr_port_request_context_switch();
    }

    hr_critical_exit(irq_state);
}

void hr_task_delay(uint32_t ticks)
{
    hr_irq_state_t irq_state;

    if (ticks == 0U)
    {
        hr_task_yield();
        return;
    }

    irq_state = hr_critical_enter();
    HR_ASSERT(g_current_task != (hr_task_t *)0);
    HR_ASSERT(g_current_task->in_ready_queue);

    ready_remove(g_current_task);
    g_current_task->wake_tick = g_kernel_tick + ticks;
    g_current_task->state = HR_TASK_BLOCKED;
    hr_trace_record(HR_TRACE_DELAY,
                    g_current_task->id,
                    0xFFU,
                    g_kernel_tick);
    hr_port_request_context_switch();
    hr_critical_exit(irq_state);
}

void hr_task_block_current(void)
{
    const hr_irq_state_t irq_state = hr_critical_enter();

    HR_ASSERT(g_current_task != (hr_task_t *)0);
    HR_ASSERT(g_current_task->in_ready_queue);

    ready_remove(g_current_task);
    g_current_task->wake_tick = HR_WAIT_FOREVER;
    g_current_task->state = HR_TASK_BLOCKED;
    hr_port_request_context_switch();
    hr_critical_exit(irq_state);
}

bool hr_task_wake(hr_task_t *task)
{
    bool switch_required = false;
    const hr_irq_state_t irq_state = hr_critical_enter();

    if ((task != (hr_task_t *)0) &&
        (task->state == HR_TASK_BLOCKED))
    {
        task->wake_tick = HR_WAIT_FOREVER;
        task->state = HR_TASK_READY;
        ready_enqueue_tail(task);
        switch_required = (g_current_task == (hr_task_t *)0) ||
                          (task->priority < g_current_task->priority);
        if (switch_required)
        {
            hr_port_request_context_switch();
        }
    }

    hr_critical_exit(irq_state);
    return switch_required;
}

bool hr_task_wake_from_isr(hr_task_t *task)
{
    bool switch_required = false;
    const hr_irq_state_t irq_state = hr_critical_enter();

    if ((task != (hr_task_t *)0) &&
        (task->state == HR_TASK_BLOCKED))
    {
        task->wake_tick = HR_WAIT_FOREVER;
        task->state = HR_TASK_READY;
        ready_enqueue_tail(task);
        switch_required = (g_current_task == (hr_task_t *)0) ||
                          (task->priority < g_current_task->priority);
        hr_trace_record(HR_TRACE_ISR_WAKE,
                        (g_current_task != (hr_task_t *)0) ?
                            g_current_task->id : 0xFFU,
                        task->id,
                        g_kernel_tick);
        if (switch_required)
        {
            hr_port_request_context_switch();
        }
    }

    hr_critical_exit(irq_state);
    return switch_required;
}

hr_task_t *hr_scheduler_current(void)
{
    return g_current_task;
}

size_t hr_scheduler_task_count(void)
{
    return g_task_count;
}

hr_task_t *hr_scheduler_task_at(size_t index)
{
    if (index >= g_task_count)
    {
        return (hr_task_t *)0;
    }
    return g_all_tasks[index];
}

uint32_t hr_scheduler_ready_bitmap(void)
{
    return g_ready_bitmap;
}
