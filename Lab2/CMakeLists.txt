cmake_minimum_required(VERSION 3.17)
project(ACS_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(ACS_2 main.cpp task1/mutex_executor.h task1/blocking_executor.h task1/atomic_executor.h task2/queue.h task2/mutex_queue.h test_b.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ACS_2 Threads::Threads)