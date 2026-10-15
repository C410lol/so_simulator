#ifndef SIMULADOR_ESCALONAMENTO_TASK_H
#define SIMULADOR_ESCALONAMENTO_TASK_H
#include <string>


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
public:
    Task(int id, std::string color, int start, int duration, int period, int deadline);
    Task(Task*);

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

    //  SETS
    void incrementActivationCount();
    void setFinished(bool _finished);
};


#endif //SIMULADOR_ESCALONAMENTO_TASK_H