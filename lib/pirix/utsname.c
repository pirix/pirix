#include <sys/utsname.h>
#include <pirix.h>

int uname(struct utsname* name) {
    return sys_uname(name);
}
