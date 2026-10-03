#ifndef SIMULADOR_ESCALONAMENTO_ISCHEDULER_H
#define SIMULADOR_ESCALONAMENTO_ISCHEDULER_H
#include <vector>


class CPU;
class TaskInstance;

class IScheduler
{
public:
    virtual ~IScheduler() = default;

    virtual std::vector<TaskInstance*> schedule(
        const std::vector<TaskInstance*>& tasks,
        const std::vector<CPU*>& cpus
    ) = 0;
};


#endif //SIMULADOR_ESCALONAMENTO_ISCHEDULER_H