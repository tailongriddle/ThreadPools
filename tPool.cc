#include "tPool.h"


/***
 * 
 * Constructor 
 * 
*/
tPool::tPool(int numThreads){

    //initialize instance variables
    qLock = new binary_semaphore(1);// work queue lock
    emptyQ = new binary_semaphore(1); // queue is empty
    stop = false; // stop flag

    // resize thread array
    ths.resize(numThreads);

    // create threads
    for(int i = 0; i < numThreads; i++){ // for num of threads
        ths[i] = new thread(&tPool::workerThread, this, i); // create thread
        cout << "Worker " << i << " started" << endl;
    }  

}

/***
 * 
 * Destructor
 * 
*/
tPool::~tPool(){
    stopPool(); // stop the pool

    for (auto th : ths) {
        delete th;
    }

    delete qLock;
    delete emptyQ;
}

/***
 * 
 * Add work to the pool
 * 
*/
void tPool::addWork(function<void(void*)> work, void* parm){
    cout << "-->Adding work" << endl;

    qLock->acquire(); // lock work queue

    workQueue.push(work); // push work to work queue
    parmQueue.push(parm); // push parm to parm queue

    qLock->release(); // release work queue lock

    emptyQ->release(); 
}

/***
 * 
 * Worker thread
 * 
*/
void tPool::workerThread(int id){

    while(!stop){
        emptyQ->acquire(); // wait for work 

        qLock->acquire(); // lock work queue

        if(!workQueue.empty()){ // if there is work in the queue...
            function<void(void*)> work = workQueue.front(); // get work from work queue
            void* parm = parmQueue.front(); // get parm from parm queue

            workQueue.pop(); // pop work from work queue
            parmQueue.pop(); // pop parm from parm queue

            qLock->release(); // release work queue lock

            cout << "Worker " << id << " about to do work" << endl;

            work(parm); // do the work
        } else {
            qLock->release(); // release work queue lock
            emptyQ->release(); 
        }
    }
    cout << "Worker " << id << " stopping" << endl;
}


/***
 * 
 * Stop the pool
 * 
*/
void tPool::stopPool(){
    emptyQ->acquire(); // lock empty queue

    stop = true; // set stop flag

    for (auto& th : ths) {
        if (th->joinable()){} // check if thread is joinable
            th->join(); // wait for thread to finish
        }

        
    for(size_t i = 0; i < ths.size(); i++){ 
        emptyQ->release(); 
    }

}



