#include "../../include/core/Task.h"



Task::Task(int id, std::string color, int start, int duration, int period, int deadline):
    id(id), color(color), start(start),
    started(false), duration(duration), period(period),
    deadline(deadline), activationCount(0), finished(false)
{ }


//  GETS
int Task::getId() const { return id; }
std::string Task::getColor() const { return color; }
int Task::getStart() const { return start; }
bool Task::isStarted() const { return started; }
int Task::getDuration() const { return duration; }
int Task::getPeriod() const { return period; }
int Task::getDeadline() const { return deadline; }
int Task::getActivationCount() const { return activationCount; }
bool Task::isFinished() const { return finished; }


//  SETS
void Task::setStarted(bool _started) { started = _started; }
void Task::incrementActivationCount() { activationCount++; }
void Task::setFinished(bool _finished) { finished = _finished; }