#include <arch/i386/config.h>
#include <arch/i386/tls.h>
#include <pirix/process.h>
#include <pirix/thread.h>
#include <pirix/kernel.h>

void arch_switch_context(thread* new_thread) {
    paging_activate_pagedir(new_thread->process->as->pagedir);
    tls_activate(new_thread->tls);
}
