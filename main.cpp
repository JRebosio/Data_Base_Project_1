#include "lib.h"
// #include "seqfile/seqfile.h"
#include "tester/tester.h"

void ui(SequentialFile _file){
    cout<<"Bienvenido a la BD:"<<endl;
    int opcion = -1;
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
}

int main() {
    
    Tester::execute(false);
    

    // string data_file="data3.txt";
    // string aux_file="data_aux3.txt";
    // SequentialFile _file(data_file,aux_file);
    // ui(_file);

    // Registro _reg1{"103","Andrea","CS",4}; //1
    // _file.AddRecord(_reg1);
    
    // Registro _reg2{"104","Carlos","CS",1}; //2
    // _file.AddRecord(_reg2);

    //  Registro _reg3{"105","Samuel","CS",1}; //5
    // _file.AddRecord(_reg3);

    //  Registro _reg4{"105","Xad","CS",1}; //7
    // _file.AddRecord(_reg4);

    // Registro _reg5{"105","Winter","CS",1}; //6
    // _file.AddRecord(_reg5);

    // Registro reg6{"105","Llama","CS",1}; //3
    // _file.AddRecord(reg6);

    // Registro reg7{"105","Nicolas","CS",1}; //4
    // _file.AddRecord(reg7);


  

    // cout<<"DATA FILE"<< endl;
    // cout<<"----------------------"<<endl;

    // _file.scanAll(data_file);

    // cout<<"AUX FILE" << endl;
    // cout<<"----------------------"<<endl;
    // _file.scanAll(aux_file);
    
     return EXIT_SUCCESS;
}