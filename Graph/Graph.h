//
// Created by alaallen on 2/22/22.
//

#ifndef LFSR_GRAPH_H
#define LFSR_GRAPH_H
#include <utility>
#include <vector>
#include <stack>
#include <queue>
#include <ctime>
#include <cstdlib>
#include "Operator.h"
#include "Table.h"
class Graph{
private:
    double totalEnergy;
    double totalError;
    std::vector<Operator*> operators;
    Table LookUpTable;
    std::stack<int> sequence;
public:
    Graph(int n_B):operators({}),totalEnergy(0),totalError(0),LookUpTable(n_B){
    }
    Graph(int n_B,vector<Operator*> ops):operators(std::move(ops)),totalEnergy(0),totalError(0),LookUpTable(n_B){
        topologicalSort();
        this->LookUpTable.init();
    }

    void addOperators(vector<Operator*> ops){
        operators = std::move(ops);
        topologicalSort();
        //this->LookUpTable.init();
    }
    void topologicalSort(){
        vector<int> out_deg(operators.size(),0);
        for (int i=0;i<operators.size();i++){
            for (auto in_op:operators[i]->getPrevOps()){
                out_deg[in_op->getIndex()]++;
            }
        }
        queue<int> to_be_visited;
        for (int i=0;i< operators.size();i++){
            if (out_deg[i] == 0)
                to_be_visited.push(i);
        }
        while (!to_be_visited.empty()){
            int to_be_visited_op = to_be_visited.front();
            sequence.push(to_be_visited_op);
            to_be_visited.pop();
            for (auto in_op:operators[to_be_visited_op]->getPrevOps()){
                out_deg[in_op->getIndex()]--;
                if (out_deg[in_op->getIndex()] == 0)
                    to_be_visited.push(in_op->getIndex());
            }
        }
    }
    void nice_print(vector<Number> output){
        cout << "Summary: "<<endl;
        cout << "Total Energy: " << totalEnergy << endl;
//        cout << "Total Error: " << totalError << endl;
        auto seq = this->sequence;
        while (!seq.empty()){
            int i = seq.top();
            seq.pop();
            cout << i << " th operator: "<< endl;
            cout << "Type:" << operators[i]->getType()<<endl;
            cout << "Input 1:"<< operators[i]->getInput()[0].getNum() << " with type "<<operators[i]->getInput()[0].getType()<< endl;
            cout << "Input 2:"<< operators[i]->getInput()[1].getNum() << " with type "<<operators[i]->getInput()[1].getType()<< endl;
            cout << "Output:" << output[i].getNum()<< " with type "<<output[i].getType()<< endl;
        }
    }

    void addInput(int index,const Number& x){
        this->operators[index]->addInput(x);
    }

    void addInput(const Input& x){
        this->operators[x.index]->addInput(x.num);
    }

    void addInput(const vector<Input>& x){
        for (const auto& i:x)
            this->operators[i.index]->addInput(i.num);
    }
    float run(bool print = true,bool app= false){
        vector<Number> output(this->sequence.size());
        auto seq = this->sequence;
        while (!seq.empty()) {
            int j = seq.top();
            seq.pop();
            Number tmp;
            if (!app || operators[j]->getType() == BC) {
                tmp = operators[j]->operate();
            }
            else{
                operators[j]->fillInput();
                operators[j]->setOutput(this->LookUpTable.getNum(operators[j]->getInput()[0],operators[j]->getInput()[1],operators[j]->getOpType()));
                tmp=operators[j]->getOutput();
            }
            output[j]=tmp;
        }

        calEnergy();
        if (print)
            nice_print(output);
        return NumToD(output.back());
    }

    void changeType(vector<CalType> types){
        this->totalError = 0;
        this->totalEnergy = 0;
        for (int j=0;j<operators.size();j++){
            operators[j]->setType(types[j]);
            operators[j]->refresh();
            operators[j]->changeEnergy();
        }
    }

    void calEnergy(){
        for (auto & j : operators){
            this->totalEnergy += j->getEnergy();
        }
    }

//    double calErrorMag(const Number &x){
//        int sign = 0;
//        for (int j=0;j<operators.size();j++){
//            if (operators[j]->getType() !=BC ) {
//                sign = 1;
//                break;
//            }
//        }
//        if (sign == 0){
//            if (x.getType() == BC) {
//                corretOutput = BtoD(x.getNum());
//            } else {
//                // SN
//                auto derand = DeRandomizer();
//                corretOutput = BtoD(derand.transform(x).getNum());
//            }
//            totalError = 0;
//        } else{
//            if (x.getType() == BC) {
//                totalError = abs(BtoD(x.getNum()) - corretOutput);
//            } else {
//                // SN
//                auto derand = DeRandomizer();
//                totalError = abs(BtoD(derand.transform(x).getNum()) -corretOutput);
//            }
//        }
//        return totalError;
//    }
    double getEnergy(){
        return this->totalEnergy;
    }

    int getSize(){
        return this->operators.size();
    }

    double getError(){
        return this->totalError;
    }





    double getCorrectOutput(const vector<Input>& inputs){
        string indices = DtoB_int(pow(2,this->getSize())-1,this->getSize());
        vector<CalType> newTypes(this->getSize());
        vector<CalType> oldTypes(this->getSize());
        for (int j=0;j<this->getSize();j++){
            newTypes[j] = (CalType)(indices[j] - '0');
        }
        for (int j=0;j<operators.size();j++){
            oldTypes[j] = operators[j]->getType();
            operators[j]->setType(newTypes[j]);
            operators[j]->refresh();
        }
        this->addInput(inputs);
        Number tmp;
        auto seq = this->sequence;
        while (!seq.empty()) {
            int j = seq.top();
            seq.pop();
            tmp = operators[j]->operate();
        }
        for (int j=0;j<operators.size();j++){
            operators[j]->setType(oldTypes[j]);
            operators[j]->refresh();
        }
        return NumToD(tmp);
    }

    void clearInput(){
        for (auto &k:operators){
            k->clearInput();
        }
    }
    double getRealOutput(const vector<Input>& inputs,bool app = true){
        clearInput();
        this->addInput(inputs);
        Number tmp;
        auto seq = this->sequence;
        while (!seq.empty()) {
            int j = seq.top();
            seq.pop();
            if (!app || operators[j]->getType() == BC) {
                tmp = operators[j]->operate();
            }
            else{
                operators[j]->fillInput();
                operators[j]->setOutput(this->LookUpTable.getNum(operators[j]->getInput()[0],operators[j]->getInput()[1],operators[j]->getOpType()));
                tmp=operators[j]->getOutput();
            }
        }
        return NumToD(tmp);
    }
    double MonteCarlo(int times,vector<int> indexInput,int k_prec,bool app = false){
        srand((unsigned) time(nullptr));
        vector<Input> inputs(indexInput.size());
        vector<Input> inputs_prec(indexInput.size());
        double total_error = 0;
        for (int i=0;i<times;i++){
            // Random input
            for (int j=0;j<indexInput.size();j++){
                double v = (double)rand()/RAND_MAX;
                inputs[j] = {indexInput[j], Number(DtoB(v,k_prec),BC)};
                inputs_prec[j] = {indexInput[j], Number(DtoB(v,k_prec),BC)};
            }
            // get real answer
            double realOutput = this->getRealOutput(inputs,app);
            // get correct answer
            double correctOutput = this->getCorrectOutput(inputs_prec);
            // Add Input
            total_error += abs(realOutput-correctOutput);
        }
        this->totalError = total_error/times;
        return this->totalError;
    }


};
#endif //LFSR_GRAPH_H
