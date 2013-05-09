file build/i386/kernel/kernel
target remote localhost:1234

define pid
  print current_thread->process->pid
end
