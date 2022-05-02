//
// Created by alaallen on 4/27/22.
//

#ifndef GAUSSIANBLUR_RANDOMIZERSOBEL_H
#define GAUSSIANBLUR_RANDOMIZERSOBEL_H
#include "RNGSobol.h"
#include "Transformer.h"
class RandomizerSobol:public Transformer{
private:
    RNGSobol rngSobol;
public:
    RandomizerSobol(): Transformer(){
    }

    Number transform(Number B){
        assert(B.getType() == BC);
        int n_S = pow(2,B.getNum().size());
        Number X(n_S);
        for (int i=0;i<n_S;i++) {
            this->rngSobol.nextSeed();
            float a = this->rngSobol.generate01()*2;
            float b = BtoD(B.getNum());
            X.setBit(a< b ? '1':'0',i);
            //cout<<a<<" "<<b<<endl;
            //getchar();
            this->rngSobol.nextSeed();
        }
        X.setType(SC);
        return X;
    }
    Number transformPrec(Number x,int y){
        return x;
    }
};
#endif //GAUSSIANBLUR_RANDOMIZERSOBEL_H
