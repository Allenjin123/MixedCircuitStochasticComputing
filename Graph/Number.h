//
// Created by alaallen on 2/20/22.
//

#ifndef LFSR_NUMBER_H
#define LFSR_NUMBER_H
#include <string>
#include <utility>
enum CalType {
    SC,BC,VOIDCAL
};

class Number{
    private:
        string value;
        CalType type;
    public:
        Number(int n=4):value(n,'0'){
            type = BC;
        }
        Number(string value_, CalType type_){
            value = std::move(value_);
            type = type_;
        }
        string getNum() const{
            return value;
        }
        CalType getType() const{
            return type;
        }
        void setNum(string value_){
            this->value = value_;
        }
        void setBit(char bit,int n_bit){
            this->value[n_bit]=bit;
        }
        void setType(CalType type_){
            this->type = type_;
        }
};

struct Input{
    int index;
    Number num;
};
#endif //LFSR_NUMBER_H
