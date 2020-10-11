#include "lib.h"

#include "seqfile/seqfile.h"


// int main (){

//     string data_file="data.txt";
//     string aux_file="data_aux.txt";

   




//     SequentialFile _file(data_file,aux_file);

//     // Registro _reg1{"100","Jorge","CS",4};
//     // _file.AddRecord(_reg1);
    
//     // Registro _reg2{"101","Luis","CS",1};
//     // _file.AddRecord(_reg2);

//     //  Registro _reg3{"102","Carlos","CS",1};
//     // _file.AddRecord(_reg3);

//     Registro reg;
//     bool _found=false;
//     _found = _file.Search("Jorge",reg);
    
//     // regget2.showData();


//     return 0;
// }



int main() {
    int opcion = -1;
    string data_file="data.txt";
    string aux_file="data_aux.txt";
    SequentialFile _file(data_file,aux_file);

    cout<<"Bienvenido a la BD:"<<endl;
    while(opcion!=0) {
        cout << "Menu de Acciones: " << "\n";
        cout << "1. Insertar Registro" << "\n";
        cout << "2. Buscar Registro" << "\n";
        cout << "3. Eliminar Registro" << "\n";
        // cout << "4. Listar Todos"<<"\n";
        cout << "0. Salir" << "\n";
        cout<<"Seleccione opcion: "<<"\n";
        cin>>opcion;

        string key="";
        bool _found=false;
        bool _delete=false;
        
        switch (opcion){
            case 1:
                //Encriptar
                Registro reg;
                reg.setData();
                _file.AddRecord(reg);
                cout<<"Se inserto correctamente"<<endl;
                break;
            case 2:
                //Desencriptar
                cout<<"Ingresa key a buscar: ";
                cin>>key;
                Registro _reg;
                _found=_file.Search(key,_reg);
                if(_found){
                    _reg.showData();
                }
                else{
                    cout<<"No se encontro el registro"<<endl;
                }
                break;
            case 3:
                cout<<"Ingresa key a eliminar: ";
                cin>>key;
                _delete=_file.DeleteRecord(key);
                if(_delete)
                {
                    cout<<"Se elimino correctamente"<<endl;
                }
                else{
                    cout<<"No se encontro el registro"<<endl;
                }
                break;
        }
    }

    return 0;
}