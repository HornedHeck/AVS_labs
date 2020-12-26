#include <iostream>
#include "task1/mutex_executor.h"
#include "task1/atomic_executor.h"
#include "task2/queue.h"
#include "task2/mutex_queue.h"

void run_task1_test(
        BlockingExecutor *executor,
        int thread_count,
        bool is_delayed,
        bool with_check = false
) {
    size_t size = 1024L * 1024L;
    auto src = new int8_t[size]{0};
    auto arg = new int8_t[size]{0};

    auto started = std::chrono::high_resolution_clock::now();
    executor->start_blocking(arg, size, is_delayed, thread_count);
    auto done = std::chrono::high_resolution_clock::now();

    std::cout << "Thread count: "
              << thread_count
              << std::endl
              << "Execution time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count()
              << "ms"
              << std::endl
              << std::endl;

    if (with_check) {
        bool isSuccess = true;
        size_t i;
        for (i = 0; i < size; ++i) {

            if (arg[i] != src[i] + 1) {
                isSuccess = false;
                src[i] = arg[i];
                break;
            }
            src[i] = arg[i];
        }
        if (isSuccess) {
            std::cout << thread_count << " - Successful\n\n";
        } else {
            std::cout << thread_count << " - Fail at " << i << std::endl;
        }
    }
}

void task1() {
    vector<int> thread_counts{4, 8, 16, 32};

    auto mutex_executor = new MutexTasksExecutor();

    std::cout << "Run tests for mutex without delay" << std::endl;
    for (auto count : thread_counts) {
        run_task1_test(mutex_executor, count, false);
    }
    std::cout << "Run tests for mutex with delay" << std::endl;
    for (auto count : thread_counts) {
        run_task1_test(mutex_executor, count, true);
    }

    auto atomic_executor = new AtomicTaskExecutor();
    std::cout << "Run tests for atomic without delay" << std::endl;
    for (auto count : thread_counts) {
        run_task1_test(atomic_executor, count, false);
    }
    std::cout << "Run tests for atomic with delay" << std::endl;
    for (auto count : thread_counts) {
        run_task1_test(atomic_executor, count, true);
    }
}

void producer(size_t task_num, Queue *queue) {
    for (size_t i = 0; i < task_num; ++i) {
        queue->push(1);
    }
}

size_t consumer(Queue *queue) {
    uint8_t data;
    size_t read = 0;
    while (queue->pop(data)) {
        ++read;
    }

    return read;
}

void task2() {

    auto queue = new MutexQueue();

    size_t task_num = 4 * 1024 * 1024l;

    thread p(producer, task_num, queue);
    p.join();
    std::cout << "Producers finished\n";

    auto c1 = std::async(std::launch::async, consumer, queue);
//    auto c2 = std::async(std::launch::async, consumer, queue);


    c1.wait();
//    c2.wait();
    size_t consumed = c1.get() /*+ c2.get()*/;
    std::cout << "Produced: " << task_num << std::endl
              << "Consumed: " << consumed << std::endl;

}


int main() {

//    task1();

    task2();

    return 0;
}

