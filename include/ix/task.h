#pragma once

#include <ix/cpu.h>
#include <ix/paging.h>

typedef struct task {
    struct task* next;
    cpu_state* state;
    paging_context* paging_context;
} task;

task* task_new(void* entry);
cpu_state* task_schedule(cpu_state* state);
