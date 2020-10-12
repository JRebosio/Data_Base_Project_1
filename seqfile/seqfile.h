#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>


#define MAX_AUX 20

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
        void setPos(int pos){
            this->pos=pos;
        }
        void showData(){
            cout<<"\nFile: "<<file;
            cout<<"\nPos: "<<pos;
            cout<<endl;
        }
        void showData(ostream &stream){
            stream<<"\nFile: "<<file;
            stream<<"\nPos: "<<pos;
            stream<<endl;
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

    // Registro(string _codigo,string _nombre, string _carrera, int ciclo){
       
    //      this->ciclo=ciclo;
    // }

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

     void showData(ostream &stream){
            stream<<"\nId:";
            _id.showData(stream);
            stream<<"\nCodigo: "<<codigo;
            stream<<"\nNombre: "<<nombre;
            stream<<"\nCarrera: "<<carrera;
            stream<<"\nCiclo:"<<ciclo;
            stream<<"\nNext:";
            _next.showData(stream);
            stream<<endl;
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

        Point header=GetHeader(data_file);
        auto aux_hd = GetHeader(aux_file);
        Registro obj = getRegByPos<Registro>(header.pos, header.file);
        // cout << "Initial record: " << obj.nombre << "   Next pos: " << obj._next.pos << endl;
        vector<Registro> records;
        int index = 0;
        
        while (obj._next.pos != -2){
            auto next = getNext(obj);
            if (obj._id.file == aux_file){
                obj._next = aux_hd;
                aux_hd.pos = obj._id.pos;
                UpdateRecord(obj);
            }
            records.push_back(obj);
            obj = next;
            index++;
        }
        if (obj._id.file == aux_file){
            obj._next = aux_hd;
            aux_hd.pos = obj._id.pos;
            UpdateRecord(obj);
        }
        records.push_back(obj);

        index = 0;
        while (index < records.size()){
            records[index]._id.pos = index;
            records[index]._id.setFile(data_file);
            if (index+1 == records.size())
                records[index]._next.pos = -2;
            else
                records[index]._next.pos = index+1;
            records[index]._next.setFile(data_file);
            WriteInPos(data_file, records[index], index);
            index++;
        }

        aux_hd.size = 0;
        InsertHeader(aux_hd, aux_file);
        header.pos=0;
        header.noDeletedEntries = true;
        header.setFile(data_file);
        InsertHeader(header,data_file);
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

    bool Search(string key,Registro &tmp){
        if(!is_empty(data_file)){
            Point hd=GetHeader(data_file);         
            if (hd.noDeletedEntries) {
                Registro obj = BinarySearchNear(key);
                if (obj==key){
                    tmp=obj;
                    return true;
                }
                else{
                    Registro _seq = SequentialNearSearchFromPoint(key, hd);
                    if(_seq==key){
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
        // cout << "Looking to insert record: " << _registro.nombre << endl;
        if(is_empty(data_file)){
            // cout << "Datafile empty, inserting at top." << endl;
            Point _header;
            _header.setPos(0);
            _header.setFile(data_file);
            _header.noDeletedEntries=true;
            _registro._id=_header;
            _registro._next.pos=-2; //es final tambien.
            InsertHeader(_header,data_file);
            heapAdd(_registro);
        }
        else{
            if(is_empty(aux_file)){
                // cout << "Aux file empty, creating header." << endl;
                Point aux_hd;
                aux_hd.setPos(-1);
                aux_hd.setFile(aux_file);
                aux_hd.size = 0;
                InsertHeader(aux_hd,aux_file);
            }
            auto aux_hd = GetHeader(aux_file);
            if(aux_hd.size+1>MAX_AUX){
                // cout << "Rebuild data start" << endl;
                RebuildData();
                aux_hd = GetHeader(aux_file);
            }

            Point header = GetHeader(data_file);
            Registro first_rec = getRegByPos<Registro>(header.pos,header.file);

             if(first_rec > _registro)
            {
                // cout << _registro.nombre << " < " << first_rec.nombre << ", inserting at top." << endl;
                // first_rec.showData();
                _registro._next = first_rec._id;
                _registro._id.setFile(aux_file);
            
                
                if(aux_hd.pos == -1)
                {
                    // cout << "No files deleted, adding via heapAdd on pos " << aux_hd.size << endl;
                    _registro._id.pos = aux_hd.size;
                    heapAdd(_registro);
                }
                else
                {
                    Registro nextDel=getRegByPos<Registro>(aux_hd.pos,aux_hd.file);
                    // cout << "Inserting at position " << nextDel._id.pos << " as indicated by the aux header" << endl;;
                    aux_hd.pos = nextDel._next.pos;
                    aux_hd.setFile(nextDel._next.file);
                    InsertHeader(aux_hd,aux_file);
                    _registro._id.pos=nextDel._id.pos;
                    WriteInPos(nextDel._id.file, _registro, nextDel._id.pos);
                }
                header.pos = _registro._id.pos;
                header.setFile(_registro._id.file);
                InsertHeader(header, data_file);
            }
            
            else{

                Registro obj;
                if(header.noDeletedEntries){
                    // cout << "No deleted entries in datafile, performing binary search." << endl;
                    obj=BinarySearchNear(_registro.nombre);
                    // cout << "Closest record found: " << obj.nombre << endl;
                    if(obj._next.pos!=-2){
                        // cout << "Performing sequential search" << endl;
                        obj=SequentialNearSearchFromPoint(_registro.nombre,header);
                        // cout << "Closest record found: " << obj.nombre << endl;
                    }
                    
                }
                else{
                    // cout << "Performing sequential search" << endl;
                    obj=SequentialNearSearchFromPoint(_registro.nombre,header);
                    // cout << "Closest record found: " << obj.nombre << endl;                    
                }
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
                // cout << "Inserting " << _registro.nombre << " after " << obj.nombre << endl;
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
            Registro obj=BinarySearchNear(key);
            if(obj.nombre==key){
                if (obj._id.pos != hd.pos){
                    auto prev = getRegByPos<Registro>(obj._id.pos-1, data_file);
                    prev._next = obj._next;
                    UpdateRecord(prev);
                }
                else
                     hd=obj._next;

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
        Registro obj = getRegByPos<Registro>(next.pos, next.file);
        Registro prev = obj;
        while(obj._next.pos!=-2){
            if(obj == key)
                return obj;
            else if (obj > key){
                return prev;}
            else {
                prev = obj;
                obj=getNext(obj);
            }
        }
        if (obj > key)
            return prev;
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

    void scanAll(string file ,ostream &stream) {
        ifstream inFile;
        inFile.open(file, ios::binary);
        //read the first pointer
        Point delPointer;
        inFile.read((char *) &delPointer, sizeof(Point));
        //read the records
        Registro obj;
        while (inFile.read((char *) &obj, sizeof(obj))) {
            // if (obj._next.pos !=-1)
                obj.showData(stream);
        }
        inFile.close();
    }

};



