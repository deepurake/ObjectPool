#pragma once

#include <iostream>
#include <list>
#include <queue>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <thread>

std::mutex mu;
static bool stop_expanding = false;

namespace memory
{
    enum class memType{
        ASSIGNED = 0,
        UNASSIGNED = 1,
    };
};

template<std::size_t MAX_OBJ_SIZE>
class containerBase{
public:
    virtual bool expired()
    {
#ifdef LOGGER
        std::cout << "Should not be called" << std::endl;
#endif
        return false;
    }
    
    char* m_buf;
    
};

template<typename OBJECT,std::size_t MAX_OBJ_SIZE, typename... ARGS>
class container: public containerBase<MAX_OBJ_SIZE>{
public:
    std::weak_ptr<OBJECT> m_object;
    using containerBase<MAX_OBJ_SIZE>::m_buf;
    container(std::shared_ptr<OBJECT> & object,ARGS ...args,char* buf){
        m_buf = buf;
        
        // Some how smart pointers are not working with placement new syntax.
        // Hence we have to use raw pointers with m_buf.
        object = std::shared_ptr<OBJECT>(new(m_buf) OBJECT(args...));
        // maintain a weak pointer
        m_object = object;
    }
    bool expired() override{
        return m_object.expired();
    }
    
};

template<std::size_t MAX_OBJ_SIZE>
class pool
{
public:
    // We maintain object list which we loop through and delete unwanted objects
    std::unordered_map<containerBase<MAX_OBJ_SIZE> * , memory::memType> m_memoryMap;
    // Number of objects currently assigned
    std::size_t m_freeCount;

    // we maintain some pre-allocated buffers to help create object fast.
    std::queue<char*> m_freeMemory;
    
    // Capacities
    std::size_t m_maxCapacity;
    std::size_t m_minCapacity;
    
    volatile bool m_runProcess;
    
    std::thread *m_manager;

    pool (std::size_t maxCapacity, std::size_t minCapacity);

    ~pool();

    // allocate new object
    template<typename OBJECT, typename ...ARGS>
    void alloc (std::shared_ptr<OBJECT> & object, ARGS ...args);

    //void dealloc(std::shared_ptr<OBJECT> & object);
    containerBase<MAX_OBJ_SIZE> *last_obj;
    
    char* m_head = NULL;
    containerBase<MAX_OBJ_SIZE> * m_cb = NULL;
    
private:

    // where we clear dealloc queue and allocate new objects if needed. From Lower priority thread
    //void process(int x);

};

#include "pool.hpp"
