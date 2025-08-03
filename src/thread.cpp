#include "thread.h"

#include <iostream>

#include "cv.h"
#include "mutex.h"

std::unique_ptr<my_ucontext_t> thread::currentContext;
std::deque<std::unique_ptr<my_ucontext_t>> thread::readyQueue;
std::deque<std::unique_ptr<my_ucontext_t>> thread::finishedQueue;
std::map<size_t, std::deque<std::unique_ptr<my_ucontext_t>>> thread::waitingThreads;
std::deque<bool> thread::threadHasCompleted;

size_t thread::thread_id = 0;

thread::thread(thread_startfunc_t func, void* arg) {
    cpu::interrupt_disable();
    constructor_helper(func, arg);
    cpu::interrupt_enable();
}

// Custom ctor for OS code
// REQUIRES: Interrupts already disabled in the beginning.
thread::thread(thread_startfunc_t func, void* arg, bool os_starter) {
    thread::constructor_helper(func, arg);
}

void thread::constructor_helper(thread_startfunc_t func, void* arg) {
    std::unique_ptr<ucontext_t> ucp = std::make_unique<ucontext_t>();
    std::unique_ptr<my_ucontext_t> ctx = std::make_unique<my_ucontext_t>();

    ctx->id = thread_id;
    this->id = thread_id;
    thread::waitingThreads.emplace(thread_id,
                                   std::deque<std::unique_ptr<my_ucontext_t>>());
    thread::threadHasCompleted.emplace_back(false);

    thread_id++;
    ctx->ucp = std::move(ucp);

    ctx->ucp->uc_stack.ss_sp = new char[STACK_SIZE];  // Stack Pointer

    ctx->ucp->uc_stack.ss_size = STACK_SIZE;  // Stack Size = 262144
    ctx->ucp->uc_stack.ss_flags = 0;
    ctx->ucp->uc_link = nullptr;
    ctx->sp = ctx->ucp->uc_stack.ss_sp;

    makecontext(ctx->ucp.get(), (void (*)())(&(thread::thread_start)), 3, this, func,
                arg);

    readyQueue.push_back(std::move(ctx));
}

thread::~thread() {}

void thread::join() {
    cpu::interrupt_disable();

    if (!thread::threadHasCompleted[this->id]) {
        thread::waitingThreads[this->id].push_back(std::move(currentContext));

        // No runnable threads!
        if (thread::readyQueue.empty()) {
            cpu::interrupt_enable_suspend();
        }

        thread::currentContext = std::move(thread::readyQueue.front());
        thread::readyQueue.pop_front();

        swapcontext(waitingThreads[this->id].back()->ucp.get(),
                    currentContext->ucp.get());
    }
    cpu::interrupt_enable();
}

void thread::yield() {
    cpu::interrupt_disable();

    thread::yield_helper();

    cpu::interrupt_enable();
}

void thread::yield_helper() {
    if (currentContext.get() != nullptr && !readyQueue.empty()) {
        readyQueue.push_back(std::move(currentContext));
        currentContext = std::move(std::move(readyQueue.front()));
        readyQueue.pop_front();
        swapcontext(readyQueue.back()->ucp.get(), currentContext->ucp.get());
    }
}

void thread::thread_start(thread* obj, thread_startfunc_t func, void* arg) {
    cpu::interrupt_enable();

    // Check
    try {
        func(arg);
    } catch (const std::exception& e) {
        throw e;
    }

    cpu::interrupt_disable();

    while (!thread::waitingThreads[currentContext->id].empty()) {
        thread::readyQueue.push_back(
            std::move(thread::waitingThreads[currentContext->id].front()));
        thread::waitingThreads[currentContext->id].pop_front();
    }

    // Erase to save memory, no more needed after clearing!
    thread::waitingThreads.erase(currentContext->id);

    thread::threadHasCompleted[currentContext->id] = true;
    obj->thread_exit();
}

// REQUIRES: Interrupts are disabled in the beginning.
void thread::thread_exit() {
    while (!finishedQueue.empty()) {
        delete[] (char*)(finishedQueue.front()->sp);
        finishedQueue.pop_front();
    }

    if (!readyQueue.empty()) {
        finishedQueue.push_back(std::move(currentContext));
        currentContext = std::move(readyQueue.front());
        readyQueue.pop_front();
        setcontext(currentContext->ucp.get());
    } else {
        cpu::interrupt_enable_suspend();  // End of main program.
    }
}

void thread::timer_interrupt() {
    cpu::interrupt_disable();

    yield_helper();

    cpu::interrupt_enable();
}

thread::thread(thread&& other) {
    // No implementation.
    throw std::runtime_error(
        "\nthread::thread(&&): Invalid Use of Thread Move Constructor");
}

thread& thread::operator=(thread&& other) {
    // No implementation.
    throw std::runtime_error(
        "\nthread::thread(&&): Invalid Use of Thread Move Copy Constructor");
}