//
// Created by george on 30/11/2017.
//

#include <bitset>
#include <iostream>
#include <string>
#include "helpfunctions.h"
#include <map>
using namespace std;
int main() {
    clock_t c_start = clock();
    auto t_start = chrono::high_resolution_clock::now();

    unsigned p = 23;
    unsigned g = 7;
    unsigned logQ = 117;
    FHEcontext context(p - 1, logQ, p, g, 3);
    activeContext = &context;
    context.SetUpSIContext();
    FHESISecKey fhesiSecKey1(context);
    FHESIPubKey fhesiPubKey1(fhesiSecKey1);
    KeySwitchSI keySwitchSI1(fhesiSecKey1);
    vector<vector<ZZ_pX>> loadeddata;
    vector<ZZ_p> labels;
    unsigned dim;
    LoadDataVecPolyX(loadeddata,labels,dim,"../sample.dat",context);
    /*
    for(unsigned i=0; i<loadeddata.size();i++){
        print("Point :"+to_string(i));
        for(unsigned int j=0;j<dim;j++){
            print(loadeddata[i][j]);
        }


    }*/
    vector<Ciphertext> cpoint1;
    vector<Ciphertext> cpoint2;
    cpoint1= EncryptVector(loadeddata[0],context,fhesiPubKey1);

    cpoint2= EncryptVector(loadeddata[1],context,fhesiPubKey1);

    /*
    vector<ZZ_pX> point;
    point = DecryptVector(cpoint,fhesiSecKey1);
    for (const auto &j : point) {
        print(j);
    }*/
    Ciphertext d;
    d=euclideanDistance(cpoint1,cpoint2,keySwitchSI1);
    print(d);
    Plaintext dp;
    fhesiSecKey1.Decrypt(dp,d);
    print(dp);
    timeCalulator(c_start,t_start);
    return 0;
}