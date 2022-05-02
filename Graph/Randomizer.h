//
// Created by alaallen on 2/20/22.
//

#ifndef LFSR_RANDOMIZER_H
#define LFSR_RANDOMIZER_H
#include "LFSR.h"
#include "Transformer.h"
class Randomizer:public Transformer{
private:
    LFSR lfsr;
    bool init;
public:
    Randomizer(): Transformer(){
        init = false;
    }

    Randomizer(int n_lfsr): Transformer(), lfsr(n_lfsr){
        init = true;
        lfsr.auto_set();
    }
    Randomizer(int n_lfsr,int i_lfsr): Transformer(), lfsr(n_lfsr){
        init = true;
        lfsr.auto_set(i_lfsr);
    }

    Number transform(Number B){
        assert(B.getType() == BC);
        int n_S = pow(2,B.getNum().size());
        if (init == false){
            int n_lfsr = ceil(log2(n_S+1));
            lfsr = LFSR(n_lfsr);
            lfsr.auto_set();
            init = true;
        }
        Number X(n_S);
        for (int i=0;i<n_S;i++) {
            this->lfsr.step();
            X.setBit(BtoD(this->lfsr.toString())< BtoD(B.getNum()) ? '1':'0',i);
        }
        X.setType(SC);
        return X;
    }
    Number transformPrec(Number B,int n_S){
        assert(B.getType() == BC);
        int n_lfsr = ceil(log2(n_S+1));
        lfsr = LFSR(n_lfsr);
        lfsr.auto_set();
        Number X(n_S);
        for (int i=0;i<n_S;i++) {
            this->lfsr.step();
            X.setBit(BtoD(this->lfsr.toString())< BtoD(B.getNum()) ? '1':'0',i);}
        X.setType(SC);
        return X;
    }

    Number transformLog(Number B, string &log){
        assert(B.getType() == BC);
        int n_S = pow(2,B.getNum().size());
        if (init == false){
            int n_lfsr = ceil(log2(n_S+1));
            lfsr = LFSR(n_lfsr);
            lfsr.auto_set();
            init = true;
        }
        Number X(n_S);
        for (int i=0;i<n_S;i++) {
            int j = this->lfsr.step();
            X.setBit(BtoD(this->lfsr.toString())< BtoD(B.getNum()) ? '1':'0',i);
            string tmp = to_string(i)+ "th step, value:"+ to_string(j) + string("\n lfsr:")+this->lfsr.toString() + "\n";
            log.append(tmp);
        }

        string tmp = "Final result: \n" + X.getNum() + "\n" + "correct value: " + to_string(BtoD(B.getNum())*255) +"\n";

        log.append(tmp);
        X.setType(SC);
        return X;
    }
    void setTap(int i=0){
        this->lfsr.setTap(i);
    }
    void printTap(){
        this->lfsr.printTap();
    }
    string getValue(){
        return this->lfsr.getValue();
    }
};
#endif //LFSR_RANDOMIZER_H
