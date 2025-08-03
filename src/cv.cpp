#include "cv.h"

cv::cv() {}

cv::~cv() {}

void cv::wait(mutex& mtx) {
    cpu::interrupt_disable();

    mtx.unlock_helper();

    //  Wait logic
    if (!thread::readyQueue.empty()) {
        cvQueue.push_back(std::move(thread::currentContext));
        thread::currentContext = std::move(thread::readyQueue.front());
        thread::readyQueue.pop_front();
        thread::currentContext->ucp.get();
        swapcontext(cvQueue.back()->ucp.get(), thread::currentContext->ucp.get());
    } else {
        cpu::interrupt_enable_suspend();
    }

    mtx.lock_helper();

    cpu::interrupt_enable();
}

void cv::signal() {
    cpu::interrupt_disable();

    if (!cvQueue.empty()) {
        thread::readyQueue.push_back(std::move(cvQueue.front()));
        cvQueue.pop_front();
    }

    cpu::interrupt_enable();
}

void cv::broadcast() {
    cpu::interrupt_disable();

    while (!cvQueue.empty()) {
        thread::readyQueue.push_back(std::move(cvQueue.front()));
        cvQueue.pop_front();
    }

    cpu::interrupt_enable();
}

cv::cv(cv&& other) {
    // No implementation
    throw std::runtime_error("\ncv::cv(&&): Invalid Use of CV Move Constructor");
}

cv& cv::operator=(cv&& other) {
    // No implementation
    throw std::runtime_error("\ncv::cv(&&): Invalid Use of CV Move Copy Constructor");
}
