#include <signal.h>

int sigprocmask(int how, const sigset_t* set, sigset_t* oldset) {
    return -1;
}

int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact) {
    return 0;
}
