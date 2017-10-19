//
// Created by george on 18/10/2017.
//
#include <string>
#include <bitset>
#include "helpfunctions.h"
#include "FHE-SI.h"
#include "Util.h"
#include "Ciphertext.h"
#include "Util.h"


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
    long coef3= 32768; // maximum binary length k=15
    long pos=19;
    long pos2=13;
    //int neg=-11;
    ZZ_pX polyNum1=numbertoZZ_pX(pos,context);
    ZZ_pX polyNum2=numbertoZZ_pX(pos2,context);
    ZZ_pX polycoef1= numbertoZZ_pX(coef1,context);
    ZZ_pX polycoef2 = numbertoZZ_pX(coef2,context);
    ZZ_pX polycoef3 = numbertoZZ_pX(coef3,context);

    print(polyNum1);
    print(polyNum2);
    print(polycoef1);
    print(polycoef2);
    print(polycoef3);
    print("-------------------------------------------------------");
    //print(LeadCoeff(polyNum1));

    //print(MSB_pos(polyNum1,16));
    //print(MSB_poly(polyNum1));
    FHESISecKey fhesiSecKey1(context);
    FHESIPubKey fhesiPubKey1(fhesiSecKey1);

    Plaintext ptx1(context,polyNum1), msb1(context,MSB_poly(polyNum1)),ptx2(context,polyNum2), msb2(context,MSB_poly(polyNum2));
    Plaintext ptcoef1(context,polycoef1),ptcoef2(context,polycoef2),ptcoef3(context,polycoef3);
    print(ptx1);
    print(msb1);
    print(ptx1);
    print(msb2);
    print(ptcoef1);
    print(ptcoef2);
    print(ptcoef3);

    print("-------------------------------------------------------");

    Ciphertext cipht1(fhesiPubKey1),cmsb1(fhesiPubKey1),cipht2(fhesiPubKey1),cmsb2(fhesiPubKey1);
    Ciphertext ccoef1(fhesiPubKey1),ccoef2(fhesiPubKey1),ccoef3(fhesiPubKey1), ctotal(fhesiPubKey1);

    fhesiPubKey1.Encrypt(cipht1,ptx1);
    fhesiPubKey1.Encrypt(cipht2,ptx2);
    fhesiPubKey1.Encrypt(cmsb1,msb1);
    fhesiPubKey1.Encrypt(cmsb2,msb2);
    fhesiPubKey1.Encrypt(ccoef1,ptcoef1);
    fhesiPubKey1.Encrypt(ccoef2,ptcoef2);
    fhesiPubKey1.Encrypt(ccoef3,ptcoef3);

    ctotal=cipht1;




}
