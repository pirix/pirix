set osabi none

define pid
  print current_thread->process->pid
end

define connect
  target remote localhost:1234
end

define kernel
  file build/i386/kernel/kernel
end

define system
  file build/i386/servers/system/system
end

define login
  file build/i386/servers/login/login
end
