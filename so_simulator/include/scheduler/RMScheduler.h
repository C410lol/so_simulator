#ifndef SIMULADOR_ESCALONAMENTO_RMSCHEDULER_H
#define SIMULADOR_ESCALONAMENTO_RMSCHEDULER_H

#include "IScheduler.h"


class RMScheduler: public IScheduler
{
protected:
    TaskInstance* selectBestCandidate(TaskInstance* instanceA, TaskInstance* instanceB, bool& wasRandom) override;
public:
    RMScheduler();
};


#endif //SIMULADOR_ESCALONAMENTO_RMSCHEDULER_H