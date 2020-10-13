#ifndef BTREE_H
#define BTREE_H



#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>

using namespace std;
          //3
//3      //1 5 
//4

template<typename T,int ORDER>
struct Node 
{
    long address{0};
	T entries[ORDER+1]; 
	long children[ORDER+2];
	bool is_leaf=false;
	long count{0};
    long right{0};

    // Node(int _ORDER): 

    //8

    //              2 | 7  | 20
    //             /   /    /   /    

    //8
    void insert_in_node(int pos,const T&value,long _children) {
        int j = count;
            while (j > pos) {
                entries[j] = entries[j - 1];
                children[j + 1] = children[j];
                j--;
            }
        entries[j] = value;
        children[j + 1] = _children;
        count++;
    }

     void delete_in_node(int pos) {
        for (int i = pos; i < count; i++) {
            entries[i] = entries[i + 1];
            children[i + 1] = children[i + 2];
        }
        count--;
     }



    bool is_overflow() { return count > ORDER; }

    bool is_underflow() { return count < floor(ORDER / 2.0); }

};



struct Registro
{
public:
    //int pos;
    long codigo;
    char nombre[20];
    char carrera[15];
    int ciclo;

};

template <typename T,int ORDER>
class btree{

private:
    string indexfile;
    string datafile;
public:
    btree(string _indexfile,string _datafile){
        this->indexfile=_indexfile;
        this->datafile=_datafile;
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


// ///////////////

//         void insert(const T &value) {
//             node root = read_node(header.root_id);
//             int state = insert(root, value);

//             if (state == BT_OVERFLOW) {
//                 split_root();
//             }
//         }

//         int insert(node &ptr, const T &value) {
//             int pos = 0;
//             while (pos < ptr.count && ptr.data[pos] < value) {
//                 pos++;
//             }
//             if (ptr.children[pos] != 0) {
//                 long page_id = ptr.children[pos];
//                 node child = read_node(page_id);
//                 int state = insert(child, value);
//                 if (state == BT_OVERFLOW) {
//                     split(ptr, pos);
//                 }
//             } else {
//                 ptr.insert_in_node(pos, value);
//                 write_node(ptr.page_id, ptr);
//             }
//             return ptr.is_overflow() ? BT_OVERFLOW : NORMAL;
//         }
// /////////////77


    void insert(Registro _reg){
        if(is_empty(indexfile)){
            long pos = HeadAddReg(_reg);
            Node<T,ORDER> root;
            root.insert_in_node(0,_reg.codigo,pos);
            root.is_leaf=true;
            root.address=0;
            WriteNode(root.address,root);
            //creamos el nodo root
        }else{
            auto root=readNode(0);
            long pos_file = HeadAddReg(_reg);
            if(root.is_leaf){
                int pos=getPostoInsertinChildren(root,_reg.codigo);//1
                root.insert_in_node(pos,_reg.codigo,pos_file);
                if(!root.is_overflow()){
                    WriteNode(root.address,root);
                }else{
                     split_root(root);
                }
            }else{
                 auto current=root;
                 Node<T,ORDER> _parent;
                 int pos_in_parent;
                 int pos_=GetLeaftToInsert(_reg.codigo,current,_parent, pos_in_parent);
                 current.insert_in_node(pos_,_reg.codigo,pos_file);
                 if(!root.is_overflow()){
                    WriteNode(current.address,root);
                }else{
                     split_node(_parent, pos_in_parent);
                }
            }
        }
    }


    void split_node(Node<T,ORDER> &parent,int pos){
        //tener en cuenta que los childres son +1
        auto to_split = readNode(parent.children[pos]);
        auto left = to_split;
        left.count=0;
        Node<T,ORDER> right;
        right.address=getNextPostToInsertIndex(indexfile);
        int iter = 0;
        int i;

        for (i = 0; iter < ceil(ORDER / 2.0); i++) {
                left.children[i] = to_split.children[iter];
                left.entries[i] = to_split.entries[iter];
                left.count++;
                iter++;
            }
         left.children[i] = to_split.children[iter];
         parent.insert_in_node(pos,to_split.entries[iter],left.address);

            if (to_split.children[0] != 0) {
                iter++;
            } else {
                right.right = left.right;
                left.right = right.address;
                parent.children[pos + 1] = right.address;
            }

                for (i = 0; iter < ORDER + 1; i++) {
                    right.children[i] = to_split.children[iter];
                    right.entries[i] = to_split.entries[iter];
                    right.count++;
                    iter++;
                }
                right.children[i] = to_split.children[iter];

                parent.children[pos] = left.address;
                parent.children[pos + 1] = right.address;

                WriteNode(parent.address, parent);
                WriteNode(left.address, left);
                WriteNode(right.address, right);
    }

    // //TEMPORAL
    //  void split(node &parent, int pos) {
    //             node node_in_overflow = this->read_node(parent.children[pos]);
    //             node child1 = node_in_overflow;
    //             child1.count = 0;
    //             node child2 = this->new_node();

    //             int iter = 0;
    //             int i;
    //             for (i = 0; iter < ceil(ORDER / 2.0); i++) {
    //                 child1.children[i] = node_in_overflow.children[iter];
    //                 child1.data[i] = node_in_overflow.data[iter];
    //                 child1.count++;
    //                 iter++;
    //             }
    //             child1.children[i] = node_in_overflow.children[iter];

    //             parent.insert_in_node(pos, node_in_overflow.data[iter]);

    //             if (node_in_overflow.children[0] != 0) {
    //                 iter++;
    //             } else {
    //                 child2.right = child1.right;
    //                 child1.right = child2.page_id;
    //                 parent.children[pos + 1] = child2.page_id;
    //             }

    //             for (i = 0; iter < BTREE_ORDER + 1; i++) {
    //                 child2.children[i] = node_in_overflow.children[iter];
    //                 child2.data[i] = node_in_overflow.data[iter];
    //                 child2.count++;
    //                 iter++;
    //             }
    //             child2.children[i] = node_in_overflow.children[iter];

    //             parent.children[pos] = child1.page_id;
    //             parent.children[pos + 1] = child2.page_id;

    //             write_node(parent.page_id, parent);
    //             write_node(child1.page_id, child1);
    //             write_node(child2.page_id, child2);
    //         }


   long getNextPostToInsertIndex(string _file){
        fstream inFile;
        long pos_to_insert;
        inFile.open(_file, ios::in | ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, ios::end);
            pos_to_insert = inFile.tellg();
            inFile.close();
        }
        return pos_to_insert;
   }

   void split_root(Node<T,ORDER> &root){

       Node<T,ORDER> left;
       Node<T,ORDER> right;
       
        int pos = 0;
        int iter = 0;
        int i;
        for (i = 0; iter < ceil(ORDER / 2.0); i++) {
            left.children[i] = root.children[iter];
            left.entries[i] = root.entries[iter];
            left.count++;
            iter++;
        }        
        left.children[i] = root.children[iter];
        root.entries[0] = root.entries[iter];
        
        left.address=getNextPostToInsertIndex(indexfile);
        right.address=left.address+sizeof(Node<T,ORDER>);
        left.right = right.address;
        left.is_leaf=true;
        right.is_leaf=true;

        if (root.children[0] != 0) {
            iter++; // the middle element
        }

        for (i = 0; iter < ORDER + 1; i++) {
            right.children[i] = root.children[iter];
            right.entries[i] = root.entries[iter];
            right.count++;
            iter++;
        }
        right.children[i] = root.children[iter];

        root.children[0] = left.address;
        root.children[1] = right.address;
        root.count = 1;
        root.is_leaf=false;

        WriteNode(root.address, root);
        WriteNode(left.address, left);
        WriteNode(right.address, right);

   }

   int getPostoInsertinChildren(Node<T,ORDER> const &node,T key){

        // int pos = -1;
        for(int i = 0; i < node.count; i++)
            if (key > node.entries[i]){
               return i;
            }
        return node.count;
   }



    int  GetLeaftToInsert(T skey,Node<T,ORDER> &node,Node<T,ORDER> &parent, int &pos_in_parent){

            if(!node.is_leaf){
                int pos=0;
                for (int i = 0; i < node.count; i++){
                    if (skey > node.entries[i])
                        pos++;
                    else break;
                }
                long address = node.children[pos];
                parent=node;
                pos_in_parent = pos;
                node = readNode(address);
                return GetLeaftToInsert(skey,node, parent, pos_in_parent);
            }else{
               int pos = -1;
                for(int i = 0; i < node.count; i++)
                    if (skey > node.entries[i]){
                        pos = i;
                        return pos;
                    }
                return pos;
                // long address = node->children[pos];
                // Record record = read_page(datafile, address);
            }


    }




    // Record searchRec(T skey, Node* node){
    //     if(!node->is_leaf){
    //         int pos = 0;
    //         for (int i = 0; i < node; i++){
    //             if (skey >= node->entries[i])
    //                 pos++;
    //             else break;
    //         }
    //         long address = node->children[pos];
    //         node = read_node(file, address);
    //         searchRec(skey, node);
    //     }
    //     else {
    //         int pos = -1;
    //         for(int i = 0; i < node -> count; i++)
    //             if (skey == node->entries[i]){
    //                 pos = i;
    //                 break;
    //             }
    //         if (pos == -1) return NULL;

    //         long address = node->children[pos];
    //         Record record = read_page(datafile, address);
    //         return record; 
    //     }
    // }


   void WriteNode(long pos,Node<T,ORDER> _reg){
        fstream outFile;
        outFile.open(indexfile,ios::in| ios::out| ios::binary | std::ofstream::app);
        if(outFile.is_open()){
            outFile.seekg(pos);
            outFile.write((char* )&_reg, sizeof(Node<T,ORDER>));
            outFile.close();
        }
   }

   Node<T,ORDER> readNode(long pos){
        ifstream outFile;
        Node<T,ORDER> obj;
        outFile.open(indexfile,ios::in| ios::out| ios::binary);
        if (outFile.is_open()) {
            outFile.seekg(pos);
            outFile.read((char *) &obj, sizeof(Node<T,ORDER>));
            outFile.close();
        }
        return obj;
    }



    Registro ReadReg(long pos){
        ifstream outFile;
        Registro obj;
        outFile.open(datafile,ios::in| ios::out| ios::binary);
        if (outFile.is_open()) {
            outFile.seekg(pos * sizeof(Registro));
            outFile.read((char *) &obj, sizeof(Registro));
            outFile.close();
        }
        return obj;
    }

    long WriteReg(long pos, Registro _reg){
        fstream outFile;
        long _pos;
        outFile.open(indexfile,ios::in| ios::out| ios::binary | std::ofstream::app);
        if(outFile.is_open()){
            outFile.seekg((pos) * sizeof(Registro));
            outFile.write((char* )&_reg, sizeof(Registro));
            _pos=outFile.tellg()- sizeof(Registro);
            outFile.close();
        }
        return _pos;
    }

    long  HeadAddReg(Registro _reg){
        fstream outFile;
        outFile.open(datafile,ios::in| ios::out| ios::binary| std::ofstream::app);
        long _pos;
        if(outFile.is_open()){
            outFile.seekg(0,ios::end);
            outFile.write((char* )&_reg, sizeof(Registro));
            _pos=outFile.tellg()- sizeof(Registro);
            outFile.close();
        }
        return _pos;
    }

     

};





#endif