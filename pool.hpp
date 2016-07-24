template<std::size_t MAX_OBJ_SIZE>
inline
static void process(pool<MAX_OBJ_SIZE> *mypool)
{
    bool expired = false;
    while(mypool->m_runProcess){
        mu.lock();
        for(auto it = mypool->m_memoryMap.begin(); it != mypool->m_memoryMap.end(); it++)
        {
            auto obj = it->first;
            // check the queues and yield
            if(obj->expired())
            {
                expired = true;
                mypool->m_memoryMap.erase(obj);
                mypool->m_freeMemory.push(obj->m_buf);
                obj->m_buf[0] = 1;
                delete obj;
                mypool->m_freeCount++;
#ifdef LOGGER
                std::cout << "reusing the memory" << std::endl;
#endif
            }
        }
        mu.unlock();
        std::this_thread::yield();
        mu.lock();
        if((mypool->m_head == NULL) && (!mypool->m_freeMemory.empty()) && (mypool->m_cb))
        {
            //auto buf = mypool->m_freeMemory.front();
            mypool->m_freeMemory.pop();
            mypool->m_memoryMap.insert({mypool->m_cb,memory::memType::ASSIGNED});
            mypool->m_freeCount--;
            mypool->m_head = mypool->m_freeMemory.front();
            mypool->m_cb = NULL;
        }
        //head.unlock();
        //std::this_thread::yield();
        
        if(mypool->m_freeCount < mypool->m_minCapacity)
        {
            if(expired)
            {
                mu.unlock();
                break;
            }
            while(mypool->m_freeCount != mypool->m_maxCapacity)
            {
                char* buffer = new char[MAX_OBJ_SIZE];
                mypool->m_freeMemory.push(buffer);
                buffer[0] = 0;
                mypool->m_freeCount++;
            }
#ifdef LOGGER
            std::cout << "expanding the memory" << std::endl;
#endif
        }
        mu.unlock();
        std::this_thread::yield();
    }
    mu.lock();
    for(auto it = mypool->m_memoryMap.begin(); it != mypool->m_memoryMap.end(); it++){
        auto obj = it->first;
        while(!obj->expired());
        delete obj;
    }
    while (!mypool->m_freeMemory.empty())
    {
        auto buf = mypool->m_freeMemory.front();
        mypool->m_freeMemory.pop();
        try{
            //if(buf && buf[0] != 1) delete buf;
        }
        catch(...){}
    }
    mu.unlock();
    
}


template<std::size_t MAX_OBJ_SIZE>
inline
pool<MAX_OBJ_SIZE>::pool (std::size_t maxCapacity, std::size_t minCapacity)
{
    m_maxCapacity = maxCapacity;
    m_minCapacity = minCapacity;
    
    // Initialize databases
    
    for(m_freeCount = 0; m_freeCount < m_maxCapacity; m_freeCount++)
    {
        // Create buffer
        char* buffer = new char[MAX_OBJ_SIZE];
        //m_memoryMap.insert({buffer,memory::memType::UNASSIGNED});
        m_freeMemory.push(buffer);
    }
    // Create a thread which processes the expired memory.
    m_runProcess = true;
    m_manager = new std::thread(process<MAX_OBJ_SIZE>,this);
}

template<std::size_t MAX_OBJ_SIZE>
inline
pool<MAX_OBJ_SIZE>::~pool()
{
    // wait for
    m_runProcess = false;
    m_manager->join();
    
    // All the constructors must have been destructed by this point. Hence release the buffers
    
    // release from objectlist
    
    /*mu.lock();
    while (!m_freeMemory.empty())
    {
        auto buf = m_freeMemory.front();
        m_freeMemory.pop();
        try{
            if(buf) delete buf;
        }
        catch(...){}
    }
    mu.unlock();
    
    for(auto it = m_memoryMap.begin(); it != m_memoryMap.end(); it++){
        auto buf = it->first;
        delete buf;
    }*/
    

}


template<std::size_t MAX_OBJ_SIZE>
template<typename OBJECT, typename ...ARGS>
inline
void pool<MAX_OBJ_SIZE>::alloc (std::shared_ptr<OBJECT> & object, ARGS ...args)
{
    mu.lock();
    if(m_head){
        m_cb = new container<OBJECT,MAX_OBJ_SIZE,ARGS...>(object, args..., m_head);
        m_head = NULL;
        mu.unlock();
    }
    else
    {
        mu.unlock();
        std::shared_ptr<OBJECT> obj((new OBJECT(args...)));
        object = obj;
    }
}
/*{
    mu.lock();
    if (!m_freeMemory.empty())
    {
        auto buf = m_freeMemory.front();
        m_freeMemory.pop();
        containerBase<MAX_OBJ_SIZE> * cb = new container<OBJECT,MAX_OBJ_SIZE,ARGS...>(object, args..., buf);
        m_memoryMap.insert({cb,memory::memType::ASSIGNED});
        //last_obj = cb;
        m_freeCount--;
        mu.unlock();
    }
    else
    {
        mu.unlock();
        std::shared_ptr<OBJECT> obj((new OBJECT(args...)));
        object = obj;
    }
}*/


