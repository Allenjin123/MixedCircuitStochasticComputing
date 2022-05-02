//
// Created by alaallen on 2/20/22.
//

#ifndef LFSR_DERANDOMIZER_H
#define LFSR_DERANDOMIZER_H
#include "Transformer.h"
#include "cmath"
class DeRandomizer:public Transformer{
public:
    DeRandomizer() : Transformer() {
    }
    Number transform(Number B){
        assert(B.getType() == SC);
        int n_B = log2(B.getNum().size());
        Number X(n_B);
        int counter = 0;
        for (int i=0;i<B.getNum().size();i++) {
            counter+=(B.getNum()[i] - '0');
        }
        double value =(double) counter / (double) B.getNum().size();
        X.setNum(DtoB(value,n_B));
        X.setType(BC);
        return X;
    }
    Number transformPrec(Number B, int n_B){
        assert(B.getType() == SC);
        Number X(n_B);
        int counter = 0;
        for (int i=0;i<B.getNum().size();i++) {
            counter+=(B.getNum()[i] - '0');
        }
        double value =(double) counter / (double) B.getNum().size();
        X.setNum(DtoB(value,n_B));
        X.setType(BC);
        return X;
    }
};

double NumToD(const Number& tmp){
    if (tmp.getType() == BC) {
        return BtoD(tmp.getNum());
    } else {
        // SN
        auto derand = DeRandomizer();
        return BtoD(derand.transform(tmp).getNum());
    }
}
#endif //LFSR_DERANDOMIZER_H
