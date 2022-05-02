//
// Created by alaallen on 3/19/22.
//

#ifndef GAUSSIANBLUR_TRUERANDOMIZER_H
#define GAUSSIANBLUR_TRUERANDOMIZER_H
#include "LFSR.h"
#include "Transformer.h"
class TrueRandomizer:public Transformer{
private:
public:
    TrueRandomizer(): Transformer(){
    }


    Number transform(Number B){
        assert(B.getType() == BC);
        int n_S = pow(2,B.getNum().size());
        Number X(n_S);
        for (int i=0;i<n_S;i++) {
            X.setBit((float)random()/RAND_MAX< BtoD(B.getNum()) ? '1':'0',i);
        }
        X.setType(SC);
        return X;
    }
    Number transformPrec(Number B,int n_S){
        assert(B.getType() == BC);
        Number X(n_S);
        for (int i=0;i<n_S;i++) {
            X.setBit((float)random()/RAND_MAX< BtoD(B.getNum()) ? '1':'0',i);
        }
        X.setType(SC);
        return X;
    }
};
#endif //GAUSSIANBLUR_TRUERANDOMIZER_H
