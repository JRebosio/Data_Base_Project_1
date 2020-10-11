#include "lib.h"
#include "seqfile/seqfile.h"


int main (){

    string data_file="data.txt";
    string aux_file="data_aux.txt";


    SequentialFile _file(data_file,aux_file);

    Registro _reg1{"100","Jorge","CS",4};
    _file.AddRecord(_reg1);
    
    Registro _reg2{"101","Luis","CS",1};
    _file.AddRecord(_reg2);

     Registro _reg3{"102","Carlos","CS",1};
    _file.AddRecord(_reg3);


    // Registro regget2 = _file.Search("Jorge");
    
    // regget2.showData();


    return 0;
}