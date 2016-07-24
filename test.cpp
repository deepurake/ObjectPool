#include <iostream>
#include <memory>
#include "pool.h"

using namespace std;

class test{
    int a = 0;
public:
    test(int i)
    {
        cout << "creating object " << i << endl;
    }
    ~test()
    {
        cout << "deleting" << endl;
    }
};

int main() {
    pool<100> mypool(2,3);
    std::shared_ptr<test> mytest;
    mypool.alloc(mytest,0);
    
    std::shared_ptr<test> mytest1;
    mypool.alloc(mytest1,1);
    
    std::shared_ptr<test> mytest2;
    mypool.alloc(mytest2,2);
    std::shared_ptr<test> mytest3;
    mypool.alloc(mytest3,3);
    std::shared_ptr<test> mytest4;
    mypool.alloc(mytest4,4);
    std::shared_ptr<test> mytest5;
    mypool.alloc(mytest5,5);
    std::shared_ptr<test> mytest6;
    mypool.alloc(mytest6,6);
    std::shared_ptr<test> mytest7;
    mypool.alloc(mytest7,7);
    // your code goes here
    return 0;
}