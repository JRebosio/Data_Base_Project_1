#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include "seqfile\seqfile.h"

#define MAX_AUX 5

using namespace std;


int main() {
    string data_file="D:\\lab2data.txt";
    string aux_file="D:\\lab2aux.txt";
    string header_file="D:\\lab2header.txt";


    SequentialFile _file(data_file,aux_file,header_file);
    /*
    vector<Registro> _registros;
    _registros.push_back(Registro{"100","Jorge","CS",4});
    _registros.push_back(Registro{"101","Luis","CS",3});
    _registros.push_back(Registro{"102","Anthony","CS",3});
    _registros.push_back(Registro{"103","Zuis","CS",3});
    _registros.push_back(Registro{"104","Nando","CS",3});
    _registros.push_back(Registro{"105","Mariaella","CS",3});
    _registros.push_back(Registro{"106","Naair","CS",3});
    _file.InsertAll(_registros);
    */
     //Registro reg=_file.Search("Luis");
     //reg.showData();


    vector<Registro> _res=_file.search("Ignacio","Omar");
    for(auto it=_res.begin();it<_res.end();it++){
        Registro _data=*(it);
        _data.showData();
    }

    /*
    _file.scanAll<Registro>(data_file);
    _file.scanAll<Registro>(aux_file);
    _file.scanAll<Point>(header_file);
    */
}