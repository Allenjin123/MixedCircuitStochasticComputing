//
// Created by alaallen on 2/20/22.
//

#ifndef LFSR_BTOD_H
#define LFSR_BTOD_H
#include <string>
string DtoB(double dec,int k_prec = 4){
    string res(k_prec,'0');
    double weigh=0.5;
    for (int i=0;i<k_prec;i++) {
        if (dec >= weigh){
            res[i] = '1';
            dec -=weigh;
        }
        weigh=weigh/2;
    }
    return res;
}
string DtoB_int(double dec,int k_prec){
    string res(k_prec,'0');
    double weigh=pow(2,k_prec-1);
    for (int i=0;i<k_prec;i++) {
        if (dec >= weigh){
            res[i] = '1';
            dec -=weigh;
        }
        weigh=weigh/2;
    }
    return res;
}
double BtoD(std::string bin){
    double weigh=0.5;
    double res = 0;
    for (int i=0;i<bin.size();i++){
        res=res+(bin[i] - '0')*weigh;
        weigh = weigh/2;
    }
    return res;
}

Number DToBN(const float& dec,int k_prec){
    Number res;
    res.setNum(DtoB(dec,k_prec));
    res.setType(BC);

    return res;
}
#endif //LFSR_BTOD_H
