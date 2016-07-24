#include <iostream>
#include <memory>
#include "pool.h"
#include <chrono>
#include <unistd.h>
#include <cstdlib>

using namespace std;

mutex mymutex;
static int latency = 0;

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

static void create_object(pool<400000> *mypool)
{
    std::shared_ptr<test> mytest;
    auto begin = std::chrono::high_resolution_clock::now();
    mypool->alloc(mytest);
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    mymutex.lock();
    latency+=diff;
    mymutex.unlock();
}

unsigned int microseconds = 100000;

int main(int argv, char** argc) {
    if(argv < 4)
    {
        std::cout << "insufficient arguments" << std::endl;
        std::cout << "usage: ./binary num_threads maxCapacity minCapacity" << std::endl;
        return 0;
    }
    int num_threads = atoi(argc[1]);
    int maxCapacity = atoi(argc[2]);
    int minCapacity = atoi(argc[3]);
    
    pool<400000> *mypool = new pool<400000>(minCapacity,maxCapacity);
    std::thread *myThreads = new std::thread[num_threads];
    for (int i=0; i<num_threads; i++){
        myThreads[i] = std::thread(create_object,mypool);
    }
    //delete myThreads;
    //sleep(1000);
    for(int i = 0; i<num_threads;i++)
    {
        myThreads[i].join();
    }
    cout << latency << "ns" << endl;
    return 0;
}