#include <fstream>
#include <iostream>
#include "BPlusNode.h"
#include "page.h"
using namespace std;

#define Node BPlusNode

class BPlusTree{
    public:
        string indexfile;

        // BPlusNode* head=nullptr;
        
        
        BPlusTree()=default;


        BPlusTree(const std::string file){
            indexfile=file;
        }


        // BPlusTree(const std::string file){
        //     std::fstream ss;
        //     ss.open(file, std::ios::in |std::ios::out | std::ios::app);
        //     if( ss.peek() != std::ifstream::traits_type::eof()){
        //            pos_root="0"; 
        //     }
        // }



        bool insert(Register registro, ll &key, Node* &node){
            
            int pos = 0;
            for(int i=0;i<node->count;i++)
            { 
                if(key > node->keys[i]) pos++;
                else break;
            }

            if(!node->isLeaf){
                  ll postemp= node->children[pos];  
                  Node* temp=readnode(postemp);  
                  if(insert(registro, key, temp)){
                      for(int i=temp->count; i>pos; i-- ){
                          node->keys[i]=node->keys[i-1];
                          node->children[i+1]=node->children[i];
                      }

                       node->keys[pos] = key;
                    
                       node->children[pos+1] =temp->pos; 
                       node->count++;  
                       writenode(node);
                  }
            }

            else {
                Node* temp= readnode(node->children[pos-1]);
                Pagina page(to_string(temp->children[0]) + ".txt");
                 if(page.size==MAX_RECORDS){
                     Pagina newpage;
                    /*  Insertamos en el registro en la pagina y lo separamos en dos  */
                    page.registers.emplace_back(registro);
                    page.sort();    /* TO DO */
                    newpage.registers.insert(newpage.registers.begin(),page.registers.begin() + MAX_RECORDS/2, page.registers.end());
                    page.registers.erase(page.registers.begin() + MAX_RECORDS/2, page.registers.end());
                    newpage.setName();
                    /* Hace que los keys apuntan a su respectiva pagina */ 
                    for(int i = node->count; i>pos; i--){
                        node->keys[i] = node->keys[i-1];
                        node->children[i] = node->children[i-1];
                    }
                    node->keys[pos] = newpage.registers[0].code;
                   //MENSAJE DE JORGE ERROR AQUI node->children[pos] = stnewpage.name; 
                   
                   
                    node->count++;
                    writenode(node);
                    //nodo.write();
                    page.write();
                    newpage.write();
                }else{
                    page.registers.emplace_back(registro);
                    page.size++;
                    page.sort();
                    page.write(); /* TO DO */
                }
            }


            if(node->count == Size){
                if(node->isLeaf) splitLeaf(key,node);
                else split(key,node);
                return true;
            }
            return false;


        }



        bool insert(Register registro) {
           
            // Node* root=read_node(indexfile, 0);
            fstream ss;
            ss.open(indexfile, std::ios::in |std::ios::out | std::ios::app);
            
            if (ss.peek() == std::ifstream::traits_type::eof()){
                 cout<<"F"<<endl;
                // pos_root="0";
                 Node newroot(true);
                // newroot.position=0;
                 newroot.count++;
                 Pagina newPage;
                 cout<<"F"<<endl;
                 newPage.registers.emplace_back(registro);
                 newPage.size++;
                 newPage.setName();
                 
                 newroot.keys[0]= registro.code; // 2018...
                // newroot.children[0]= stoll(newPage.name);// 201820025.txt;
                newroot.children[0]= 0;


                 ss.seekg(0,ss.beg);
                 ss.write((char*) &newroot,sizeof(Node));
                 ss<<'\n';
                 ss.close();

                 newPage.write();
                 return true;
            }

            else{
                Node* root;
                root=readnode(0);
                ll key=registro.code;
                if(insert(registro,key, root)){
                    Node* newNode=new Node(false);
                    Node* check;
                    newNode = check;
                    check=readnode(0);
                    check->pos=-1;
                    writenode(check);

                    newNode->isLeaf=0;
                    newNode->pos=0;
                    newNode->keys[0]=key;
                    newNode->children[0]=check->pos;
    
                    newNode->children[1]=root->pos;
                    newNode->count++;
        //          newNode.print_node();
                    /*Aqui esta el problema*/
                   
                    writenode(newNode);

                }


            }
            
            return true;
        }


        Node* readnode(ll pos){
            Node* node;
            fstream ss;
            ss.open(indexfile, std::ios::in |std::ios::out | std::ios::app);
            ss.seekg(pos*(sizeof(Node)+1), ios::beg);
            ss.read((char *)(&(*node)),sizeof(Node));
            ss.close();
            return node;
        }


        void writenode(Node* node){
            fstream ss;
            ss.open(indexfile, std::ios::in |std::ios::out | std::ios::app);
            ss.seekp(node->pos*(sizeof(Node)+1), ios::beg);
            ss.write((char *)(&(*node)),sizeof(Node));
            ss<<'\n';
            ss.close();
        }


};