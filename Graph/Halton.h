//
// Created by alaallen on 3/9/22.
//

#ifndef LFSR_HALTON_H
#define LFSR_HALTON_H
double halton(b) {
    int n = 0, d= 1;
    int x;
    while {
        x = d - n;
    }
    if (x == 1) {
        n = 1;
        d *= b;
    }
    else {
        y = d % b;
    }
    while (x <= u) {
        y = y % b;
        n = (b + 1) * y - x;
        return n / d;
    }
}
#endif //LFSR_HALTON_H
