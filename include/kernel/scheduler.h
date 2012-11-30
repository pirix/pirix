#pragma once

#include "cpu.h"
#include "thread.h"

void scheduler_switch();
thread* scheduler_current();
thread* scheduler_dequeue_thread();
void scheduler_enqueue_thread(thread* new_thread);
cpu_state* scheduler_schedule(cpu_state* state);
void task_switch(cpu_state* state);
