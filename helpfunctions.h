//
// Created by George Sakellariou on 29/09/2017.
//

#ifndef AUTH_THESIS_FHE_SI_HELPFUNCTION_H
#define AUTH_THESIS_FHE_SI_HELPFUNCTION_H

#include <string>
#include "FHEContext.h"
#include "Matrix.h"
#include <string>
#include <iostream>
#include <fstream>
#include "FHE-SI.h"
void readdata(const std::string filename);
unsigned calculatelogQ(unsigned const &p,unsigned xi, unsigned &dim );
unsigned calculateXI(unsigned const &p,unsigned size, unsigned &dim);
bool LoadData(Matrix<ZZ> &rawData, vector<ZZ> &labels, unsigned &dim, const string &filename);
bool LoadDataPolyX(vector<ZZ_pX> &rawData, vector<ZZ_p> &labels, unsigned &dim, const string &filename, FHEcontext &context);
void savePublicKey(const FHESIPubKey &fhesiPubKey, const string &filename);
void saveSwitchedKey(const KeySwitchSI &keySwitchSI, const string &filename);
void saveSecretKey(const FHESISecKey &fhesiSecKey, const string &filename);
ZZ_p extraxtHM(const ZZ_pX &poly);


template <typename T>
void print(const T &message){
    std::cout<<message<<std::endl;
}


#endif //AUTH_THESIS_FHE_SI_HELPFUNCTION_H
