#include "btree.h"

int main(){
    string index="index.txt";
    string data="data.txt";
    btree<int, 3> _tree(index,data);

    for (int i = 0; i < 39; i++){
        Registro reg{i+1, "Nombre", "CS", 3};
        _tree.insert(reg);
    }

    _tree.search(22);


   // Registro _reg3{20,"Gustavo","CS",3};



    // cout<<"IndexFile"<<endl;
    // cout<<"---------------------------"<<endl;
    // _tree.scanAll<Node<int, 3>>("index.txt");
    // cout<<"DataFile"<<endl;
    // cout<<"---------------------------"<<endl;
    // _tree.scanAll<Registro>("data.txt");

    // cout<<"\n\n\n\n";
    
    // Node<int,3 > tmp=_tree.readNode(0);

    // for(auto u: tmp.children){

    //     cout<<u<<endl;
    // }
    
    // tmp.showData();

   // _tree.insert(_reg3);
    return 0;
}