#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <functional>

using namespace std;

class ThreadPool{
public:
    ThreadPool(int num):stop(false){
        for(int i = 0; i < num; ++i){
            threads.emplace_back(
                [this]{
                    while(true){
                        unique_lock<mutex> lock(mtx);
                        cv.wait(lock, [this]{
                            return !tasks.empty() || stop;
                        });
                        if(stop && tasks.empty()){
                            return;
                        }
                        function<void()> task(move(tasks.front()));
                        tasks.pop();
                        lock.unlock();
                        task();
                    }
                }
            );
        }
    }

    ~ThreadPool(){
        {
            unique_lock<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for(auto& t : threads){
            t.join();
        }
    }

    template<class F, class... Args>
    void enqueue(F&& f, Args&&... args){
        function<void()> task = bind(forward<F>(f), forward<Args>(args)...);
        {
            unique_lock<mutex> lock(mtx);
            tasks.template emplace(move(task));
        }
        cv.notify_one();
    }
private:
    vector<thread> threads;
    queue<function<void()> > tasks;
    mutex mtx;
    condition_variable cv;
    bool stop;
};

#endif //THREADPOOL_H