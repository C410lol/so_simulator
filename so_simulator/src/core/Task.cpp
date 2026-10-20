#include "../../include/core/Task.h"

#include "DTOs/TaskDTO.h"


Task::Task(int id, std::string color, int start, int duration, int period, int deadline):
    id(id), color(color), start(start),
    duration(duration), period(period),
    deadline(deadline), activationCount(0), finished(false)
{ }

Task::Task(const TaskDTO& taskDTO):
id(taskDTO.id), color(taskDTO.color), start(taskDTO.start),
duration(taskDTO.duration), period(taskDTO.period), deadline(taskDTO.deadline),
activationCount(taskDTO.activationCount), finished(taskDTO.finished)
{ }


//  GETS
int Task::getId() const { return id; }
std::string Task::getColor() const { return color; }
int Task::getStart() const { return start; }
int Task::getDuration() const { return duration; }
int Task::getPeriod() const { return period; }
int Task::getDeadline() const { return deadline; }
int Task::getActivationCount() const { return activationCount; }
bool Task::isFinished() const { return finished; }

std::vector<TaskInstance *> Task::getInstances() { return instances; }


//  Returns true if the task reached the limit of 10 activations
bool Task::activatedLimitOfTimes() const { return activationCount == 10; }


//  SETS
void Task::incrementActivationCount() { activationCount++; }
void Task::setFinished(bool _finished) { finished = _finished; }

void Task::addInstance(TaskInstance *instance)
{
    instances.push_back(instance);
}
