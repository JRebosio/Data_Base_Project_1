#ifndef MANAGER_H
#define MANAGER_H


#include "..\lib.h"


template<typename T>
class Manager{

private:
    string file;
public:

    Manager(string _file){
        this->file=_file;
    }

     int get_size(){
        int numRecords = 0;
        fstream inFile;
        inFile.open(file, ios::in | ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, ios::end);
            long bytes = inFile.tellg();
            numRecords = bytes / sizeof(T);
            inFile.close();
        } else cout << "Could not open the file.\n";
        return numRecords;
    }

     void heapAdd(T _reg){
        fstream outFile;
        outFile.open(_reg._id.file,ios::in| ios::out| ios::binary);
        if(outFile.is_open()){
            outFile.seekg(0,ios::end);
            outFile.write((char* )&_reg, sizeof(T));
            outFile.close();
        }
    }

    void heapAddPos(T _reg,int pos){
        fstream outFile;
        outFile.open(file,ios::in| ios::out| ios::binary);
        if(outFile.is_open()){
            outFile.seekg((pos) * sizeof(T));
            outFile.write((char* )&_reg, sizeof(T));
            outFile.close();
        }
    }

    T getRegByPos(int pos){
        ifstream outFile;
        T obj;
        outFile.open(file,ios::in| ios::out| ios::binary);
        if (outFile.is_open()) {
            outFile.seekg(pos * sizeof(T));
            outFile.read((char *) &obj, sizeof(T));
            outFile.close();
        }
        return obj;
    }

};
















#endif