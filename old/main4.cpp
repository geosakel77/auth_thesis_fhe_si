//
// Created by george on 20/10/2017.
//

#include "../helpfunctions.h"
#include "FHE-SI.h"
#include "Util.h"
#include "Ciphertext.h"
#include "Util.h"
#include "ZZ_p.h"

int main() {

    //Setup FHE Context
    unsigned p = 23;
    unsigned g = 7;
    unsigned logQ = 117;
    FHEcontext context(p - 1, logQ, p, g, 3);
    activeContext = &context;
    context.SetUpSIContext();
    print("Cryptographic context:");
    print(context);
    long coef1=1;
    long coef2=-1;
    long coef3= 16; // maximum binary length k=4,2^4
    long pos=10;
    long pos2=7;
    //int neg=-11;
    ZZ_pX polyNum1(pos);
    ZZ_pX polyNum2(pos2);
    ZZ_pX polycoef1(coef1);
    ZZ_pX polycoef2(coef2);
    ZZ_pX polycoef3(coef3);
    ZZ_pX msb1(MSB(pos));
    ZZ_pX msb2(MSB(pos2));
    print(polyNum1);
    print(polyNum2);
    print(polycoef1);
    print(polycoef2);
    print(polycoef3);
    print("-------------------------------------------------------");
    print(context.zMstar.phiM());
    FHESISecKey fhesiSecKey1(context);
    FHESIPubKey fhesiPubKey1(fhesiSecKey1);
    KeySwitchSI keySwitchSI1(fhesiSecKey1);

    Plaintext ptx1(context,polyNum1), pmsb1(context,msb1),ptx2(context,polyNum2), pmsb2(context,msb2);
    Plaintext ptcoef1(context,polycoef1),ptcoef2(context,polycoef2),ptcoef3(context,polycoef3);
    print(ptx1);
    print(pmsb1);
    print(ptx2);
    print(pmsb2);
    print(ptcoef1);
    print(ptcoef2);
    print(ptcoef3);

    print("-------------------------------------------------------");


    Ciphertext cipht1(fhesiPubKey1),cmsb1(fhesiPubKey1),cipht2(fhesiPubKey1),cmsb2(fhesiPubKey1);
    Ciphertext ccoef1(fhesiPubKey1),ccoef2(fhesiPubKey1),ccoef3(fhesiPubKey1), ctotal(fhesiPubKey1),ctotal1(fhesiPubKey1);

    fhesiPubKey1.Encrypt(cipht1,ptx1);
    fhesiPubKey1.Encrypt(cipht2,ptx2);
    fhesiPubKey1.Encrypt(cmsb1,msb1);
    fhesiPubKey1.Encrypt(cmsb2,msb2);
    fhesiPubKey1.Encrypt(ccoef1,ptcoef1);
    fhesiPubKey1.Encrypt(ccoef2,ptcoef2);
    fhesiPubKey1.Encrypt(ccoef3,ptcoef3);

    ctotal=cipht1;
    ctotal+=cipht2;
    Plaintext ptotal,ptotal1;
    //ctotal=FHE_MAX(cipht1,cipht2,ccoef1,ccoef2,ccoef3,keySwitchSI1);
    print(ctotal);
    fhesiSecKey1.Decrypt(ptotal,ctotal);
    print(ptotal);

}