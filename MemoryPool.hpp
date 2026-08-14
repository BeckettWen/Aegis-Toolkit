// this is the memory thread pool
#pragma once

#include <queue>
#include <thread>
#include <functional>
#include <any>

namespace Aegis_ThreadPool {

    enum thread_StatusCode: std::size_t {
        Success = 0,
        Failure = 1,
        Cleanup_Failure = 2
    };


    // the beginning of the thread pool
    class thread_pool {
    private:
        std::queue<std::jthread> threadPool;
        bool status_isShutDown = false;

    public:
        thread_pool(){}
        ~thread_pool(){}

        // create a thread into the thread pool
        template<typename type ,typename... Args>
        std::expected<void, thread_StatusCode> Insert_Into_thread_pool(type& task, Args&&... args) {
            try {
                threadPool.push(std::jthread( std::forward<type>(task), std::forward<Args>(args)...));
            } catch (const std::exception& e) {
                return std::unexpected<thread_StatusCode>(thread_StatusCode::Failure);
            }
        }

        bool isShutDown(){ return status_isShutDown; }

        std::size_t getQueueSize(){ return threadPool.size(); }

        std::expected<void, thread_StatusCode> shut_down() {
            try {
                while (!threadPool.empty()) {
                    threadPool.front().request_stop();
                    threadPool.pop();
                }
            }catch (std::exception& exception) {
                return std::unexpected<thread_StatusCode>(Cleanup_Failure);
            }

            status_isShutDown = true;
            return {};
        }

    };
}