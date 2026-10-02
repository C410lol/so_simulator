#ifndef SIMULADOR_ESCALONAMENTO_TASK_H
#define SIMULADOR_ESCALONAMENTO_TASK_H
#include <string>
#include <utility>


class Task
{
private:
    int id;
    std::string color;
    int start;
    int duration;
    int period;
    int deadline;

    int activationCount;
public:
    Task(int id, std::string color, int start, int duration, int period, int deadline);
};


#endif //SIMULADOR_ESCALONAMENTO_TASK_H