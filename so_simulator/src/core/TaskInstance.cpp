#include "../../include/core/TaskInstance.h"
#include "../../include/IDs/TaskInstanceId.h"


TaskInstance::TaskInstance(Task* task, int activationTime):
    id(TaskInstanceId::next()), task(task), activationTime(activationTime),
    executedTime(0), currentQuantum(0), state(TaskState::READY),
    deadlineMissed(false)
{
    if (task)
        absoluteDeadline = activationTime + task->getDeadline();
}


//  GETS
int TaskInstance::getId() const { return id; }
Task* TaskInstance::getTask() const { return task; }
int TaskInstance::getActivationTime() const { return activationTime; }
int TaskInstance::getAbsoluteDeadline() const { return absoluteDeadline; }
int TaskInstance::getExecutedTime() const { return executedTime; }
int TaskInstance::getCurrentQuantum() const { return currentQuantum; }
TaskState TaskInstance::getTaskState() const { return state; }
bool TaskInstance::isDeadlineMissed() const { return deadlineMissed; }


//  SETS
void TaskInstance::incrementExecutedTime() { executedTime++; }
void TaskInstance::incrementCurrentQuantum() { currentQuantum++; }
void TaskInstance::resetCurrentQuantum() { currentQuantum = 0; }
void TaskInstance::setTaskState(TaskState _state) { state = _state; }
void TaskInstance::setDeadlineMissed(bool _deadlineMissed) { deadlineMissed = _deadlineMissed; }