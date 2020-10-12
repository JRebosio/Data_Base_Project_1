#include "tester.h"
#include <iostream>
#include <string>


void Tester::execute(){

    for(int i=0;i<NUMBER_OF_TESTS;i++){
        string file=std::to_string(i+1)+".txt";
        std::cout<<file<<endl;
        string data="data"+file;
        string dataux="data_aux"+file;
        Mocker mock(file);
        TestSeqFile<Registro>(mock.GetData(),data,dataux);
        cout << "Test[" << i + 1 << "] sucessful" << endl;

    }
}


template <typename T>
void Tester::TestSeqFile(vector<string> elements,string data, string dataux){
 
    //tomar tiempos.
    SequentialFile _file(data,dataux);
    for(int j=0;j<elements.size(); ++j) {
        T _tmp;
        strcpy(_tmp.codigo, std::to_string(j+1).c_str());
        strcpy(_tmp.nombre, elements[j].c_str());
        strcpy(_tmp.carrera, "CS");
        strcpy(_tmp.codigo, "4");
        _file.AddRecord(_tmp);
    }

}