/*
 * cv.h -- interface to the CV class
 *
 * You may add new variables and functions to this class.
 *
 * Do not modify any of the given function declarations.
 */

#pragma once

#include "cpu.h"
#include "mutex.h"

class cv {
   public:
    cv();
    ~cv();

    void wait(mutex&);  // wait on this condition variable
    void signal();      // wake up one thread on this condition
                        // variable
    void broadcast();   // wake up all threads on this condition
                        // variable

    /*
     * Disable the copy constructor and copy assignment operator.
     */
    cv(const cv&) = delete;
    cv& operator=(const cv&) = delete;

    /*
     * Move constructor and move assignment operator.  Implementing these is
     * optional in Project 2.
     */
    cv(cv&&);
    cv& operator=(cv&&);

    std::deque<std::unique_ptr<my_ucontext_t>> cvQueue;
};
