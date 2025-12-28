#pragma once
#include <algorithm>
#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <mutex>

// Modified thread pool custom designed for game engine

class RenderPool {
public:
    RenderPool(size_t threads = std::thread::hardware_concurrency())
        : stop(false)
    {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                workerLoop();
                });
        }
    }

    ~RenderPool() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            stop = true;
        }
        cv.notify_all();
        for (auto& t : workers) t.join();
    }

    // Submit N jobs for this frame
    void beginFrame(int jobCount) {
        jobsRemaining.store(jobCount, std::memory_order_release);
    }

    // Enqueue a job (no future, no allocation)
    void enqueue(std::function<void()> job) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            jobs.emplace_back(std::move(job));
        }
        cv.notify_one();
    }

    // Wait until all jobs finish
    void waitFrame() {
        std::unique_lock<std::mutex> lock(frameMutex);
        frameCV.wait(lock, [&] {
            return jobsRemaining.load(std::memory_order_acquire) == 0;
            });
    }

private:
    void workerLoop() {
        for (;;) {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(mutex);
                cv.wait(lock, [&] { return stop || !jobs.empty(); });
                if (stop && jobs.empty()) return;
                job = std::move(jobs.back());
                jobs.pop_back();
            }

            job();

            if (jobsRemaining.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                std::lock_guard<std::mutex> lock(frameMutex);
                frameCV.notify_one();
            }
        }
    }

private:
    std::vector<std::thread> workers;
    std::vector<std::function<void()>> jobs;

    std::mutex mutex;
    std::condition_variable cv;
    bool stop;

    std::atomic<int> jobsRemaining{ 0 };
    std::condition_variable frameCV;
    std::mutex frameMutex;
};
