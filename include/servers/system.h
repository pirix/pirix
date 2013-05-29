#pragma once

#define PATHMGR_RESOLVE 0x1
#define PATHMGR_ATTACH  0x2
#define PATHMGR_DETACH  0x3

struct pathmgr_msg {
    int tag;
    char path[1];
};

union system_msg {
    int tag;
    struct pathmgr_msg pm;
};
