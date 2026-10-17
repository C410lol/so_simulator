#ifndef SIMULADOR_ESCALONAMENTO_ISCHEDULER_H
#define SIMULADOR_ESCALONAMENTO_ISCHEDULER_H
#include <vector>

#include "core/TaskInstance.h"


class CPU;
class TaskInstance;

class IScheduler
{
protected:
    virtual TaskInstance* selectBestCandidate(TaskInstance* instanceA, TaskInstance* instanceB, bool& wasRandom) = 0;
    TaskInstance* untie(TaskInstance* instanceA, TaskInstance* instanceB, bool& wasRandom);
public:
    virtual ~IScheduler() = default;

    virtual void schedule(
        std::vector<TaskInstance*>& readyQueue,
        std::vector<CPU*>& cpus,
        bool& wasRandom
    );
};


#endif //SIMULADOR_ESCALONAMENTO_ISCHEDULER_H