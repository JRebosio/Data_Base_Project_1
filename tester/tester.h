#ifndef TESTER_H
#define TESTER_H



#define NUMBER_OF_TESTS 1

#include "../lib.h"
#include "../mocker/mocker.h"
#include "../seqfile/seqfile.h"


class Tester{

    template<typename T>
    static void TestSeqFile(vector<string> _data,string data, string data_aux);

public:
    static void execute(); 

};





#endif