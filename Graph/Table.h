//
// Created by alaallen on 3/1/22.
//

#ifndef LFSR_TABLE_H
#define LFSR_TABLE_H
#include <unordered_map>
#include "Randomizer.h"
#include "Adder.h"
#include "Multiplier.h"
#include <string>
#define NLENGTH 3
class Table{
private:
    int n; // n_bit binary
    int n_lfsr; // 0 means default
    int seed; // 0 == 0001 1 == 0010
    int tap;
    std::unordered_map<std::string,std::string> table;
    // key: "Number1Number2" eg "101110" value: "ResAddResMul" eg "110111"

public:
    Table(int n,int n_lfsr=0,int seed=0,int tap=0):n(n),n_lfsr(n_lfsr),seed(seed),tap(tap){
    }

    void init(){

        auto adder = Adder(0,SC,{});
        auto muler = Multiplier(0,SC,{});
        string key,value;
        for (int i=0;i<pow(2,2*n);i++){
            auto rand = Randomizer();
            auto rand1 = Randomizer();
            auto derand = DeRandomizer();
            auto derand1 = DeRandomizer();
            Number x(4),y(4),z(16);
            x.setType(BC);
            y.setType(BC);
            key = DtoB_int(i,2*n);
            x.setNum(key.substr(0,n));
            y.setNum(key.substr(n,n));
            x=rand.transform(x);
            y=rand1.transform(y);
            adder.addInputs(x,y);
            muler.addInputs(x,y);
            //cout << adder.operate().getNum()<<endl;
            z.setNum(adder.operate().getNum()); // muler.operate().getNum();
            z.setType(SC);
            z=derand.transform(z);
            value = z.getNum();
            z.setNum(muler.operate().getNum());
            z.setType(SC);
            z=derand1.transform(z);
            value+=z.getNum();
            adder.clearInput();
            muler.clearInput();
            table.insert({key,value});
        }
    }

    void nice_print(){
        auto print_key_value = [](std::string const& key, std::string const& value) {
            std::cout << "Key:[" << key << "] Value:[" << value << "]\n";
        };

        std::cout << "Iterate and print keys and values of unordered_map, being\n"
                     "explicit with the type of the iterator, n:\n";
        for( const std::pair<const std::string, std::string>& n_ : this->table ) {
            print_key_value(n_.first, n_.second);
        }
        std::cout << "\n";
    }
    string getAddStr(const string& x,const string& y){
        return table[x+y].substr(0,n);
    }
    string getMulStr(const string& x,const string& y){
        return table[x+y].substr(n,n);
    }
    string getStr(const string& x,const string& y,OpType type){
        assert(type == 0 || type == 1);
        if (type == Add){ //add
            return getAddStr(x,y);
        } else{
            return getMulStr(x,y);
        }
    }
    Number getAddNum(const Number& x,const Number& y){
        assert(x.getType() == y.getType());
        assert(x.getType() == BC);
        return {getAddStr(x.getNum(),y.getNum()),BC};
    }
    Number getMulNum(const Number& x,const Number& y){
        assert(x.getType() == y.getType());
        assert(x.getType() == BC);
        return {getMulStr(x.getNum(),y.getNum()),BC};
    }
    Number getNum(const Number& x,const Number& y,OpType type){
        assert(type == 0 || type == 1);
        if (type == Add){ //add
            return getAddNum(x,y);
        } else{
            return getMulNum(x,y);
        }
    }
};

#endif //LFSR_TABLE_H
