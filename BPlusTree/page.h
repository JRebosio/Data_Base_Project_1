#include <iostream> 
#include <string>
#include <fstream>
#include "Register.h"
#include <vector>

#define MAX_RECORDS 10
#define ll long long
using namespace std; 

void swap(Register* a, Register* b)
{  
    Register t = *a;
    *a = *b;  
    *b = t;  
}  

int partition (vector<Register> &arr, int low, int high)
{  
    ll pivot =arr[high].code;   //char num[20]="12314      ";  ignora los vacios        
    int i = (low - 1); 
  
    for (int j = low; j <= high - 1; j++)  
    {  
        if (arr[j].code < pivot)
        {  
            i++; 
            swap(&arr[i], &arr[j]);  
        }  
    }  
    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);  
}  


void quickSort(vector <Register>  &arr, int low, int high)
{  
    if (low < high)  
    {  
        int pi = partition(arr, low, high);  
        quickSort(arr, low, pi - 1);  
        quickSort(arr, pi + 1, high);  
    }  
}  

istream & operator >> (istream & stream, Register & record)
{	   
    stream.read((char *) &record, sizeof(record));
    string bufer;
    getline(stream,bufer);
    return stream;  
}



struct Pagina{
    int size; 
    string name;  //to define 
    vector<Register> registers;
    string puntero_siguiente;

    void sort(){
        quickSort(registers, 0, registers.size()-1 );
    }
    void setName(){
        name = to_string(registers[0].code) + ".txt";
        
    }
    void write(){
        fstream file;
        file.open(name,ios::out| ios::binary |ios::trunc);
        for( int i = 0 ; i  < registers.size() ; i++){
            file.write((char*) &registers[i] , sizeof(registers[i]));
            file << endl;
        }
        file.close();
    };

    Pagina(string fileName): name{fileName},size(0) {
        loadPage(fileName);
    }; 
    Pagina(): name{""},size(0) {
    }; 
    void loadPage(string fileName){
        fstream file;
        file.open(fileName, ios::in | ios::binary);
        Register buffer;
        string bug;
        while(file >> buffer){
            registers.push_back(buffer);
        }
        size = registers.size();
        file.close();
    }

    void printPage(){
        for(int i = 0; i < registers.size() ; i ++){
            registers[i].print();
        }
    }
};