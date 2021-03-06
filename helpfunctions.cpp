//
// Created by George Sakellariou on 29/09/2017.
//

#include "helpfunctions.h"

using namespace std;

void readdata( const string filename){
    cout << "Read data from :" << filename <<endl;
    string line;
    ifstream infile;
    infile.open(filename);
    if (infile.is_open())
    {
        while (!infile.eof())
        {
            getline(infile,line);
            cout << line << '\n';
        }
        infile.close();
    }
    else cout << "Unable to open file";

}

FHEcontext setupFHEContext(unsigned p, unsigned q, unsigned logQ){

};

unsigned calculatelogQ(unsigned const &p,unsigned xi, unsigned &dim ){
    unsigned n = (p-1)/2-1;

    double lgQ = 4.5*log(n)+max(1,(int) dim-1)*(log(1280)+2*log(n)+log(xi));




}

unsigned calculatelogQ1(unsigned const &p, unsigned &d) {
    unsigned n=p-1;
    double lgQ=log(3*d*pow(n,2.5));
    auto logQ = (unsigned) ceil(lgQ / log(2) + 12);

    return logQ;

}

unsigned calculatelogQ2(unsigned const &p, unsigned &d) {
    unsigned n=p-1;
    double lgQ=log(3*d*pow(n,4.5));
    auto logQ = (unsigned) ceil(lgQ / log(2) + 22);

    return logQ;

}

unsigned calculateXI(unsigned const &p,unsigned size, unsigned &dim){
    unsigned blockSize = 1;
    unsigned val = (p-1)/2-1;

    while (val > 1) {
        blockSize <<= 1;
        val >>= 1;

    }

    auto nBlocks = static_cast<unsigned int>(size / blockSize);
    if (size % blockSize != 0) {
        nBlocks++;
    }

    unsigned xi = max(nBlocks, dim);
    return xi;
}

bool LoadData(Matrix<ZZ> &rawData, vector<ZZ> &labels, unsigned &dim, const string &filename) {
    ifstream fin;
    fin.open(filename);
    if (!fin) {
        cout << "Unable to read data file." << endl;
        return false;
    }

    rawData.Clear();
    labels.clear();

    int label, n;

    fin >> dim >> n;
    vector<ZZ> data(dim);
    for (int i = 0; i < n; i++) {
        for (unsigned j = 0; j < dim; j++) {
            fin >> data[j];
        }
        fin >> label;

        rawData.AddRow(data);
        labels.push_back(to_ZZ(label));
    }

    return true;
}

bool LoadDataPolyX(vector<ZZ_pX> &rawData, vector<ZZ_p> &labels, unsigned &dim, const string &filename, FHEcontext &context) {
    int label, n;
    long phim =context.zMstar.phiM();
    ifstream fin;
    fin.open(filename);
    if (!fin) {
        cout << "Unable to read data file." << endl;
        return false;
    }

    rawData.clear();
    labels.clear();


    ZZ p = context.ModulusP();
    fin >> dim >> n;


    ZZ_pX data;
    data.SetMaxLength(phim);
    long temp;
    for (int i = 0; i < n; i++) {

        for (unsigned j = 0; j < dim; j++) {
            fin>> temp;
            SetCoeff(data,j,temp);
        }
        fin >> label;
        rawData.push_back(data);
        labels.push_back(to_ZZ_p(label));

    }

    return true;

}
//binary enconding functions
ZZ_pX numbertoZZ_pX(long &number, FHEcontext &econtext) {
    ZZ_pX data;
    auto size = static_cast<unsigned int>(to_int(econtext.ModulusP()));
    for(unsigned int i=0; i<size;i++){
        SetCoeff(data,i,to_ZZ_p(bitset<16>(number)[i]));
    }

    return data;

}

long ptToNumber(Plaintext &ptxt) {
    long number=0;
    for(int i=0;i<ptxt.message.rep.length();i++ ){
        number+=IsOne(ptxt.message.rep[i])*pow(2,i);
    }
    return number;
}

unsigned long MSB(ZZ_pX &numberPoly, long vector_length) {
    unsigned long position=0;

    vec_ZZ_p coefvec = VectorCopy(numberPoly,vector_length);

    for(long i=0;i<coefvec.length();i++){
        if(coefvec[i]!=0){
            position=i;
        }

    }

    return position;
}

ZZ_pX MSB_poly(ZZ_pX &numberPoly) {
    ZZ_pX msb_poly;
    SetCoeff(msb_poly,MSB(numberPoly),to_ZZ_p(1));
    return msb_poly;
}


ZZ_pX complement2(ZZ_pX &numberPoly){
    vector<ZZ_p> coef;
    for(unsigned i=0;i<numberPoly.rep.length();i++){

    }



}
Plaintext binaryFHEAddTransformation(Plaintext &plaintext) {
    plaintext.message.rep.length();

}
//binary encoding functions
//base10 encoding functions
unsigned long MSB(long &number) {
    unsigned long msb=0;
    for(int i=0;i<bitset<16>(number).size();i++){
        if(bitset<16>(number)[i]!=0){
            msb=i;
        }
    }

    return msb;

}



Ciphertext FHE_Sub(Ciphertext &c1, Ciphertext &c2, Ciphertext &negCoef, KeySwitchSI &keySwitchSI) {
    Ciphertext ctotal, cnegtotal ;
    cnegtotal=c2;
    cnegtotal*=negCoef;
    cnegtotal.ScaleDown();
    keySwitchSI.ApplyKeySwitch(cnegtotal);
    ctotal=c1;
    ctotal+=cnegtotal;
    return ctotal;
}


Ciphertext FHE_MSB(Ciphertext &c1, Ciphertext &c2, Ciphertext &negCoef, Ciphertext &eCoef, KeySwitchSI &keySwitchSI) {
    Ciphertext msb=FHE_Sub(c1,c2,negCoef,keySwitchSI);
    msb+=eCoef;
    return msb;
}


Ciphertext FHE_MAX(Ciphertext &c1, Ciphertext &c2, Ciphertext &posCoef, Ciphertext &negCoef, Ciphertext &eCoef,
                   KeySwitchSI &keySwitchSI) {
    Ciphertext max,hmax1,hmax2,msb;
    msb=FHE_MSB(c1,c2,negCoef,eCoef,keySwitchSI);
    hmax2=FHE_Sub(posCoef,msb,negCoef,keySwitchSI);
    hmax2*=c2;
    hmax2.ScaleDown();
    hmax1=c1;
    hmax1*=msb;
    hmax1.ScaleDown();
    max=hmax1;
    max+=hmax2;
    keySwitchSI.ApplyKeySwitch(max);
    return max;
}

//base10 encoding functions

void savePublicKey(const FHESIPubKey &fhesiPubKey, const string &filename) {
    ofstream outputfile(filename);
    if(!outputfile){
        std::cerr<<"Can not open "<<filename<<"."<<std::endl;
        exit(1);
    }

    fhesiPubKey.Export(outputfile);

}

void saveSecretKey(const FHESISecKey &fhesiSecKey, const string &filename) {
    ofstream outputfile(filename);
    if(!outputfile){
        std::cerr<<"Can not open "<<filename<<"."<<std::endl;
        exit(1);
    }

    fhesiSecKey.Export(outputfile);

}

void saveSwitchedKey(const KeySwitchSI &keySwitchSI, const string &filename) {
    ofstream outputfile(filename);
    if(!outputfile){
        std::cerr<<"Can not open "<<filename<<"."<<std::endl;
        exit(1);
    }

    keySwitchSI.Export(outputfile);

}

ZZ_p extraxtHM(const ZZ_pX &poly) {
    ZZ_p dhm;
    for(long i=0;i<poly.rep.length();i++)
        dhm+=coeff(poly,i);

    return dhm;
}


bool LoadDataVecPolyX(vector<vector<ZZ_pX>> &rawData, vector<ZZ_p> &labels, unsigned &dim, const string &filename,
                      FHEcontext &context) {
    int label, n;
    long phim =context.zMstar.phiM();
    ifstream fin;
    fin.open(filename);
    if (!fin) {
        cout << "Unable to read data file." << endl;
        return false;
    }

    rawData.clear();
    labels.clear();


    ZZ p = context.ModulusP();
    fin >> dim >> n;


    ZZ_pX data;
    data.SetMaxLength(1);

    long temp;
    for (int i = 0; i < n; i++) {
        vector<ZZ_pX> point;
        for (unsigned j = 0; j < dim; j++) {
            fin>> temp;
            SetCoeff(data,0,temp);
            point.push_back(data);
        }
        fin >> label;
        rawData.push_back(point);
        labels.push_back(to_ZZ_p(label));

    }

    return true;

}


vector<Ciphertext> EncryptVector(const vector<ZZ_pX> &point, const FHEcontext &fhEcontext, const FHESIPubKey &fhesiPubKey) {
    unsigned long dimension = point.size();
    vector<Ciphertext> encrypted_vector;
    for(unsigned i=0; i<dimension;i++){
        Plaintext coefficient(fhEcontext,point[i]);
        Ciphertext encrypted_coefficient(fhesiPubKey);
        fhesiPubKey.Encrypt(encrypted_coefficient,coefficient);
        encrypted_vector.push_back(encrypted_coefficient);
    }
    return encrypted_vector;
}


vector<ZZ_pX> DecryptVector(const vector<Ciphertext> &cpoint, const FHESISecKey &fhesiSecKey) {
    unsigned long dimension=cpoint.size();
    vector<ZZ_pX> decrypted_vector;
    for (int i = 0; i < dimension ; ++i) {
        Plaintext coefficient;
        fhesiSecKey.Decrypt(coefficient,cpoint[i]);
        decrypted_vector.push_back(coefficient.message);
    }
    return decrypted_vector;
}


vector<ZZ_pX> DecryptVectorKS(const vector<Ciphertext> &cpoint, const FHESISecKey &fhesiSecKey, const KeySwitchSI &keySwitchSI) {
    unsigned long dimension=cpoint.size();
    vector<ZZ_pX> decrypted_vector;
    for (int i = 0; i < dimension ; ++i) {
        Plaintext coefficient;
        Ciphertext encrypted_coefficient= cpoint[i];
        keySwitchSI.ApplyKeySwitch(encrypted_coefficient);
        fhesiSecKey.Decrypt(coefficient,encrypted_coefficient);
        decrypted_vector.push_back(coefficient.message);
    }
    return decrypted_vector;
}

Ciphertext euclideanDistance(vector<Ciphertext> &cpoint1, vector<Ciphertext> &cpoint2, KeySwitchSI &keySwitchSI) {
    Ciphertext total;
    unsigned long dimension=cpoint1.size();
    total=euclideanDistanceP(cpoint1[0],cpoint2[0],keySwitchSI);
    for (int i = 1; i <dimension ; ++i) {
        Ciphertext coefficient_distance;
        coefficient_distance=euclideanDistanceP(cpoint1[i],cpoint2[i],keySwitchSI);
        total+=coefficient_distance;
    }
    return total;

}

Ciphertext euclideanDistanceP(Ciphertext &c1, Ciphertext &c2, KeySwitchSI &keySwitchSI) {
    Ciphertext total;
    Ciphertext semi_total;
    semi_total=c2;
    semi_total*=-1;
    semi_total+=c1;
    total=semi_total;
    total*=semi_total;
    total.ScaleDown();
    keySwitchSI.ApplyKeySwitch(total);
    return total;
}




void timeCalulator(const clock_t &c_start, const chrono::high_resolution_clock::time_point &t_start) {
    //clock_t c_start = clock();
    //auto t_start = chrono::high_resolution_clock::now();
    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << fixed << setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << chrono::duration<double, milli>(t_end-t_start).count()
              << " ms"<<endl;
}
