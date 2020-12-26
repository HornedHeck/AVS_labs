#ifndef ACS_2_MUTEX_QUEUE_H
#define ACS_2_MUTEX_QUEUE_H

#include "queue.h"
#include <mutex>
#include <queue>
#include <future>

using std::mutex;
using std::queue;
using std::thread;

class MutexQueue : public Queue {

    queue<uint8_t> items;
    mutex m_write;
    mutex m_read;
    bool is_get_active;

    int8_t read_task() {
        is_get_active = true;
        while (is_get_active) {
            if (!items.empty()) {
                auto res = items.front();
                items.pop();
                return res;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }

public:

    void push(uint8_t data) override {
        m_write.lock();
        items.push(data);
        m_write.unlock();
    }

    bool pop(uint8_t &res) override {
        m_read.lock();
        auto res_f = std::async(std::launch::async, &MutexQueue::read_task, this);
        auto status = res_f.wait_for(std::chrono::milliseconds(1));
        if (status == std::future_status::ready) {
            res = res_f.get();
            m_read.unlock();
            return true;
        } else {
            is_get_active = false;
            m_read.unlock();
            return false;
        }

    }

    size_t size() {
        return items.size();
    }

};

#endif //ACS_2_MUTEX_QUEUE_H
