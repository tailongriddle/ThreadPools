// Compile as: g++ poolTester.cc tPool.cc -std=c++20 -pthread -o poolTester
// Note we want the 2020 standard to use this version of semaphores

#include <iostream>
#include "tPool.h"

typedef struct fooParm {
    int id;
    int waitTime;
} fooParm;

void foo(void* parm) {
    fooParm* p = (fooParm*)parm; // downcast

    // I can't get the screenLock from the tPool class here
    //   so I just put it together into a string and print it once
    std::string output = "  Foo called with id " + std::to_string(p->id) + "\n";
    std::cout << output;
    this_thread::sleep_for(chrono::milliseconds(p->waitTime));
}

int main() {
    // Initialize the random seed
    //srand(time(NULL));  // each run will be different
    srand(0);  // each run will be the same - good for testing


    tPool* tp = new tPool(4);

    // Note the alternate syntax for creating the structure on the heap in C++
    //   with the new (instead of malloc) and the {initializer list}
    //   Be careful using {} for objects - there are subtle issues
    //   so for now we will stick to simple structs for {}
    // Right now I'm having each foo sleep for 0-10 seconds
    for (int i=0; i<20; i++) {
        // Slight pause before adding the next piece of work
        this_thread::sleep_for(chrono::milliseconds(rand()%1000));

        // Ask the thread pool to do the work when it gets a chance
        tp->addWork(foo, new fooParm{i, rand()%10000});
    }

    // We need to wait a bit before stopping the pool to give the threads a chance to finish all work
    // This is for testing until we get the stop to wait for all the work on it to finish before stopping
    //this_thread::sleep_for(chrono::milliseconds(rand()%100000)); // 1000 seconds
    tp->stopPool();  // This is the eventual way to stop the pool
}