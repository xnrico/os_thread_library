#include "mutex.h"

mutex::mutex() {
    cpu::interrupt_disable();
    free = true;
    cpu::interrupt_enable();
}

mutex::~mutex() {}

void mutex::lock() {
    cpu::interrupt_disable();

    lock_helper();

    cpu::interrupt_enable();
}

void mutex::lock_helper() {
    if (!this->free) {
        this->waitingQueue.push_back(std::move(thread::currentContext));

        if (!thread::readyQueue.empty()) {
            thread::currentContext = std::move(std::move(thread::readyQueue.front()));
            thread::readyQueue.pop_front();

            swapcontext(waitingQueue.back()->ucp.get(),
                        thread::currentContext->ucp.get());
        } else {
            cpu::interrupt_enable_suspend();  // No runnable threads!
        }
    } else {
        this->free = false;
        this->holder_id = thread::currentContext->id;
    }
}

void mutex::unlock() {
    cpu::interrupt_disable();

    unlock_helper();

    cpu::interrupt_enable();
}

void mutex::unlock_helper() {
    if (this->holder_id != thread::currentContext->id) {
        cpu::interrupt_enable();
        throw std::runtime_error("\nmutex::unlock(): Unlocking an unowned Mutex");
    }

    if (!this->waitingQueue.empty()) {
        // Change the lock owner to this thread!
        this->holder_id = this->waitingQueue.front()->id;
        thread::readyQueue.push_back(std::move(this->waitingQueue.front()));
        this->waitingQueue.pop_front();
    } else {
        // No one is holding!
        this->holder_id = SIZE_T_MAX_VAL;
        this->free = true;
    }
}

mutex::mutex(mutex &&other) {
    // No implementation
    throw std::runtime_error("\nmutex::mutex(&&): Invalid Use of Mutex Move Constructor");
}

mutex &mutex::operator=(mutex &&other) {
    // No implementation
    throw std::runtime_error(
        "\nmutex::mutex(&&):: Invalid Use of Mutex Move Copy Constructor");
}