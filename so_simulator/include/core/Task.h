#ifndef SIMULADOR_ESCALONAMENTO_TASK_H
#define SIMULADOR_ESCALONAMENTO_TASK_H
#include <string>
#include <vector>


class TaskInstance;
class TaskDTO;

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
    bool finished;

    std::vector<TaskInstance*>  instances;
public:
    Task(int id, std::string color, int start, int duration, int period, int deadline);
    Task(const TaskDTO& taskDTO);

    //  GETS
    int getId() const;
    std::string getColor() const;
    int getStart() const;
    int getDuration() const;
    int getPeriod() const;
    int getDeadline() const;
    int getActivationCount() const;
    bool activatedLimitOfTimes() const;
    bool isFinished() const;

    std::vector<TaskInstance*> getInstances();

    //  SETS
    void incrementActivationCount();
    void setFinished(bool _finished);

    void addInstance(TaskInstance* instance);
};


#endif //SIMULADOR_ESCALONAMENTO_TASK_H