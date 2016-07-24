#include <iostream>
#include <memory>
#include "pool.h"
#include <chrono>
#include <unistd.h>

using namespace std;


class test{
    int a[100000];
public:
    test()
    {
        //cout << "allocate"<<endl;
    }
    ~test()
    {
        //cout << "deallocate"<<endl;
    }
};

unsigned int microseconds = 100000;
int main() {
    pool<400000> mypool(2,10);
    {
        //std::shared_ptr<test> mytest[100];
        
        int diff = 0;
        
        for(int i =0; i<30; i++)
        {
            {
                std::shared_ptr<test> mytest;
                auto begin = std::chrono::high_resolution_clock::now();
                mypool.alloc(mytest);
                auto end = std::chrono::high_resolution_clock::now();
                diff += std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
            }
            usleep(microseconds);
        }
        std::cout << diff << "ns" << std::endl;
    }
    
    {
        std::shared_ptr<test> mytest[100];
        
        int diff = 0;
        
        for(int i =0; i<30; i++)
        {
            auto begin = std::chrono::high_resolution_clock::now();
            std::shared_ptr<test> obj((new test()));
            mytest[i] =  obj;
            auto end = std::chrono::high_resolution_clock::now();
            diff += std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
            usleep(microseconds);
        }
        std::cout << diff << "ns" << std::endl;
        std::cout << sizeof(test) << endl;
    }
    
   
    
    return 0;
}