//
// Created by alaallen on 2/20/22.
//

#ifndef LFSR_VOIDTRANSFORMER_H
#define LFSR_VOIDTRANSFORMER_H
#include "Transformer.h"
class VoidTransformer:public Transformer{
public:
    VoidTransformer(): Transformer(){}
    Number transform(Number x) override{
        return x;
    }
    Number transformPrec(Number x,int nPrec) override{
        return x;
    }
};
#endif //LFSR_VOIDTRANSFORMER_H
