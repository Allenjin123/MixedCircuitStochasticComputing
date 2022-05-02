//
// Created by alaallen on 3/15/22.
//

#ifndef LFSR_CAADDER_H
#define LFSR_CAADDER_H

#include "Adder.h"
class CAAdder: public Adder{
public:
    CAAdder(int index): Adder(index, true){
        this->type = SC;
    }
    CAAdder(int index,vector<Operator *> PrevOps = {}): Adder(index,SC,std::move(PrevOps), true){
        energy =  1.5;
    }
    Number operate();

};


#endif //LFSR_CAADDER_H
