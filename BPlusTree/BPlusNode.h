
#define NumberKeys 5
#define Size NumberKeys
#define ll long long
// template<typename key>
class BPlusNode{
    
    public:
        ll keys[Size+1];
        ll children[Size+2];
        ll pos;
        // std::string children[Size+2];
        bool isLeaf;
        int count=0;

       // BPlusNode() = default;
        
        BPlusNode(){
            std::fill(keys,keys+Size+1, -1ll);    
            std::fill(children, children+Size+2 , -1ll);
            isLeaf=false;
        }

        BPlusNode(bool _isleaf){
            BPlusNode();
            isLeaf=_isleaf;
        }



    // void escribir_nodo(Node &node){

    // }





};