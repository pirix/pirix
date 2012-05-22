int syscall(int r0, int r1, int r2, int r3, int r14) {
    int id = ((int*)r14)[-1] & 0xffffff;
    return 0;
}
