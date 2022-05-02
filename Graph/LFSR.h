//
// Created by alaallen on 2/19/22.
//

#ifndef LFSR_LFSR_H
#define LFSR_LFSR_H
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;
class LFSR {
private:
    int n;
    string seed;
    string value;
    vector<int> tap;
    vector<vector<int>> taps;
    // creates an LFSR with the specified seed and tap
public:
    LFSR(int n_ = 3,string seed_ = "0", vector<int> tap_ = {1}) : seed(seed_), tap(tap_), value(seed_),n(n_) {}

    // returns the number of bits in the LFSR.
    unsigned length() {
        return seed.length();
    }

    // returns bit i as 0 or 1.
    int bitAt(int i) {
        return value[i - 1] - '0';
    }


    // returns a string representation of this LFSR
    string toString() {
        return value;
    }


    // simulates one step of this LFSR and return the new bit as 0 or 1
    int step() {
        int t = value[this->tap[0]-1] - '0';
        for (int j = 1; j < this->tap.size(); j++) {
            t = t ^ (this->value[this->tap[j]-1] - '0');
        }
        for (int i = value.length() - 1; i > 0; i--)
            value[i] = value[i - 1];
        value[0] = t + '0';
        return t;
    }


    // simulates k steps of this LFSR and return the k bits as a k-bit integer
    int generate(int k) {
        for (int i = 0; i < k; i++)
            this->step();
        return strtoul(value.c_str(), NULL, 2);
    }

    bool LFSR_check(const vector<int> &para,int n_) {
        string g(n_, '0');
        g[n_ - 1] = '1';
        int m = 0;
        vector<string> rec;
        rec.reserve(pow(2, n_) - 1);
        for (int i = 0; i < pow(2, n_) - 1; i++) {
            rec.emplace_back(n_, '0');
        }
        bool sign = true;
        for (int i = 1; i <= pow(2, n_) - 1; i++) {
            rec[m] = g;
            string seed_ = g;
            for (int f = n_ - 1; f > 0; f--) {
                g[f] = g[f - 1];
            }
            int t = seed_[para[0] - 1] - '0';
            for (int j = 1; j < para.size(); j++) {
                t = t ^ (seed_[para[j] - 1] - '0');
            }
            g[0] = t + '0';
            for (int k = 0; k <= m; k++) {
                if (g == rec[k] && i != pow(2, n_) - 1) {
                    sign = false;
                    break;
                }
            }
            if (sign == 0) {
                break;
            }
            m++;
        }
        return sign;
    }

    vector<vector<int>> nchoosek(int n_, int k){
        vector<vector<int>> combinations;
        vector<int> combination;
        nchoosek_helper(1,k,n_,combinations,combination);
        return combinations;
    }

    void nchoosek_helper(int offset, int k,int n_,vector<vector<int>> &combinations, vector<int> &combination) {
        if (k == 0) {
            combinations.emplace_back(combination);
            return;
        }
        for (int i = offset; i <= n_ - k + 1; ++i) {
            combination.push_back(i);
            nchoosek_helper(i+1, k-1,n_,combinations,combination);
            combination.pop_back();
        }
    }
    vector<vector<int>> LFSR_search(int n_) {
        vector<vector<int>> record;
        for (int i = 2; i <= n_; i++) {
            vector<vector<int>> C;
            C = nchoosek(n_, i);
            int a = C.size();
            for (int j = 0; j < a; j++) {
                bool sign = LFSR_check(C[j], n_);
                if (sign == 1) {
                    record.emplace_back(C[j]);
                }
            }
        }
        return record;
    }

    void nice_print(const vector<vector<int>> &res){
        for (const auto& i:res){
            for (const auto &j:i){
                cout<<j<<" ";
            }
            cout<< endl;
        }
    }
    void auto_set(int i=0){
        auto s = LFSR_search(n);
        taps = s;
        tap= s[i%s.size()];
        string g(n, '0');
        g[n - 1] = '1';
        seed=g;
        value=seed;
    }

    void setTap(int i=1){
        assert(!taps.empty());
        tap = taps[i%taps.size()];
    }

    void print_taps(){
        cout<<endl;
        nice_print(this->taps);
        cout<<endl;
    }

    string getValue(){
        return value;
    }
    void printTap(){
        for (const auto &j:this->tap){
            cout<<j<<" ";
        }
    }
};
// tests this class by directly calling all instance methods



#endif //LFSR_LFSR_H
