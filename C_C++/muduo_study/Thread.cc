#include "Thread.h"

Thread::Thread(const ThreadFunc& func) : func_(func), autoDelete_(false)
{

}

void Thread::Start()
{
        pthread_create(&threadId_, NULL, ThreadRoutine, this);
}

void Thread::Join()
{
        pthread_join(threadId_, NULL);
}

void* Thread::ThreadRoutine(void *arg)
{
        auto thread = static_cast<Thread *>(arg);
        // cout << "a=" << Thread::a;
        thread->Run();
        // hread::Run();

        if (thread->autoDelete_)
                delete thread;
        
        return NULL;
}

void Thread::SetAutoDelete(bool autoDelete)
{
        autoDelete_ = autoDelete;  
}

void Thread::Run()
{
        func_();
}