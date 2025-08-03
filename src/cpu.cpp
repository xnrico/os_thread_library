#include "cpu.h"

#include "mutex.h"

cpu::cpu(thread_startfunc_t func, void *arg) {
    interrupt_vector_table[TIMER] = thread::timer_interrupt;

    if (func != nullptr) {
        // VERY IMPORTANT!
        // thread ctor should be only for users! we have our own logic, inside another
        // constructor to handle the inside-library functionality!
        thread initial(func, arg, true);
        thread::currentContext = std::move(thread::readyQueue.front());
        thread::readyQueue.pop_front();

        setcontext(thread::currentContext->ucp.get());
    }
}