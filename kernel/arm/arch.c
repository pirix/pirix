#include <arm/config.h>
#include <pirix/paging.h>
#include <pirix/process.h>
#include <pirix/thread.h>

void arch_switch_context(thread* new_thread) {
    paging_activate_context(new_thread->process->context);
}
