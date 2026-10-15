#ifndef SIMULADOR_ESCALONAMENTO_EDFSCHEDULER_H
#define SIMULADOR_ESCALONAMENTO_EDFSCHEDULER_H
#include <queue>

#include "IScheduler.h"
#include "core/TaskInstance.h"


class EDFScheduler: public IScheduler
{
private:
    TaskInstance* selectBestCandidate(TaskInstance* instanceA, TaskInstance* instanceB, bool& wasRandom) override;
public:
    EDFScheduler();
};


#endif //SIMULADOR_ESCALONAMENTO_EDFSCHEDULER_H