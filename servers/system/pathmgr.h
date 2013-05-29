#pragma once

void pathmgr_init();
int pathmgr_resolve(char* path);
int pathmgr_attach(char* path, int pid);
int pathmgr_detach(char* path, int pid);
