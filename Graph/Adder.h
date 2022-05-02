//
// Created by alaallen on 2/22/22.
//

#ifndef LFSR_ADDER_H
#define LFSR_ADDER_H

#include "Operator.h"
#include "Randomizer.h"
class Adder:public Operator{
public:
    Adder(int index, bool precTrun = true): Operator(index, Add,precTrun){
    }
    Adder(int index,CalType type,vector<Operator *> PrevOps = {},bool precTrun = true): Operator(index,type,std::move(PrevOps),Add,precTrun){
        energy = type == BC? 1.5:2;
    }
    Number operate() override{
        preOperate();
        Number x= input[0];
        Number y= input[1];
        assert(x.getNum().size() == y.getNum().size());
        int n = x.getNum().size();

        n = type == BC && precTrun == false ? n+1:n;

        Number res_number = Number();
        string res_value(n,'0');

        switch (type) {
            case SC: {
                auto randomizer = Randomizer();
                auto const_number = Number();
                const_number.setType(BC);
                const_number.setNum(DtoB(0.5));
                const_number = randomizer.transformPrec(const_number,n);
                res_number.setType(SC);
                for (int i = 0; i < n; i++) {
                    res_value[i] = (const_number.getNum()[i] - '0') == 1 ? y.getNum()[i] : x.getNum()[i];
                }
                break;
            }
            case BC: {
                res_number.setType(BC);
                res_value = DtoB(BtoD(x.getNum())  + BtoD(y.getNum()) , n);
                break;
            }
            default:
                break;
        }
        res_number.setNum(res_value);
        output = res_number;
        return res_number;
    }
    void changeEnergy(){
        energy = type == BC? 1.5:2;
    };

};

#endif //LFSR_ADDER_H
