#pragma once

#include <sys/types.h>

// subscript names for c_cc
#define VEOF    1
#define VEOL    2
#define VERASE  3
#define VINTR   4
#define VKILL   5
#define VMIN    6
#define VQUIT   7
#define VSTART  8
#define VSTOP   9
#define VSUSP  10
#define VTIME  11

// input flags
#define BRKINT 0x001
#define ICRNL  0x002
#define IGNBRK 0x004
#define IGNCR  0x008
#define IGNPAR 0x010
#define INLCR  0x020
#define INPCK  0x040
#define ISTRIP 0x080
#define IXANY  0x100
#define IXOFF  0x200
#define IXON   0x400
#define PARMRK 0x800

// output flags
#define OPOST   0x1
#define ONLCR   0x2
#define OCRNL   0x4
#define ONOCR   0x8
#define ONLRET  0x10
#define OFDEL   0x20
#define OFILL   0x40
#define NLDLY   0x80
#define   NL0   0x00
#define   NL1   0x80
#define CRDLY   0xf00
#define   CR0   0x100
#define   CR1   0x200
#define   CR2   0x400
#define   CR3   0x800
#define TABDLY  0xf000
#define   TAB0  0x1000
#define   TAB1  0x2000
#define   TAB2  0x4000
#define   TAB3  0x8000
#define BSDLY   0x30000
#define   BS0   0x10000
#define   BS1   0x20000
#define VTDLY   0xc0000
#define   VT0   0x40000
#define   VT1   0x80000
#define FFDLY   0x300000
#define   FF0   0x100000
#define   FF1   0x200000

// baud rates
#define B0      0
#define B50     1
#define B75     2
#define B110    3
#define B134    4
#define B150    5
#define B200    6
#define B300    7
#define B600    8
#define B1200   9
#define B1800  10
#define B2400  11
#define B4800  12
#define B9600  13
#define B19200 14
#define B38400 15

// control modes
#define CSIZE   0xf
#define   CS5   0x1
#define   CS6   0x2
#define   CS7   0x4
#define   CS8   0x8
#define CSTOPB  0x10
#define CREAD   0x20
#define PARENB  0x40
#define PARODD  0x80
#define HUPCL   0x100
#define CLOCAL  0x200

// local modes
#define ECHO   0x1
#define ECHOE  0x2
#define ECHOK  0x4
#define ECHONL 0x8
#define ICANON 0x10
#define IEXTEN 0x20
#define ISIG   0x40
#define NOFLSH 0x80
#define TOSTOP 0x100

// tcsetattr() constants
#define TCSANOW   1
#define TCSADRAIN 2
#define TCSAFLUSH 3

// tcflush() constants
#define TCIFLUSH  1
#define TCIOFLUSH 2
#define TCOFLUSH  3

// tcflow() constants
#define TCIOFF 1
#define TCION  2
#define TCOOFF 3
#define TCOON  4

#define NCCS 32

typedef unsigned char cc_t;
typedef unsigned int speed_t;
typedef unsigned int tcflag_t;

struct termios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t c_cc[NCCS];
};

speed_t cfgetispeed(const struct termios*);
speed_t cfgetospeed(const struct termios*);
int     cfsetispeed(struct termios*, speed_t);
int     cfsetospeed(struct termios*, speed_t);
int     tcdrain(int);
int     tcflow(int, int);
int     tcflush(int, int);
int     tcgetattr(int, struct termios*);
pid_t   tcgetsid(int);
int     tcsendbreak(int, int);
int     tcsetattr(int, int, const struct termios*);
