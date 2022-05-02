#include <iostream>
#include "LFSR.h"
#include "Adder.h"
#include "Multiplier.h"
#include "Graph.h"
#include "Pareto.h"
#include <chrono>
#include "Table.h"

using namespace std;
using namespace std::chrono;
int main() {
    cout << "Part 1" << endl;
    Number x = Number("110",BC); //"6/8"
    Number y = Number("010",BC); //"2/8"
    // +: 4/8 *: 3/16
    cout << "Binary" <<endl;
    auto BCAdder = Adder(0,BC,{}, false);
    BCAdder.addInput(x);
    BCAdder.addInput(y);
    cout <<"Adder without precision truncaiton "<< BCAdder.operate().getNum()<<endl;

    auto BCMul = Multiplier(0,BC,{},false);
    BCMul.addInput(x);
    BCMul.addInput(y);
    cout <<"Multiplier without precision truncaiton " <<BCMul.operate().getNum()<<endl;

    cout << "After Transformer" <<endl;
    auto BtoSTrans = Randomizer(4,1);
    x=BtoSTrans.transformPrec(x,8);
    y=BtoSTrans.transform(y);
    cout << x.getType() <<endl;
    cout << y.getType() <<endl;
    cout << x.getNum() <<endl;
    cout << y.getNum() <<endl;

    cout << "SC" <<endl;

    auto SCAdder = Adder(0,SC);
    SCAdder.addInputs(x,y);

    cout << SCAdder.operate().getNum()<<endl;
    auto SCMul = Multiplier(0,SC);
    SCMul.addInputs(x,y);
    cout << SCMul.operate().getNum()<<endl;
    cout << "Part 2" << endl;
    vector<Operator*> ops(8);
    vector<Number> inputs(7);
    ops[0] = new Multiplier(0,BC);
    ops[1] = new Multiplier(1,BC);
    ops[2] = new Adder(2,BC);
    ops[3] = new Adder(3,BC,{ops[0],ops[1]});
    ops[4] = new Multiplier(4,BC,{ops[1],ops[2]});
    ops[5] = new Adder(5,BC,{ops[3],ops[4]});
    ops[6] = new Multiplier(6,BC,{ops[3]});
    ops[7] = new Multiplier(7,BC,{ops[5],ops[6]});

    inputs[0] = Number("1111",BC);
    inputs[1] = Number("1111",BC);
    inputs[2] = Number("1111",BC);
    inputs[3] = Number("1111",BC);
    inputs[4] = Number("1111",BC);
    inputs[5] = Number("1111",BC);
    inputs[6] = Number("1111",BC);

    vector<Input> inputs_with_index(7);
    inputs_with_index[0] = {0,inputs[0]};
    inputs_with_index[1] = {0,inputs[1]};
    inputs_with_index[2] = {1,inputs[2]};
    inputs_with_index[3] = {1,inputs[3]};
    inputs_with_index[4] = {2,inputs[4]};
    inputs_with_index[5] = {2,inputs[5]};
    inputs_with_index[6] = {6,inputs[6]};

    Graph graph = Graph(4,ops);
    graph.addInput(inputs_with_index);
    graph.run();
    cout << endl;
    cout << "Part 3: " << endl;
    cout << endl;
    vector<CalType> newTypes(8);
    Pareto<double, std::string> pareto;
    const int nops = graph.getSize();
    const int nelem = powl(2,nops); // 5 operators -> 32 combinations
    const int nvals = 2; // error & energy
    vector<string> indices(nelem);
    std::vector<double> metrics ( nvals );
    printf ( "adding %d random elements\n", nelem );

    auto start = high_resolution_clock::now();
    for ( int i=0; i<nelem; i++ ) {
        indices[i] = DtoB_int(i,nops);
        for (int j=0;j<nops;j++){
            newTypes[j] = (CalType)(indices[i][j] - '0');
        }
        graph.changeType(newTypes);
        graph.addInput(inputs_with_index);
        graph.run(false);
        metrics[0] = graph.getEnergy();
        metrics[1] = graph.MonteCarlo(100,{0,0,1,1,2,2,6},4, true);
        pareto.addvalue ( metrics, indices[i] );
        printf ( "  element %s: ", indices[i].c_str());
        detail::display_vector ( metrics );
        printf ( "\n\n" );
    }

    printf ( "The pareto optimal values are:\n" );
    pareto.show ( 3 );
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "
         << (double )duration.count()/1000000.0 << " seconds" << endl;

    return 0;
}
