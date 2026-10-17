#ifndef SIMULADOR_ESCALONAMENTO_EDFSCHEDULER_H
#define SIMULADOR_ESCALONAMENTO_EDFSCHEDULER_H

#include "IScheduler.h"


class EDFScheduler: public IScheduler
{
protected:
    TaskInstance* selectBestCandidate(TaskInstance* instanceA, TaskInstance* instanceB, bool& wasRandom) override;
public:
    EDFScheduler();
};


#endif //SIMULADOR_ESCALONAMENTO_EDFSCHEDULER_H