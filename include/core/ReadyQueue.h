#ifndef SIMULADOR_ESCALONAMENTO_READYQUEUE_H
#define SIMULADOR_ESCALONAMENTO_READYQUEUE_H
#include <vector>

#include "TaskInstance.h"


class ReadyQueue
{
private:
    std::vector<TaskInstance*> readyTasks;

public:
    void add(TaskInstance* task);
    void remove(TaskInstance* task);

    std::vector<TaskInstance*> getReadyQueue() const;
};


#endif //SIMULADOR_ESCALONAMENTO_READYQUEUE_H