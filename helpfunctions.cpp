//
// Created by George Sakellariou on 29/09/2017.
//
#include <string>
#include <iostream>
#include <fstream>
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