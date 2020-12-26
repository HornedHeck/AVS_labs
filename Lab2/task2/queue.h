#ifndef ACS_2_QUEUE_H
#define ACS_2_QUEUE_H

#include <thread>

class Queue {

protected:

    void thread_delay() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

public:

    /**
     * Записывает элемент в очередь
     * Если очередь фиксированного размера и заполнена,
     * ожидает освобождения
    */
    virtual void push(uint8_t data) = 0;

    /**
     * Если очередь пуста, ждёт 1мс записи в очередь
     * Если не пуста, помещает первое значение в res, удаляет его и возвращет true
     * Если всё ещё пуста, то возврщает false
    */
    virtual bool pop(uint8_t &res) = 0;


};

#endif //ACS_2_QUEUE_H
