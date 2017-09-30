#include <iostream>
#include "helpfunctions.h"
#include "Regression.h"
int main() {

    Matrix<ZZ> rawData;
    vector<ZZ> labels;
    unsigned dim;

    if (!LoadData(rawData, labels, dim, "../sample.dat")) {
        return 1;
    }

    return 0;
}