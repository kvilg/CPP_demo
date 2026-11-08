// Синхронизация grabber/processFrames
//
// Есть N потоков, которые получают кадры с камер, и M потоков, которые
// занимаются декомпрессией полученных кадров.
// N >= 1, M >= 1, M != N (в общем случае).
// Нужно придумать способ синхронизации функций grabber() и processFrames().
//
// В решении нельзя полагаться на равномерность загрузки. Например, какая-то
// камера выдаёт 30 fps 2МП, а другая 15 fps 8МП.

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class IFrame{
public:
    IFrame() {}
    IFrame(std::uint64_t i) { id = i; }
    virtual ~IFrame() {}


    std::uint64_t id = 0;
};

std::shared_ptr<IFrame> getFrame(){  //получаем данные
    static std::atomic<std::uint64_t> nextId {0};
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
    return std::make_shared<IFrame>(nextId++);
}

void decompress(std::shared_ptr<IFrame>){//отправили

}

std::atomic<bool> stopFlag {false};

std::mutex mx;
std::queue<std::shared_ptr<IFrame>> frameQ;
std::condition_variable cv;
// выполняется на пуле потоков N
void grabber()
{
    while (!stopFlag)
    {
        std::shared_ptr<IFrame> frame = getFrame(); // функция блокирующая
        {
            std::lock_guard<std::mutex> lk(mx);
            frameQ.push(std::move(frame));
        }
        cv.notify_one();
    }
}



// выполняется на пуле потоков M
void processFrames()
{
    while (!stopFlag)
    {
        std::shared_ptr<IFrame> frame;
        {
            std::unique_lock<std::mutex> lk(mx);
            cv.wait(lk,[]{return !frameQ.empty();});
            frame = frameQ.front();
            frameQ.pop();
        }
        decompress(frame);

    }
}

int main() {
    const int N = 2; // количество потоков grabber
    const int M = 3; // количество потоков processFrames

    std::vector<std::thread> threads;
    threads.reserve(static_cast<std::size_t>(N + M));

    for (int i = 0; i < N; ++i) {
        threads.emplace_back(grabber);
    }

    for (int i = 0; i < M; ++i) {
        threads.emplace_back(processFrames);
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    stopFlag = true;

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "[main] frameQ size " << frameQ.size() << '\n';
    while (!frameQ.empty()) {
        const auto frame = frameQ.front();
        std::cout << "frame.id = " << frame->id << "\n";
        frameQ.pop();
    }

    return 0;
}
