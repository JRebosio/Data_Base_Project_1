#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>


#define MAX_AUX 5

using namespace std;


struct Point{
public:
    char file[20];
    int  pos;
    int size;
    //-1 eliminado y -2 final
    void setFile(string _file){
        strcpy(this->file, _file.c_str());
    }
    void setPost(int pos){
        this->pos=pos;
    }
    void showData(){
        cout<<"\nFile: "<<file;
        cout<<"\nPos: "<<pos;
        cout<<endl;
    }

};


struct Registro
{
public:
    //int pos;
    char codigo[5];
    char nombre[20];
    char carrera[15];
    int ciclo;
    Point _id;
    Point _next;

    bool operator>(const Registro& d ) const{
        return string{nombre}>string{d.nombre};

    }
    bool operator<(const Registro& d) const {
        return string{nombre} < string{d.nombre};
    }
    void showData(){
        cout<<"\nId:";
        _id.showData();
        cout<<"\nCodigo: "<<codigo;
        cout<<"\nNombre: "<<nombre;
        cout<<"\nCarrera: "<<carrera;
        cout<<"\nCiclo:"<<ciclo;
        cout<<"\nNext:";
        _next.showData();
        cout<<endl;
    }

};



class SequentialFile{
private:
    string data_file;
    string aux_file;
    int extra_size=sizeof(Point);

    Registro BinarySearchNear(string key){
        int data_file_size=get_size(data_file);
        int l, u, m;
        l = 0;
        u = data_file_size- 1;
        Registro obj;
        while (l <= u) {
            m = (l + u) / 2;
            obj=getRegbyPost<Registro>(m,data_file);
            if (obj.nombre > key) {
                u = m - 1;
            }
            else if (obj.nombre <key) {
                l = m + 1;
            } else {
                return obj;
            }
        }
    }

     void heapAdd(Registro _reg){
        fstream outFile;
        outFile.open(_reg._id.file,ios::in| ios::out| ios::binary);
        if(outFile.is_open()){
            outFile.seekg(0,ios::end);
            outFile.write((char* )&_reg, sizeof(Registro));
            outFile.close();
        }
    }

    template <typename  T>
    void WriteInPos(string file,T _reg,int pos){
        fstream outFile;
        outFile.open(file,ios::in| ios::out| ios::binary);
        if(outFile.is_open()){
            outFile.seekg(((pos) * sizeof(T))+extra_size);
            outFile.write((char* )&_reg, sizeof(T));
            outFile.close();
        }
    }


    int get_size(string file){
        int numRecords = 0;
        fstream inFile;
        inFile.open(file, ios::in | ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, ios::end);
            int bytes = inFile.tellg();
            numRecords = (bytes - extra_size) / sizeof(Registro);
            inFile.close();
        } else cout << "Could not open the file.\n";
        return numRecords;
    }

    bool is_empty(string file){
        fstream inFile;
        int bytes=0;
        inFile.open(file, ios::in | ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, ios::end);
            bytes = inFile.tellg();
            inFile.close();
        }
        else{
            return true;
        }
        return (bytes==0);
    }

    template <typename  T>
    T getRegbyPost(int pos,string file){
        ifstream outFile;
        T obj;
        outFile.open(file,ios::in| ios::out| ios::binary);
        if (outFile.is_open()) {
            outFile.seekg((pos * sizeof(T))+extra_size);
            outFile.read((char *) &obj, sizeof(T));
            outFile.close();
        }
        return obj;
    }


    Registro getNext(Registro r){
        
        return getRegbyPost<Registro>(r._next.pos,r._next.file);
    }

    void RebuildData(){

        vector<Registro> _registro;
        Point _heapreg=GetHeader(data_file);
        Registro regtemp=getRegbyPost<Registro>(_heapreg.pos,_heapreg.file);
        _registro.push_back(regtemp);

        int data_file_size=get_size(data_file);
        int aux_file_size=get_size(aux_file);
        int count=1;
        while(count<data_file_size+aux_file_size){
            Registro next=getNext(regtemp);
            _registro.push_back(next);
            regtemp=next;
            count++;
        }
        for(auto it=_registro.begin();it<_registro.end();it++){
            int index = std::distance(_registro.begin(), it);
            Registro _tmi=*(it);
            _tmi._id.pos=index;
            _tmi._id.setFile(data_file);
            if(it+1==_registro.end()){
                _tmi._next.pos=-2;
            } else{
                _tmi._next.pos=index+1;
                _tmi._next.setFile(data_file);
            }
            WriteInPos<Registro>(data_file,_tmi,index);
        }
        //  CleanFile(aux_file);
        _heapreg.pos=0;
        _heapreg.setFile(data_file);
         InsertHeader(_heapreg,data_file);
    }

    void UpdateRecord(Registro _reg){
        WriteInPos(_reg._id.file,_reg,_reg._id.pos);
    }

    void InsertHeader(Point _header,string file){
        fstream outFile;
        outFile.open(file,ios::in| ios::out| ios::binary);
        if(outFile.is_open()){
            outFile.seekg(0);
            outFile.write((char* )&_header, sizeof(Point));
            outFile.close();
        }
    }

    Point GetHeader(string file){
        Point _header;
        fstream outFile;
        outFile.open(file,ios::in| ios::out| ios::binary);
        if(outFile.is_open()){
            outFile.seekg(0);
            outFile.read((char* )&_header, sizeof(Point));
            outFile.close();
        }
        return _header;
    }


public:
    SequentialFile(string _data_file,string _aux_file){
        this->data_file=_data_file;
        this->aux_file=_aux_file;
    }


    // Point GetHeader(string file){
    //     Point _header;
    //     fstream outFile;
    //     outFile.open(file,ios::in| ios::out| ios::binary);
    //     if(outFile.is_open()){
    //         outFile.seekg(0);
    //         outFile.read((char* )&_header, sizeof(Point));
    //         outFile.close();
    //     }
    //     return _header;
    // }

    // void SearchLogic(string key,Registro &prev,Registro &obj){
    //     obj._id.pos=-3;
    //     prev._id.pos=-3;
        
    //     Point _header=GetHeader(data_file);
    //     if(_header.file==data_file){


    //     }else{



    //     }
    // }

    // void BinarySearch2(string key,Registro &prev,Registro&obj){

    //     int data_file_size=get_size(data_file);
    //     int l, u, m;
    //     l = 0;
    //     u = data_file_size- 1;
    //     while (l <= u) {
    //         m = (l + u) / 2;
    //         prev=obj;
    //         obj=getRegbyPost<Registro>(m,data_file);
    //         if (obj.nombre > key) {
    //             u = m - 1;
    //         }
    //         else if (obj.nombre <key) {
    //             l = m + 1;
    //         } else {
    //             break;
    //         }
    //     }

    // }





    Registro Search(string key){

    Registro  tmp;
    tmp._id.pos=-3;
    
     if(!is_empty(data_file)){
              Registro obj=BinarySearchNear(key);
              if (obj.nombre==key){
                //   tmp=&obj;
                  return obj;

                }else{
                    Registro _next;
                    _next._id=obj._next;
                    while(_next._id.pos!=-2){
                        _next=getNext(obj);
                        if(_next.nombre==key){
                            //  tmp=&_next;
                             return _next;
                        }else{
                            obj=_next;
                        }
                    }
                    Point _heapreg=GetHeader(data_file);
                    Registro _firtsreg=getRegbyPost<Registro>(_heapreg.pos,_heapreg.file);
                    if (_firtsreg.nombre==key){
                        // tmp=&_firtsreg;
                        return _firtsreg;

                    }else{
                        _next._id=_firtsreg._next;
                        while(_next._id.pos!=-2){
                            _next=getNext(_firtsreg);
                            if(_next.nombre==key){
                                // tmp=&_next;
                                return _next;
                            }else{
                                _firtsreg=_next;
                            }
                        }
                    }
                    return tmp;
                }
        }
        return tmp;
    }

    void AddRecord(Registro _registro){

         if(is_empty(data_file)){
            Point _header;
            _header.setPost(0);
            _header.setFile(data_file);
            _registro._id=_header;
            _registro._next.pos=-2; //es final tambien.
            InsertHeader(_header,data_file);
            heapAdd(_registro);
         }
         else{

             if(is_empty(aux_file)){
                  Point _auxHeader;
                  _auxHeader.setPost(-1);
                  _auxHeader.setFile(aux_file);
                  InsertHeader(_auxHeader,aux_file);
             }
             int aux_size=get_size(aux_file);
             if(aux_size+1>MAX_AUX){
                RebuildData();
                aux_size=0;
             }

             int data_size=get_size(data_file);
             int aux_file_size=aux_size;
             
             Point _headeraxu=GetHeader(aux_file);
             Point _heapreg=GetHeader(data_file);
             Registro _firtsreg=getRegbyPost<Registro>(_heapreg.pos,_heapreg.file);

             if(_firtsreg>_registro){
                    _registro._next=_firtsreg._id;
                    _registro._id.setFile(aux_file);
                   
                    
                    if(_headeraxu.pos=-1){
                         _registro._id.pos=aux_file_size;
                        heapAdd(_registro);
                    }
                    else{
                        Registro _nexdel=getRegbyPost<Registro>(_headeraxu.pos,_headeraxu.file);
                        InsertHeader(_nexdel._next,aux_file);
                         _registro._id.pos=_headeraxu.pos;
                        WriteInPos(aux_file,_registro,_headeraxu.pos);
                    }
                }
                else{
                        Registro obj=BinarySearchNear(_registro.nombre);
                        Registro next;
                        next._id=obj._next;
                        while(obj._next.pos!=-2){
                            next=getNext(obj);
                            if(next>_registro){
                                break;
                            }
                            obj=next;
                        }
                        _registro._next=obj._next;
                        _registro._id.setFile(aux_file);

                         if(_headeraxu.pos=-1){
                             _registro._id.pos=aux_file_size;
                            heapAdd(_registro);
                         }
                         else{
                            Registro _nexdel=getRegbyPost<Registro>(_headeraxu.pos,_headeraxu.file);
                            InsertHeader(_nexdel._next,aux_file);
                            _registro._id.pos=_headeraxu.pos;
                            WriteInPos(aux_file,_registro,_headeraxu.pos);
                        }                        //actualizar en el datafile
                        obj._next.setFile(aux_file);
                        obj._next.pos=_registro._id.pos;
                        UpdateRecord(obj);
                    }
          }
    }

    void DeleteRecord(string key){
        
        // Registro obj=BinarySearchNear(key);
        // if(_found.nombre==key){
        //     _found._next.pos=-1;
        //     UpdateRecord(_found);  
        // }else{

        //     Registro _next;
        //     _next._id=obj._next;
        //     while(_next._id.pos!=-2){
        //         _next=getNext(obj);
        //         if(_next.nombre==key){
        //                 break;
        //         }else{
        //             obj=_next;
        //         }
        //     }


                    
        }

        // if(_found._id.pos!=-3){
        //     if(_found._id.file==data_file){
        //         _found._next.pos=-1;
        //         UpdateRecord(_found);
        //     }else{
        //         Point _headeraux=GetHeader(aux_file);
                
        //     }
        // }



};



