#pragma once

#include <pthread.h>
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
using namespace std;

class Thread {
public:
        typedef boost::function<void ()> ThreadFunc;
        explicit Thread(const ThreadFunc& func);


        void Start();
        void Join();

        void SetAutoDelete(bool autoDelete);

private:
        static void* ThreadRoutine(void *arg);
        void Run();
        ThreadFunc func_;

        pthread_t threadId_;
        bool autoDelete_;
};
