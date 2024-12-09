#ifndef TPOOL_H
#define TPOOL_H
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <semaphore>
#include <vector>
#include <queue>
#include <chrono>
#include <functional>


using namespace std; // use the standard namespace


// defines the tPool class
// instance variables, methods, and constructor
class tPool {
    public:
        //constructor
        tPool(int numThreads);//create thread pool with numThreads threads
        //deconstructor
        ~tPool();
        //addWork
        void addWork(function<void(void*)> work, void* parm);//add work to the pool, producer
        //stopPool
        void stopPool();//stop the pool

    private:
        //instance variables
        //use -> rather than . when accessing the semaphore's methods such as release()
        binary_semaphore* qLock; // work queue lock
        binary_semaphore* emptyQ; // lock if queue is empty
        counting_semaphore<INT_MAX>* workAvailable;
        //thread vector
        vector<thread*> ths; //vector of threads
        //unbounded queue (never block when adding more work to pool)
        queue<function<void(void*)>> workQueue; //queue of work to be done
        queue<void*> parmQueue; //queue of parameters for work to be done

        //workerThread, private helper method
        void workerThread(int id);

        //stop flag
        bool stop;

        int numThreads; // number of threads

};
    
#endif