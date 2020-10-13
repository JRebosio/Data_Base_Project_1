#include "btree.h"

int main(){
    string index="index.txt";
    string data="data.txt";
    Registro _reg{1,"Jorge","CS",3};
    Registro _reg2{2,"Lorenzo","CS",3};
    Registro _reg3{20,"Gustavo","CS",3};
   // Registro _reg3{20,"Gustavo","CS",3};

    btree<int, 3> _tree(index,data);
    
    _tree.insert(_reg);
    _tree.insert(_reg2);
    _tree.insert(_reg3);
    cout<<"\n\n\n\n";
    
    Node<int,3 > tmp=_tree.readNode(0);

    for(auto u: tmp.children)
    
    tmp.showData();

   // _tree.insert(_reg3);
    return 0;
}