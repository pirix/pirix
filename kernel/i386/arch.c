#include <i386/config.h>
#include <i386/tls.h>
#include <pirix/process.h>
#include <pirix/thread.h>
#include <pirix/kernel.h>

void arch_switch_context(thread* new_thread) {
    paging_activate_context(new_thread->process->context);
    tls_activate(new_thread->tls);
}
