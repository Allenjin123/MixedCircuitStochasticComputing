//
// Created by alaallen on 2/22/22.
//

#ifndef LFSR_MULTIPLIER_H
#define LFSR_MULTIPLIER_H
#include "Operator.h"
#include "Randomizer.h"


class Multiplier:public Operator{
public:
    Multiplier(int index,bool precTrun = true): Operator(index,Mul,precTrun){
    }
    Multiplier(int index,CalType type,const vector<Operator *>& PrevOps= {}, bool precTrun = true):
    Operator(index,type,PrevOps,Mul,precTrun){
        energy = type == BC? 10:1;
    }
    Number operate() override{
        preOperate();


        Number x= input[0];
        Number y= input[1];

        assert(x.getNum().size() == y.getNum().size());
        int n = x.getNum().size();
        n = type == BC && precTrun == false ? n*2:n;
        Number res_number = Number();
        string res_value(n,'0');
        switch (type) {
            case SC: {
                res_number.setType(SC);
                for (int i = 0; i < n; i++) {
                    res_value[i] = ((y.getNum()[i] - '0') & (x.getNum()[i] - '0')) + '0';
                }
                break;
            }
            case BC: {
                res_number.setType(BC);
                res_value = DtoB(BtoD(x.getNum()) * BtoD(y.getNum()), n);
                break;
            }
            default:
                break;
        }
        res_number.setNum(res_value);
        output = res_number;
//        auto yyy = DeRandomizer();
//        cout<<BtoD(yyy.transform(input[0]).getNum()) << endl;
//        cout<<BtoD(yyy.transform(input[1]).getNum())  << endl;
//        cout<<BtoD(yyy.transform(res_number).getNum())  << endl;
//        getchar();
        return res_number;
    }
    void changeEnergy(){
        energy = type == BC? 5:1;
    };
};




#endif //LFSR_MULTIPLIER_H
