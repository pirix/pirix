unsigned __udivsi3(unsigned n, unsigned d) {
    unsigned q;
    unsigned r;
    unsigned sr;

    if (d == 0) return 0;
    if (n == 0) return 0;
    sr = __builtin_clz(d) - __builtin_clz(n);
    if (sr > 31) return 0;
    if (sr == 31) return n;
    ++sr;

    q = n << (32 - sr);
    r = n >> sr;
    unsigned carry = 0;
    for (; sr > 0; --sr) {
        r = (r << 1) | (q >> (31));
        q = (q << 1) | carry;
        const int s = (int)(d - r - 1) >> (31);
        carry = s & 1;
        r -= d & s;
    }

    q = (q << 1) | carry;
    return q;
}

int __divsi3(int a, int b) {
    int s_a = a >> 31;
    int s_b = b >> 31;
    a = (a ^ s_a) - s_a;
    b = (b ^ s_b) - s_b;
    s_a ^= s_b;
    return (__udivsi3(a, b) ^ s_a) - s_a;
}

int __modsi3(int a, int b) {
    return a - __divsi3(a, b) * b;
}

unsigned __umodsi3(unsigned a, unsigned b) {
    return a - __udivsi3(a, b) * b;
}
