
#include "Files.h"

using namespace std;

int main(){


    Register Alumno(201820025,"Heider","Base Dedatos", "Ciencias de la Computacion");


    Alumno.print();

    BPlusTree tree("nodos.txt");
    tree.insert(Alumno);



    // Node nodo;
    // fstream ss;
   
    // ss.open("nodos.txt", ios::binary|ios::in );
    //  ss.seekg(0, ss.beg);
    // ss.read((char *)&nodo,sizeof(Node));
    // ss.close();

    // for(int i=0; i<3; i++){
    //     cout<<nodo.keys[i]<<endl;
    // }
    // cout<<"F"<<endl;
    // for(int i=0; i<3; i++){
    //     cout<<nodo.children[i]<<endl;
    // }

    // return 0;

}