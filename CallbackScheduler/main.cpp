#include <iostream>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>
#include <queue>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <mutex>

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

class Call{
public:
    std::function<void()> callback;
    TimePoint when;
};

struct CallCmp {
    bool operator()(const Call& a, const Call& b) const {
        return a.when > b.when;
    }
};



class CallbackScheduler
{
public:
    CallbackScheduler()
    {
        threadDoWork = std::thread(&CallbackScheduler::doWork, this);
    }

    ~CallbackScheduler()
    {
        threadDoWork.detach();
    }

    void Schedule(std::function<void()> callback, TimePoint when)
    {
        Call call(callback,when);
        callQ.push(call);
        cv.notify_one();
    }

    bool callEmpty(){
        return callQ.empty();
    }
private:
    std::priority_queue<Call, std::vector<Call>,CallCmp> callQ;
    std::thread threadDoWork;
    std::mutex mtx;
    std::condition_variable cv;

    void doWork(){
        while (true){
            std::unique_lock<std::mutex> ul(mtx);
            cv.wait(ul,[this] {
                return !callQ.empty();
            });

            auto when = callQ.top().when;
            cv.wait_until(ul, when, [this, when] {
                return !callQ.empty() && when < std::chrono::system_clock::now();
            });
            callQ.top().callback();
            callQ.pop();
            ul.unlock();
        }
    }
};



void do_magick(){
    static std::atomic<int> id{0};
    std::cout << " id = " << id++ << "\n";
}


int main() {
    std::mutex m;
    std::unique_lock ul(m);
    std::condition_variable cv;
    CallbackScheduler callbackScheduler;

    auto s = std::chrono::seconds(1);
    callbackScheduler.Schedule(do_magick,std::chrono::system_clock::now() + s);
    s = std::chrono::seconds(19);
    callbackScheduler.Schedule(do_magick,std::chrono::system_clock::now() + s);
    s = std::chrono::seconds(4);
    callbackScheduler.Schedule(do_magick,std::chrono::system_clock::now() + s);
    s = std::chrono::seconds(6);
    callbackScheduler.Schedule(do_magick,std::chrono::system_clock::now() + s);
    s = std::chrono::seconds(2);
    callbackScheduler.Schedule(do_magick,std::chrono::system_clock::now() + s);
    s = std::chrono::seconds(1);
    callbackScheduler.Schedule(do_magick,std::chrono::system_clock::now() + s);

    std::this_thread::sleep_for(std::chrono::seconds (20));
    std::cout<<"qwe";
    return 0;
}
