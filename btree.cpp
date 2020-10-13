#include "btree.h"

int main(){
    string index="index.txt";
    string data="data.txt";
    Registro _reg{1,"Jorge","CS",3};
    Registro _reg2{2,"Lorenzo","CS",3};
    Registro _reg3{20,"Gustavo","CS",7};
    Registro _reg4{40,"Luis","CS",6};
    Registro _reg5{55,"Carlos","CS",5};
    Registro _reg6{60,"Christian","CS",10};
   // Registro _reg3{20,"Gustavo","CS",3};

    btree<int, 3> _tree(index,data);
    
    _tree.insert(_reg);
    _tree.insert(_reg2);
    _tree.insert(_reg3);
    _tree.insert(_reg4);

    cout<<"IndexFile"<<endl;
    cout<<"---------------------------"<<endl;
    _tree.scanAll<Node<int, 3>>("index.txt");
    cout<<"IndexFile"<<endl;
    cout<<"---------------------------"<<endl;
    _tree.scanAll<Registro>("data.txt");

    // cout<<"\n\n\n\n";
    
    // Node<int,3 > tmp=_tree.readNode(0);

    // for(auto u: tmp.children){

    //     cout<<u<<endl;
    // }
    
    // tmp.showData();

   // _tree.insert(_reg3);
    return 0;
}