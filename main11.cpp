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
    /*
1901
1907
1913
1931
1933
1949
1951
1973
1979
1987
1993
1997
1999
2003
2011
2017
2027
2029
2039
2053
2063
2069
2081
2083
2087
2089
2099

     */
    clock_t c_start = clock();
    auto t_start = chrono::high_resolution_clock::now();

    unsigned p = 2027;
    unsigned g = 7;
    //unsigned logQ = 80;
    unsigned dim =10;
    unsigned size=100;
    //unsigned xi = calculateXI(p,4096,dim);
    //print(xi);
    unsigned logQ = calculatelogQ1(p,size);
    print(logQ);

    FHEcontext context(p - 1, logQ, p, g, 3);

    activeContext = &context;
    context.SetUpSIContext();
    print(context);
    FHESISecKey fhesiSecKey1(context);
    print(fhesiSecKey1);
    FHESIPubKey fhesiPubKey1(fhesiSecKey1);
    print(fhesiPubKey1);
    KeySwitchSI keySwitchSI1(fhesiSecKey1);

/*

    unsigned dim=10;
    unsigned p = 9743;
    unsigned g = 7;
    unsigned logQ = 117;

    unsigned xi=calculateXI(p,20,dim);
    print(xi);
    unsigned logw=calculatelogQ(p,xi,dim);
    print(logw);

    FHEcontext context(p - 1, logQ, p, g, 3);
    print(context);
    activeContext = &context;
    context.SetUpSIContext();
    print(context);


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

    for(unsigned i=0; i<loadeddata.size();i++){
        print("Point :"+to_string(i));
        for(unsigned int j=0;j<dim;j++){
            print(loadeddata[i][j]);
        }


    }
    vector<Ciphertext> cpoint1;
    vector<Ciphertext> cpoint2;
    cpoint1= EncryptVector(loadeddata[0],context,fhesiPubKey1);

    cpoint2= EncryptVector(loadeddata[1],context,fhesiPubKey1);


    vector<ZZ_pX> point;
    point = DecryptVector(cpoint,fhesiSecKey1);
    for (const auto &j : point) {
        print(j);
    }
    Ciphertext d;
    d=euclideanDistance(cpoint1,cpoint2,keySwitchSI1);
    print(d);
    Plaintext dp;
    fhesiSecKey1.Decrypt(dp,d);
    print(dp);
    timeCalulator(c_start,t_start);
    */
    return 0;
}