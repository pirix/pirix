#include <pirix/task.h>
#include <pirix/kheap.h>

#define STACK_SIZE 4096

static task* first_task = 0;
static task* current_task = 0;

task* task_new(void* entry) {
    char* usr_stack = kmalloc(STACK_SIZE);
    char* irq_stack = kmalloc(STACK_SIZE);

    cpu_state* new_state = (cpu_state*)(irq_stack + STACK_SIZE) - 1;

    *new_state = (cpu_state) {
        .usr_r13 = (unsigned)(usr_stack + STACK_SIZE),
        .cpsr = 0x00000050,
        .r15 = (unsigned)entry
    };

    task* new_task = kmalloc(sizeof(task));
    new_task->state = new_state;
    new_task->next = first_task;
    new_task->paging_context = paging_create_context();
    first_task = new_task;

    return new_task;
}

void task_exit(int retval) {
    task* t = first_task;
    while (t->next != current_task) {
        t = t->next;
    }
    t->next = current_task->next;
    return;
}

cpu_state* task_schedule(cpu_state* state) {
    if (current_task) {
        current_task->state = state;
    }

    if (current_task && current_task->next) {
      current_task = current_task->next;
    }
    else if (first_task) {
      current_task = first_task;
    }
    else {
        return state;
    }

    paging_activate_context(current_task->paging_context);

    return current_task->state;
}
