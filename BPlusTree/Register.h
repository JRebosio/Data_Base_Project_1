
#include <iostream>
using namespace std;

// template<typename Key>
class Register
{
    public:
        //char code[12];
        long long code;
        char nombre[12] ;
        char apellidos[20] ;
        char carrera[30];
        void print(){
            std::cout<<"codigo: "<<code<<string(4,' ')<<" Nombre: "<<nombre<< " Apellidos: "<<apellidos <<" Carrera : "<<carrera<<std::endl;
        }

        Register()=default;

        Register(const long long _code,const std::string name,const string lname,const string Tcarrera){
            // for(int i=0; i<11; i++) code[i] = i < _code.size()? _code[i]:' ';
            // code[11]='\0';
            code=_code;
            for(int i=0; i<11; i++) nombre[i]=i<name.size()? name[i]:' ';
            nombre[11]='\0';
            for(int i=0; i<19; i++) apellidos[i]=i<lname.size()? lname[i]:' ';
            apellidos[19]='\0';
            for(int i=0; i<29; i++) carrera[i]=i<Tcarrera.size()? Tcarrera[i]:' ';
            carrera[29]='\0';
        }    
};