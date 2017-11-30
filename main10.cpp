//
// Created by george on 21/11/2017.
//
#include <bitset>
#include <iostream>
#include <string>
#include "helpfunctions.h"
#include <map>
using namespace std;
int main(){

    unsigned p = 23;
    unsigned g = 7;
    unsigned logQ = 117;
    FHEcontext context(p - 1, logQ, p, g, 3);
    activeContext = &context;
    context.SetUpSIContext();
    print("Cryptographic context:");
    print(context);
    vector<ZZ_pX> loadeddata;
    vector<ZZ_p> labels;
    unsigned dim;
    LoadDataPolyX(loadeddata,labels,dim,"../sample.dat",context);

    map<int,Ciphertext> cpoints;
    map<int,Plaintext> ppoints;
    map<int,Ciphertext>distance;
    map<int,Plaintext> dHM;

    FHESISecKey fhesiSecKey1(context);
    FHESIPubKey fhesiPubKey1(fhesiSecKey1);
    KeySwitchSI keySwitchSI1(fhesiSecKey1);

    for(int i=0;i<loadeddata.size();i++){
        Plaintext p_point(context,loadeddata[i]);
        ppoints[i]=p_point;
        print(p_point);
        Ciphertext c_point(fhesiPubKey1);
        fhesiPubKey1.Encrypt(c_point,p_point);
        cpoints[i]=c_point;

    }

    Ciphertext centroid = cpoints[0];
    for(int i=0;i<loadeddata.size();i++){
        Ciphertext dist;
        dist=centroid;
        dist*=-1;
        dist+=cpoints[i];
        //dist=FHE_Sub(cpoints[i],centroid,cnegcoef,keySwitchSI1);
        distance[i]=dist;
    }


    for(int i=0;i<loadeddata.size();i++){
        Plaintext dist;
        fhesiSecKey1.Decrypt(dist,distance[i]);
        print(dist);
        dHM[i]=dist;

    }


    print("------------Centroid---------------------");
    Ciphertext total=cpoints[0];
    for(int i=1;i<loadeddata.size();i++){
        total+=cpoints[i];
    }
    Plaintext ptotal;
    fhesiSecKey1.Decrypt(ptotal,total);
    print(ptotal);
    print(ptotal.message/2);

    ZZ_pX centroidx =ptotal.message;
    ZZ_pX new_centroid;
    ZZ_p coef;
    long mean=2;
    for(long i=0;i<centroidx.rep.length();i++){
        coef=coeff(centroidx,i);
        ZZ x= rep(coef);
        long t=to_long(x)/mean;
        SetCoeff(new_centroid,i,t);
    }

    print(new_centroid);

    ZZ_pX dp0 =dHM[0].message;
    ZZ_pX dp1 =dHM[1].message;

    ZZ_p dhm;
    ZZ phim=to_ZZ(p);
    ZZ hmd=to_ZZ(0);
    for(long i=0;i<dp1.rep.length();i++){
        dhm=coeff(dp1,i);
        print(dhm);
        ZZ x= rep(dhm);
        if(x>phim/2){
            print("---------");
            ZZ t= x-phim;
            print(t);
            t*=-1;
            print(t);
            hmd+=t;
            print("____________");
        }else{
            hmd+=x;
        }
        print(x);

    }
    print(hmd);
    print(to_long(hmd));
    print(context.ModulusP());


    size_t a=1014652156710223703;


    auto l= static_cast<uint32_t>(a);

    print(l);



    return 0;
};