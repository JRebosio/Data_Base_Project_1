#ifndef TESTER_H
#define TESTER_H



#define NUMBER_OF_TESTS 5

#include "../lib.h"
#include "../mocker/mocker.h"
#include "../seqfile/seqfile.h"
#include "../bplustree/btree.h"


class Tester{

    template<typename T>
    static void TestSeqFile(vector<string> _data,string data, string data_aux,bool vervose);

public:
    static void execute(bool vervose); 

};





#endif