#include <pirix.h>
#include <termios.h>

speed_t cfgetispeed(const struct termios* tio) {
    return 0;
}

speed_t cfgetospeed(const struct termios* tio) {
    return 0;
}

int cfsetispeed(struct termios* tio, speed_t speed) {
    return 0;
}

int cfsetospeed(struct termios* tio, speed_t speed) {
    return 0;
}

int tcdrain(int a) {
    return 0;
}

int tcflow(int a, int b) {
    return 0;
}

int tcflush(int a, int b) {
    return 0;
}

int tcgetattr(int a, struct termios* tio) {
    return 0;
}

pid_t tcgetsid(int a) {
    return sys_getpid();
}

int tcsendbreak(int a, int b) {
    return 0;
}

int tcsetattr(int fd, int actions, const struct termios* tio) {
    return 0;
}
