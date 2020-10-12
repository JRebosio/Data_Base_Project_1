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
        bool noDeletedEntries;
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

    bool operator>(const string& d ) const{
        return string{nombre} > d;
    }
    
    bool operator<(const string& d) const {
        return string{nombre} < d;
    }

    bool operator>=(const string& d ) const{
        return string{nombre} >= d;
    }
    
    bool operator<=(const string& d) const {
        return string{nombre} <= d;
    }

    bool operator==(const string& d) const {
        return string{nombre} == d;
    }
     void setData() {
        cout << "Codigo(5): ";
        cin >> codigo;
        cout << "Nombre(20): ";
        cin >> nombre;
        cout << "Carrera(15): ";
        cin >> carrera;
        cout << "Ciclo(15): ";
        cin >> ciclo;
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
        Registro prev;
        while (l <= u) {
            m = (l + u) / 2;
            obj=getRegByPos<Registro>(m,data_file);
            if (obj > key) {
                u = m - 1;
            }
            else if (obj < key) {
                l = m + 1;
            } else {
                return obj;
            }
            prev = obj;
        }
        
        return prev;
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
            //lo crea.
            inFile.clear(); 
            inFile.open(file, ios_base::out); // will create if necessary 
            return true;
        }
        return (bytes==0);
    }


    template <typename  T>
    T getRegByPos(int pos,string file){
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
        
        return getRegByPos<Registro>(r._next.pos,r._next.file);
    }

    void RebuildData(){

        Point _heapreg=GetHeader(data_file);
        auto aux_hd = GetHeader(aux_file);
        Registro obj = getRegByPos<Registro>(_heapreg.pos, _heapreg.file);
        int index = 0;
        
        while (obj._next.pos != -2){
            auto next = getNext(obj);

            if (obj._id.file == aux_file){
                aux_hd.pos = obj._id.pos;
                obj._next = aux_hd;
                UpdateRecord(obj);
            }
            
            obj._id.pos = index;
            obj._id.setFile(data_file);
            obj._next.pos = index+1;
            obj._next.setFile(data_file);
            
            WriteInPos(data_file, obj, index);
            obj = next;
            index++;
        }
        if (obj._id.file == aux_file)
            aux_hd.pos = obj._id.pos;
        obj._id.pos = index;
        obj._id.setFile(data_file);
        WriteInPos(data_file, obj, index);

        aux_hd.size = 0;
        InsertHeader(aux_hd, aux_file);
        _heapreg.pos=0;
        _heapreg.noDeletedEntries = true;
        _heapreg.setFile(data_file);
        InsertHeader(_heapreg,data_file);

        // Registro regtemp=getRegByPos<Registro>(_heapreg.pos,_heapreg.file);
        // _registro.push_back(regtemp);

        // int data_file_size=get_size(data_file);
        // int aux_file_size=get_size(aux_file);
        // int count=1;
        // while(count<data_file_size+aux_file_size){
        //     Registro next=getNext(regtemp);
        //     _registro.push_back(next);
        //     regtemp=next;
        //     count++;
        // }
        // for(auto it=_registro.begin();it<_registro.end();it++){
        //     int index = std::distance(_registro.begin(), it);
        //     Registro _tmi=*(it);
        //     _tmi._id.pos=index;
        //     _tmi._id.setFile(data_file);
        //     if(it+1==_registro.end()){
        //         _tmi._next.pos=-2;
        //     } else{
        //         _tmi._next.pos=index+1;
        //         _tmi._next.setFile(data_file);
        //     }
        //     WriteInPos<Registro>(data_file,_tmi,index);
        // }
        
        //  CleanFile(aux_file);
        
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


    bool SequentialSearchPrev(string key, pair<Registro, Registro> &res){
        Point hd = GetHeader(data_file);
        res.first = getRegByPos<Registro>(hd.pos, hd.file);
        while(res.first._next.pos!=-2){
            if(res.first.nombre==key)
                return true;
            else {
                res.second = res.first;
                res.first=getNext(res.first);
            }
        }
        return false; 
    }

    bool SequentialSearchFromPoint(string key, Point next, pair<Registro, Registro> &res){
        res.first = getRegByPos<Registro>(next.pos, next.file);
        while(res.first._next.pos!=-2){
            if(res.first.nombre==key)
                return true;
            else {
                res.second = res.first;
                res.first=getNext(res.first);
            }
        }
        return false;
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
    //         obj=getRegByPos<Registro>(m,data_file);
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







    bool Search(string key,Registro &tmp){

        if(!is_empty(data_file)){
            Point hd=GetHeader(data_file);         
            if   (hd.noDeletedEntries) {
                cout << "No deleted entries, performing BinarySearchNear. Size of datafile: " << get_size(data_file) << endl;
                Registro obj = BinarySearchNear(key);
                cout << "Nearest obj: " << obj.nombre << "  Nearest pos: " << obj._id.pos << endl;
                if (obj==key)
                    {
                        tmp=obj;
                        return true;
                     }
                else{
                        cout << "Couldn't find record in datafile, starting sequential search from " << obj._id.pos << endl;
                        Registro _seq = SequentialNearSearchFromPoint(key, hd);
                        cout << "Nearest obj: " << _seq.nombre << "  Nearest pos: " << _seq._id.pos << endl;
                        if(_seq==key)
                        {
                                tmp=_seq;
                                return true;
                        }
                            return false;
                    }
            }
            else{
                    Registro _seq = SequentialNearSearchFromPoint(key, hd);
                            if(_seq==key){
                                tmp=_seq;
                                return true;
                            }
                            return false;              
            }
            return false;//aca
        }

        return false;
    }

    void AddRecord(Registro _registro){

         if(is_empty(data_file)){
            Point _header;
            _header.setPost(0);
            _header.setFile(data_file);
            _header.noDeletedEntries=true;
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
                _auxHeader.size = 0;
                InsertHeader(_auxHeader,aux_file);
            }
            auto aux_hd = GetHeader(aux_file);
            if(aux_hd.size+1>MAX_AUX){
                RebuildData();
                aux_hd = GetHeader(aux_file);
            }

            Point _heapreg=GetHeader(data_file);
            Registro _firtsreg=getRegByPos<Registro>(_heapreg.pos,_heapreg.file);

             if(_firtsreg>_registro)
            {
                _registro._next=_firtsreg._id;
                _registro._id.setFile(aux_file);
            
                
                if(aux_hd.pos=-1)
                {
                    _registro._id.pos= aux_hd.size;;
                    heapAdd(_registro);
                }
                else
                {
                    Registro nextDel=getRegByPos<Registro>(aux_hd.pos,aux_hd.file);
                    aux_hd.pos = nextDel._next.pos;
                    aux_hd.setFile(nextDel._next.file);
                    InsertHeader(aux_hd,aux_file);
                    _registro._id.pos=nextDel._id.pos;
                    WriteInPos(nextDel._id.file, _registro, nextDel._id.pos);
                }
            }
            
            else{

                Registro obj;
                if(_heapreg.noDeletedEntries){
                        obj=BinarySearchNear(_registro.nombre);

                        if(obj._next.pos!=-2){
                            obj=SequentialNearSearchFromPoint(_registro.nombre,_heapreg);
                            cout << "Sequential search nearest result for " << _registro.nombre << ": " << obj.nombre<<endl;
                        }
                    
                }else{
                        obj=SequentialNearSearchFromPoint(_registro.nombre,_heapreg);
                }
                    cout << "Inserting " << _registro.nombre << " after " << obj.nombre << endl;
                    _registro._next=obj._next;
                    _registro._id.setFile(aux_file);

                    if(aux_hd.pos==-1){
                        _registro._id.pos=aux_hd.size;
                    heapAdd(_registro);
                    }
                    else{
                        Registro nextDel=getRegByPos<Registro>(aux_hd.pos,aux_hd.file);
                        _registro._id.pos=aux_hd.pos;
                        aux_hd.pos = nextDel._next.pos;
                        aux_hd.setFile(nextDel._next.file);
                        InsertHeader(aux_hd,aux_file);
                        WriteInPos(nextDel._id.file, _registro, nextDel._id.pos);
                    }
                    obj._next.setFile(aux_file);
                    obj._next.pos=_registro._id.pos;
                    UpdateRecord(obj);          
                        
            }
            aux_hd.size++;
            InsertHeader(aux_hd, aux_file);
          }
    }

    Registro internalSearch(string key){
        Point hd = GetHeader(data_file);

        if (hd.noDeletedEntries)
            return BinarySearchNear(key);
        else
            return SequentialNearSearch(key);
    }


    void updateDeletion(pair<Registro, Registro> res){
        auto hd = GetHeader(data_file);
        if (res.first._id.pos != hd.pos){
            res.second._next = res.first._next;
            UpdateRecord(res.second);
        }
        if (res.first._id.file == aux_file){
            Point aux_hd = GetHeader(aux_file);
            res.first._next = aux_hd;
            aux_hd = res.first._id;
            aux_hd.size--;
            InsertHeader(aux_hd, aux_file);
        }
        else
            res.first._next.pos = -1;
        
        UpdateRecord(res.first);  
        return;  
    }

    bool DeleteRecord(string key){
        Point hd = GetHeader(data_file);
        
        if (hd.file == data_file && hd.noDeletedEntries){
            cout << "No deberia entrar aca" << endl;
            Registro obj=BinarySearchNear(key);
            if(obj.nombre==key){
                if (obj._id.pos != hd.pos){
                    
                    auto prev = getRegByPos<Registro>(obj._id.pos-1, data_file);
                    // hd.noDeletedEntries = false;
                    // InsertHeader(hd, data_file);
                    prev._next = obj._next;
                    UpdateRecord(prev);
                }
                else{
                     hd=obj._next;
                }
                obj._next.pos=-1;
                UpdateRecord(obj);  
                hd.noDeletedEntries = false;
                InsertHeader(hd, data_file);
                return true;
            }
            else {
                pair<Registro, Registro> res;
                if(SequentialSearchFromPoint(key, obj._next, res)){
                    updateDeletion(res);
                    hd.noDeletedEntries = false;
                    InsertHeader(hd, data_file);
                    return true;
                }
            }
        }
        else{
            pair<Registro, Registro> res;
            if (SequentialSearchPrev(key, res)){
                cout << "Succesful sequential search" << endl;
                updateDeletion(res);   
                hd.noDeletedEntries = false;
                InsertHeader(hd, data_file);
                return true;
            }
        }
        return false;
    
    }

    Registro SequentialNearSearch(string key){
        Point hd = GetHeader(data_file);
        Registro obj = getRegByPos<Registro>(hd.pos, hd.file);
        Registro prev = obj;
        while(obj._next.pos!=-2){
            if( obj == key)
                return obj;
            else if (obj > key)
                return prev;
            else {
                prev=obj;
                obj=getNext(obj);
            }
        } 
        return obj;
    }



     Registro SequentialNearSearchFromPoint(string key, Point next){
        cout << "Looking to insert " << key << endl;
        Registro obj = getRegByPos<Registro>(next.pos, next.file);
        Registro prev = obj;
        while(obj._next.pos!=-2){
            if(obj == key)
                return obj;
            else if (obj > key){
                cout << "Found entry " << obj.nombre << " > " << key << ". Returning " << prev.nombre << endl;
                return prev;}
            else {
                prev = obj;
                obj=getNext(obj);
            }
        }
        if (obj > key){
            cout << "Found entry " << obj.nombre << " > " << key << ". Returning " << prev.nombre << endl;
            return prev;}
        cout << "Got to the end of list, returning last element: " << obj.nombre << endl;
        return obj;
    }


    Registro SequentialUpperSearch(string key){
        Point hd = GetHeader(data_file);
        Registro obj = getRegByPos<Registro>(hd.pos, hd.file);
        while(obj._next.pos!=-2){
            if( obj >= key)
                return obj;

            else {
                obj=getNext(obj);
            }
        } 
        return obj;
    }

     Registro SequentialUpperSearchFromPoint(string key, Point next){
        Registro obj = getRegByPos<Registro>(next.pos, next.file);
        while(obj._next.pos!=-2){
            if(obj >= key)
                return obj;
            else {
                obj=getNext(obj);
            }
        } 
        return obj;
    }

    void scanAll(string file) {
        ifstream inFile;
        inFile.open(file, ios::binary);
        //read the first pointer
        Point delPointer;
        inFile.read((char *) &delPointer, sizeof(Point));
        //read the records
        Registro obj;
        while (inFile.read((char *) &obj, sizeof(obj))) {
            // if (obj._next.pos !=-1)
                obj.showData();
        }
        inFile.close();
    }


    
};



