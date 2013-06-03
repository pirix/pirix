#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/syscall.h>

#define PTHREAD_STACK_SIZE 1024

int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*func)(void*), void* arg) {
    char* stack = malloc(PTHREAD_STACK_SIZE);
    void* stack_top = (void*)(stack + PTHREAD_STACK_SIZE);
    *thread = syscall(SYS_CLONE, func, stack_top, 0);
    return 0;
}

void pthread_exit(void* value) {

}
