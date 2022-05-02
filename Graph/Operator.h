//
// Created by alaallen on 2/20/22.
//

#ifndef LFSR_OPERATOR_H
#define LFSR_OPERATOR_H
#include <memory>
#include <utility>
#include <vector>
#include <cassert>
#include <memory>
#include "VoidTransformer.h"
#include "Randomizer.h"
#include "DeRandomizer.h"

#define INPUTSIZE 2
enum OpType{
    Add,Mul
};
class Operator{
protected:
    int index;
    bool precTrun;
    CalType type;
    OpType optype;
    vector<Number> input;
    Number output;
    vector<Operator *> PrevOps;
    vector<std::unique_ptr<Transformer>> PrevTrans;
    double energy;
public:
    Operator(int index,OpType opType, bool precTrun = true): index(index),precTrun(precTrun),type(VOIDCAL),optype(opType),
    input(), output(),
    PrevOps(),PrevTrans(),energy(0)
    {
    }
    Operator(int index,CalType type,vector<Operator *> PrevOps,OpType opType,bool precTrun = true):optype(opType),
    index(index),precTrun(precTrun),
    type(type),
    input(), output(),
    PrevOps(std::move(PrevOps)),PrevTrans(),energy(0)
    {
    }
    virtual Number operate() = 0;
    void addInput(const Number& x){
        this->input.emplace_back(x);
    }
    void addInputs(const Number& x,const Number& y){
        assert(this->input.empty());
        this->input.emplace_back(x);
        this->input.emplace_back(y);
    }
    void fillInput(){
        assert(input.size() <= INPUTSIZE);
        int i = INPUTSIZE - input.size();
        for (int j=0;j<i;j++){
            input.emplace_back(this->PrevOps[j]->output);
        }
    }
    CalType getType() const{
        return type;
    }

    void setType(CalType type_){
        this->type = type_;
    }

    OpType getOpType() const{
        return optype;
    }

    void setOpType(OpType type_){
        this->optype = type_;
    }
    void setOutput(Number out){
        output = out;
    }
    Number getOutput(){
        return output;
    }
    virtual void changeEnergy() = 0;

    double getEnergy(){
        return energy;
    }

    int getInputNum() const{
        return input.size();
    }

    int getIndeg() const{
        return PrevOps.size();
    }

    int getIndex() const{
        return index;
    }

    vector<Operator *> getPrevOps(){
        return PrevOps;
    }
    vector<Number> getInput(){
        return input;
    }

    void refresh(){
        this->input.clear();
        this->PrevTrans.clear();
        this->energy = 0;
    }

    void clearInput(){
        this->input.clear();
    }

    void preOperate(){
        fillInput();

        //int n_prec = input[0].getNum().size() > input[1].getNum().size()? input[1].getNum().size():input[0].getNum().size();
        assert(input.size() == INPUTSIZE);
        // Transformers share the same sequence with Input
        for (auto & in:input){
            assert(in.getType() != VOIDCAL);
            if (this->type != in.getType())
                switch (this->type) {
                    case SC: {
                        this->PrevTrans.emplace_back(new Randomizer());
                        this->energy += 2;
                        //n_prec = pow(2,n_prec);
                        break;
                    }
                    case BC: {
                        this->PrevTrans.emplace_back(new DeRandomizer());
                        this->energy += 1;
                        //n_prec = log2(n_prec);
                        break;
                    }
                    case VOIDCAL: {
                        break;
                    }
                }
            else
                this->PrevTrans.emplace_back(new VoidTransformer());
            in = this->PrevTrans.back()->transform(in);
        }

        int min_len = this->input[0].getNum().size();

        for (int k=1;k<INPUTSIZE;k++){
            if (this->input[k].getNum().size() < min_len){
                min_len = this->input[k].getNum().size();
            }
        }

        for (int k=0;k<INPUTSIZE;k++){
            this->input[k].setNum(this->input[k].getNum().erase(min_len));
        }

    }
};
#endif //LFSR_OPERATOR_H
