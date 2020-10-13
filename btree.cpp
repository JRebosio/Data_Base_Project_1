#include "btree.h"

int main(){
    string index="index.txt";
    string data="data.txt";
    Registro _reg{1,"Jorge","CS",3};
    btree<int, 3> _tree(index,data);
    _tree.insert(_reg);
    return 0;
}