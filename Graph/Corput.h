double corput(int n, int base){
    double q=0, bk=(double)1/base;

    while (n > 0) {
        q += (n % base)*bk;
        n /= base;
        bk /= base;
    }

    return q;
}