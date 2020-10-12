#ifndef MOCKER_H
#define MOCKER_H

#include "../lib.h"
#include <iostream> 
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class Mocker{

string file;
public:
    Mocker(string _file){
        this->file= "mocker/" + _file;
    }
    std::vector<string> GetData(){
        std::vector<string> _alldata;
        ifstream myFile;
        myFile.open(file);
        string name;
        while (myFile.good())
        {
            getline(myFile, name);
            _alldata.push_back(name);
        }
        myFile.close();
      return _alldata;
    }


};


#endif