#include "tester.h"
#include <iostream>
#include <string>
#include <chrono> 


void Tester::execute(bool vervose){

    int fileType;
        cout << "1. Sequential File" << endl << "2. B+ Tree" << endl;
        string ffile="tiempos.txt";
        if (fileType == 1){
            char _file[11];
            strcpy(_file, ffile.c_str());

            ofstream _cout;
            _cout.open (_file, std::ofstream::out | std::ofstream::app);

            for(int i=0;i<NUMBER_OF_TESTS;i++){
                string file=std::to_string(i+1)+".txt";
                // std::cout<<file<<endl;
                string data="data"+file;
                string dataux="data_aux"+file;
                Mocker mock(file);
                auto data_ = mock.GetData();
                auto start = std::chrono::high_resolution_clock::now();
                TestSeqFile<Registro>(data_,data,dataux, vervose);
                auto stop = std::chrono::high_resolution_clock::now(); 
                cout << "Test[" << i + 1 << "] sucessful" << endl;
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); 
                _cout<<"Test[" << i + 1 << "]"<<"---"<<"Size: "<< data_.size()<<"---"<<"Duracion: "<<duration.count();
        }
        _cout<<endl;
        _cout.close();
    }
    
    else{
        string file="tiemposBtree.txt";
        ofstream _cout;
        _cout.open(file, std::ofstream::out | std::ofstream::app);
        for(int i=1; i<=NUMBER_OF_TESTS; i++){
            string index="index"+to_string(i)+".txt";
            string data="data"+to_string(i)+".txt";
            btree<int, 3> _tree(index,data);
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 1; j <= 1000*i*i*i; j++){
                Register reg{j, "Nombre", "CS", i%10};
                _tree.insert(reg);
            }
            auto stop = std::chrono::high_resolution_clock::now(); 
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); 
            _cout<<"Test[" << i + 1 << "]"<<"---"<<"Size: "<< 1000*i*i*i <<"---"<<"Duracion: "<<duration.count()<<endl;
            _cout<<endl;
            
        }
        _cout.close();
    }

}


template <typename T>
void Tester::TestSeqFile(vector<string> elements,string data, string dataux,bool vervose){
    //tomar tiempos.
    SequentialFile _file(data,dataux);
    for(int j=0;j<elements.size(); ++j) {
        T _tmp;
        strcpy(_tmp.codigo, std::to_string(j+1).c_str());
        strcpy(_tmp.nombre, elements[j].c_str());
        strcpy(_tmp.carrera, "CS");
        strcpy(_tmp.codigo, "4");
        _file.AddRecord(_tmp);

        if(vervose){
            char file[11];
            string ffile="output.txt";
            strcpy(file, ffile.c_str());

            ofstream cout;
            cout.open (file, std::ofstream::out | std::ofstream::app);
            cout<<"iteracion "<< j<<endl;
            cout<<"DATA FILE"<< endl;
            cout<<"----------------------"<<endl;
            cout<<"Header"<<endl;
            Point head=_file.GetHeader(data);
            head.showData(cout);
            cout<<"----------------------"<<endl;
            cout<<"DATA"<<endl;
            _file.scanAll(data,cout);
            cout<<endl;
            cout<<"AUX FILE" << endl;
            cout<<"----------------------"<<endl;
            cout<<"Header"<<endl;
            Point _head=_file.GetHeader(dataux);
            _head.showData(cout);
            cout<<"----------------------"<<endl;
            cout<<"DATA"<<endl;
            _file.scanAll(dataux,cout);   
            cout<<endl;
            cout.close();
        }
    }

}