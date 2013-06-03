#include <pirix/paging.h>
#include <pirix/process.h>
#include <pirix/thread.h>
#include <arch/i386.h>
#include <arch/i386/tls.h>

void arch_switch_context(thread* new_thread) {
    paging_activate_context(new_thread->process->context);
    tls_activate(new_thread->tls);
}
