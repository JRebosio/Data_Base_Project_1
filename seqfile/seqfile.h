#ifndef SEQFILE_H
#define SEQFILE_H


struct Point{
public:
    char file[20];
    int pos;
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
    string header_file;


public:
    SequentialFile(string _data_file,string _aux_file,string header_file){
        this->data_file=_data_file;
        this->aux_file=_aux_file;
        this->header_file=header_file;
    }

    template <typename  T>
    void scanAll(string file){
        ifstream inFile;
        inFile.open(file,ios::binary);
        T obj;
        cout<<file<<endl;
        cout<<"--------------------------------"<<endl;
        if(inFile.is_open()){
            while (inFile.read((char *) &obj, sizeof(T))){
                //if(obj._next.pos!=-1){
                    obj.showData();
                //}
            }
            cout<<"--------------------------------"<<endl;
            inFile.close();
        }else cout<<"No se puede abrir el archivo.\n";
    }

    void InsertAll(vector<Registro> _registros){
        for(auto it=_registros.begin();it<_registros.end();it++){
            AddRegistro((*it));
        }
    }



    Registro Search(string key){

        Registro obj=BinarySearchNear(key,data_file);
        if (obj.nombre==key){
            return obj;
        }else{
            Registro _next;
            _next._id=obj._next;
            while(_next._id.pos!=-2){
                _next=getNext(obj);
                if(_next.nombre==key){
                    return _next;
                }else{
                    obj=_next;
                }
            }
            Point _heapreg=getRegbyPost<Point>(0,header_file);
            Registro _firtsreg=getRegbyPost<Registro>(_heapreg.pos,_heapreg.file);
            if (_firtsreg.nombre==key){
                return _firtsreg;

            }else{
                _next._id=_firtsreg._next;
                while(_next._id.pos!=-2){
                    _next=getNext(_firtsreg);
                    if(_next.nombre==key){
                        return _next;
                    }else{
                        _firtsreg=_next;
                    }
                }
            }
            cout<<"Nose encontro Registro"<<endl;
            return _next;
        }
    }

    vector<Registro> search(string begin, string end){

        vector<Registro> _result;
        Point _heapreg=getRegbyPost<Point>(0,header_file);
        Registro _firtsreg=getRegbyPost<Registro>(_heapreg.pos,_heapreg.file);
        Registro _firtdata=getRegbyPost<Registro>(0,data_file);

        Registro _next;

            if(begin>=_firtdata.nombre){
                Registro _obj=BinarySearchNear(begin,data_file);
                _next._id=_obj._next;
                while(_next._id.pos!=-2) {
                    _next=getNext(_obj);
                    if(_next.nombre<end){
                        _result.push_back(_next);
                        _obj=_next;
                    }else{
                        break;
                    }
                }
                return  _result;
            }else if(_firtdata.nombre>begin && _firtsreg.nombre<=begin){
                    Registro _obj=_firtdata;
                    _next._id=_obj._next;
                    while( _next._id.pos!=-2  ){
                        _next=getNext(_obj);
                        if(_next.nombre<end){
                            _result.push_back(_next);
                            _obj=_next;
                        }else{
                            break;
                        }
                    }
                return  _result;

            }else{
                return _result;
            }

    }

    void AddRegistro(Registro _registro){

            int aux_size=get_size(aux_file);
            int data_file_size=get_size(data_file);
            int aux_file_size=(aux_size-1);
            if(data_file_size==0){
                Point _header;
                _header.pos=0;
                _header.setFile(data_file);
                _registro._id=_header;
                _registro._next.pos=-2; //es final tambien.
                heapAddFilePost(header_file,_header,0);
                heapAdd(_registro);
            }else{

                if(aux_size+1>MAX_AUX){
                    RebuildData();
                    aux_file_size=get_size(data_file);
                    aux_size=get_size(aux_file);
                    aux_file_size=(aux_size-1);
                }
                Point _heapreg=getRegbyPost<Point>(0,header_file);
                Registro _firtsreg=getRegbyPost<Registro>(_heapreg.pos,_heapreg.file);

                if(_firtsreg>_registro){
                    _registro._next=_firtsreg._id;
                    _registro._id.setFile(aux_file);
                    _registro._id.pos=aux_file_size+1;
                    heapAdd(_registro);
                    heapAddFilePost(header_file,_registro._id,0);
                }
                else{
                        Registro obj=BinarySearchNear(_registro.nombre,data_file);
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
                        _registro._id.pos=aux_file_size+1;
                        heapAdd(_registro);
                        //actualizar en el datafile
                        obj._next.setFile(aux_file);
                        obj._next.pos=aux_file_size+1;
                        UpdateRecord(obj);
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
    void heapAddFilePost(string file,T _reg,int pos){
        fstream outFile;
        outFile.open(file,ios::in| ios::out| ios::binary);
        if(outFile.is_open()){
            outFile.seekg((pos) * sizeof(T));
            outFile.write((char* )&_reg, sizeof(T));
            outFile.close();
        }
    }


    template <typename  T>
    void heapAddFile(string file,T _reg){
        fstream outFile;
        outFile.open(file,ios::in| ios::out| ios::binary);
        if(outFile.is_open()){
            outFile.seekg(0,ios::end);
            outFile.write((char* )&_reg, sizeof(T));
            outFile.close();
        }
    }

    Registro BinarySearchNear(string key,string file){
        fstream outFile;
        outFile.open(data_file,ios::in| ios::out |ios::binary);
        int data_file_size=get_size(file);
        int l, u, m;
        l = 0;
        u = data_file_size- 1;
        Registro obj;
        if (outFile.is_open()) {
            while (l <= u) {
                m = (l + u) / 2;
                outFile.seekg(m * sizeof(Registro));
                outFile.read((char *) &obj, sizeof(Registro));

                if (obj.nombre > key) {
                    u = m - 1;
                }
                else if (obj.nombre <key) {
                    l = m + 1;
                } else {
                    return obj;
                }
            }
            outFile.close();
        }
    }




    void UpdateRecord(Registro _reg){
        fstream outFile;
        outFile.open(_reg._id.file,ios::in| ios::out| ios::binary);
        if(outFile.is_open()){
            outFile.seekg((_reg._id.pos) * sizeof(Registro));
            outFile.write((char* )&_reg, sizeof(Registro));
            outFile.close();
        }
    }

    int get_size(string file){
        int numRecords = 0;
        fstream inFile;
        inFile.open(file, ios::in | ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, ios::end);
            long bytes = inFile.tellg();
            numRecords = bytes / sizeof(Registro);
            inFile.close();
        } else cout << "Could not open the file.\n";
        return numRecords;
    }

    template <typename  T>
    T getRegbyPost(int pos,string file){
        ifstream outFile;
        T obj;
        outFile.open(file,ios::in| ios::out| ios::binary);
        if (outFile.is_open()) {
            outFile.seekg(pos * sizeof(T));
            outFile.read((char *) &obj, sizeof(T));
            outFile.close();
        }
        return obj;
    }

    Registro getNext(Registro r){
        ifstream outFile;
        Registro obj;
        obj._id.pos=-1;
        outFile.open(r._next.file,ios::in| ios::out| ios::binary);
        if (outFile.is_open()) {
            outFile.seekg(r._next.pos * sizeof(Registro));
            outFile.read((char *) &obj, sizeof(Registro));
            outFile.close();
        }
        return obj;
    }

    void RebuildData(){
        vector<Registro> _registro;
        Point _heapreg=getRegbyPost<Point>(0,header_file);
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
            heapAddFilePost<Registro>(data_file,_tmi,index);
        }
         CleanFile(aux_file);
        _heapreg.pos=0;
        _heapreg.setFile(data_file);
         heapAddFilePost(header_file,_heapreg,0);
    }


    void CleanFile(string file){
        ofstream ofs;
        ofs.open(file, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }




};


#endif