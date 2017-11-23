#include "../helpfunctions.h"
#include "Plaintext.h"
#include "DoubleCRT.h"
#include "Ciphertext.h"
#include "NTL/ZZ_pX.h"

#include <time.h>
#include "FHE-SI.h"
#include "Matrix.h"


bool runTest(bool disp, long long seed, unsigned p, FHEcontext &context) {
    std::cout<<"Starting"<<std::endl;
    ZZ seedZZ;
    seedZZ = seed;

    srand48(seed);
    SetSeed(seedZZ);

    FHESISecKey secretKey(context);
    const FHESIPubKey &publicKey(secretKey);

    long phim = context.zMstar.phiM();

    ZZ_pX ptxt1Poly, ptxt2Poly, sum, sumMult, prod, prod2, sumQuad;
    Plaintext resSum, resSumMult, resProd, resProdSwitch, resProd2, resSumQuad;

    ptxt1Poly.rep.SetLength(phim);
    ptxt2Poly.rep.SetLength(phim);
    std::cout<<"Plaintext without values:"<<ptxt1Poly<<","<< ptxt2Poly<<std::endl;

    for (long i=0; i < phim; i++) {
        ptxt1Poly.rep[i] = RandomBnd(p);
        ptxt2Poly.rep[i] = RandomBnd(p);
    }

    std::cout<<"Plaintext before normalization:"<<ptxt1Poly<<","<<ptxt2Poly<<std::endl;
    ptxt1Poly.normalize();
    ptxt2Poly.normalize();

    std::cout<<"Plaintext:"<<ptxt1Poly<<","<<ptxt2Poly<<std::endl;

    sum = ptxt1Poly + ptxt2Poly;
    std::cout<<"Plaintext sum:"<<sum<<std::endl;

    sumMult = ptxt2Poly * 7;
    std::cout<<"Plaintext sumMult:"<<sumMult<<std::endl;
    prod = ptxt1Poly * ptxt2Poly;
    std::cout<<"Plaintext prod:"<<prod<<std::endl;
    prod2 = prod * prod;
    std::cout<<"Plaintext square:"<<prod2<<std::endl;
    sumQuad = prod2 * prod2 * 9;
    std::cout<<"Plaintext sum:"<<sumQuad<<std::endl;


    rem(prod, prod, to_ZZ_pX(context.zMstar.PhimX()));
    rem(prod2, prod2, to_ZZ_pX(context.zMstar.PhimX()));
    rem(sumQuad, sumQuad, to_ZZ_pX(context.zMstar.PhimX()));
    std::cout<<prod<<", "<<prod2<<", "<<sumQuad<<endl;
    Plaintext ptxt1(context, ptxt1Poly), ptxt2(context, ptxt2Poly);
    std::cout<<"Plaintext 1:"<<ptxt1<<", "<<ptxt2<<std::endl;

    Ciphertext ctxt1(publicKey), ctxt2(publicKey);

    publicKey.Encrypt(ctxt1, ptxt1);
    publicKey.Encrypt(ctxt2, ptxt2);
    std::cout<<"Ciphertext 1:"<<ctxt1<<", "<<ctxt2<<std::endl;
    Ciphertext cSum = ctxt1;
    cSum += ctxt2;
    std::cout<<"Ciphertext sum"<<cSum<<std::endl;
    Ciphertext cSumMult = ctxt2;
    for (int i = 1; i < 7; i++) {
        cSumMult += ctxt2;
    }
    std::cout<<"Ciphertext sumMult"<<cSumMult<<std::endl;
    Ciphertext cProd = ctxt1;
    cProd *= ctxt2;
    std::cout<<"Ciphertext cProd"<<cProd<<std::endl;
    secretKey.Decrypt(resSum, cSum);
    secretKey.Decrypt(resSumMult, cSumMult);
    std::cout<<"DEcrypt cSum"<<resSum<<std::endl;
    std::cout<<"Decrypt cSumMult"<<resSumMult<<std::endl;


    KeySwitchSI keySwitch(secretKey);
    keySwitch.ApplyKeySwitch(cProd);
    std::cout<<"Ciphertext Switched cProd"<<cProd<<std::endl;

    secretKey.Decrypt(resProd, cProd);
    std::cout<<"Decrypt cProd: "<<resProd<<std::endl;
    cProd *= cProd;
    Ciphertext tmp = cProd;
    Ciphertext cSumQuad = cProd;

    keySwitch.ApplyKeySwitch(cProd);
    secretKey.Decrypt(resProd2, cProd);

    for (int i = 0; i < 8; i++) {
        cSumQuad += tmp;
    }
    keySwitch.ApplyKeySwitch(cSumQuad);
    cSumQuad *= cProd;
    keySwitch.ApplyKeySwitch(cSumQuad);
    secretKey.Decrypt(resSumQuad, cSumQuad);

    bool success = ((resSum.message == sum) && (resSumMult.message == sumMult) &&
                    (resProd.message == prod) && (resProd2.message == prod2) &&
                    (resSumQuad == sumQuad));

    if (disp || !success) {
        cout << "Seed: " << seed << endl << endl;

        if (resSum.message != sum) {
            cout << "Add failed." << endl;
        }
        if (resSumMult.message != sumMult) {
            cout << "Adding multiple times failed." << endl;
        }
        if (resProd.message != prod) {
            cout << "Multiply failed." << endl;
        }
        if (resProd2.message != prod2) {
            cout << "Squaring failed." << endl;
        }
        if (resSumQuad.message != sumQuad) {
            cout << "Sum and quad failed." << endl;
        }
    }

    if (disp || !success) {
        cout << "Test " << (success ? "SUCCEEDED" : "FAILED") << endl;
    }

    return success;
}






int main() {

    unsigned p=23;
    unsigned g=7;
    unsigned logQ=117;

    Matrix<ZZ> rawData;
    vector<ZZ> labels;
    unsigned dim;

    if (!LoadData(rawData, labels, dim, "../sample1.dat")) {
        return 1;
    }
    std::cout<<"The dimension of data points is:"<<std::endl;
    std::cout<<dim<<std::endl;
    std::cout<<"The labels of data points are:"<<std::endl;
    for(auto item = labels.begin();item !=labels.end();item++){
        std::cout<<*item<<' ';
    }
    std::cout<<'\n'<<std::endl;

    std::cout<<"The points are:"<<std::endl;

    std::cout<<rawData<<std::endl;

    std::cout<<"------------------------------"<<std::endl;
    std::cout<<"Keys Generation"<<std::endl;
    unsigned xi= calculateXI(p, static_cast<unsigned int>(labels.size()), dim);

    logQ = calculatelogQ(p,xi,dim);

    std::cout<<"The value of logQ is :"<<logQ<<std::endl;

    FHEcontext context(p-1,logQ,p,g,3);
    activeContext =&context;
    context.SetUpSIContext();

    std::cout<<context<<std::endl;
    runTest(true,18000,p,context);
    /*

    FHESISecKey fhesiSecKey(context);
    const FHESIPubKey &fhesiPubKey(fhesiSecKey);

    KeySwitchSI keySwitchSI(fhesiSecKey);
    std::cout<<"------------------------------"<<std::endl;
    std::cout<<"Paintext Format"<<std::endl;
    ZZ p1 = to_ZZ(context.ModulusP());


    Matrix<Plaintext> ptData;
    std::cout<<rawData.NumCols()<<","<<rawData.NumRows()<<std::endl;
    for (unsigned i=0; i<rawData.NumRows();i++){
        vector<Plaintext> ptrow;
        vector<ZZ> row;
        for (unsigned j=0; j<rawData.NumCols();j++){

            //std::cout<<rawData[i][j]<<", "<<to_ZZ(rawData[i][j])<<", "<< to_ZZ_pX(rawData[i][j])<<", "
            //         <<to_ZZ(rawData[i][j])%p1<<", "<<to_ZZ_pX(p1)<<std::endl;
            std::cout<<rawData[i][j]<<", "<<to_ZZ(rawData[i][j])%p1<<std::endl;
            row.push_back(to_ZZ(rawData[i][j])%p1);
        }
        for(unsigned k=0 ;k<row.size();k++){
            std::cout<<row[k]<<",";
        }
        std::cout<<endl;

        ptrow.push_back(Plaintext(context,row));
        ptData.AddRow(ptrow);
        std::cout<<"------------------"<<endl;
        row.clear();
    }

    std::cout<<ptData<<std::endl;


    Plaintext ptx1(context,to_ZZ(rawData[1][2])%p1);
    Plaintext ptx2(context,to_ZZ(rawData[1][3])%p1);

    Plaintext sumptx, ptx11;
    std::cout<<"Plaintext data:"<<rawData[1][2]<<", "<<rawData[1][3]<<", "<<std::endl;
    std::cout<<"Plaintext addition:"<<rawData[1][2]+rawData[1][3]<<std::endl;
    Ciphertext cipht1(fhesiPubKey);
    Ciphertext cipht2(fhesiPubKey);
    Ciphertext sumcipht;
    fhesiPubKey.Encrypt(cipht1,ptx1);
    fhesiPubKey.Encrypt(cipht2,ptx2);


    std::cout<<"Ciphertext data:"<<cipht1<<", "<< cipht2<<std::endl;
    sumcipht = cipht1;
    sumcipht+=cipht2;

    std::cout<<"Ciphertext sum:"<<sumcipht<<std::endl;
    fhesiSecKey.Decrypt(sumptx,sumcipht);
    std::cout<<"Decrypted Sum:"<<sumptx<<std::endl;
    keySwitchSI.ApplyKeySwitch(sumcipht);

    std::cout<<"------------------"<<std::endl;

    Plaintext ksptx;
    fhesiSecKey.Decrypt(ksptx,sumcipht);
    std::cout<<"Key switched encryption:"<<sumcipht<<std::endl;
    std::cout<<"Key switched decryption:"<<ksptx<<std::endl;
    */


    return 0;
}