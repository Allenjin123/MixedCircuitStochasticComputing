//
// Created by alaallen on 2/20/22.
//

#ifndef LFSR_TRANSFORMER_H
#define LFSR_TRANSFORMER_H
#include "Number.h"
#include "BtoD.h"
class Transformer{
    // bit length of input is got from input number
public:
    Transformer(){
    }
    virtual Number transform(Number)=0;
    virtual Number transformPrec(Number,int)=0;

    // enable manually set the output precision
};
#endif //LFSR_TRANSFORMER_Hjnnnnnnj
