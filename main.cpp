#include "lib.h"
#include "tester/tester.h"

void ui(){
    cout << "Bienvenido a la BD:"<<endl;
    cout << "Escoja el tipo de archivo:" << endl;
    cout << "1. Sequential File" << endl << "2. B+ Tree" << endl;
    int fileType;
    cin >> fileType;

    if (fileType == 1){
        SequentialFile _file("data.txt", "data_aux.txt");
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
    else{
        btree<int, 3> _file("index5.txt", "data5.txt");
    
        int opcion = -1;
        while(opcion!=0) {
            cout << "Menu de Acciones: " << "\n";
            cout << "1. Insertar Registro" << "\n";
            cout << "2. Buscar Registro" << "\n";
            // cout << "4. Listar Todos"<<"\n";
            cout << "0. Salir" << "\n";
            cout<<"Seleccione opcion: "<<"\n";
            cin>>opcion;

            int key;
            bool _found=false;
            bool _delete=false;
            
            switch (opcion){
                case 1:
                    //Encriptar
                    Register reg;
                    reg.setData();
                    _file.insert(reg);
                    cout<<"Se inserto correctamente"<<endl;
                    break;
                case 2:
                    //Desencriptar
                    cout<<"Ingresa key a buscar: ";
                    cin>>key;
                    Register _reg;
                    _found= _file.search(key,_reg);
                    if(_found){
                        _reg.showData();
                    }
                    else{
                        cout<<"No se encontro el registro"<<endl;
                    }
                    break;
            }
        }
    }
}

int main() {
    
    int menu;
    cout << "1. Interface" << endl << "2. Testing" << endl;
    cin >> menu;
    
    if (menu == 1){
        ui();
        return 0;
    }
    else{
        Tester::execute(false);
        return EXIT_SUCCESS;
    }
    

}