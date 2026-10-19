#include "../../include/core/TaskInstance.h"
#include "../../include/IDs/TaskInstanceId.h"
#include "DTOs/TaskInstanceDTO.h"


TaskInstance::TaskInstance(Task* task, int activationTime):
    id(TaskInstanceId::next()), task(task), activationTime(activationTime),
    executedTime(0), currentQuantum(0), state(TaskState::READY),
    deadlineMissed(false)
{
    if (task)
    {
        instanceNumber = task->getActivationCount();
        absoluteDeadline = activationTime + task->getDeadline();
    }
}

TaskInstance::TaskInstance(const TaskInstanceDTO& instanceDTO):
id(instanceDTO.id), activationTime(instanceDTO.activationTime),
task(nullptr), instanceNumber(instanceDTO.instanceNumber), absoluteDeadline(instanceDTO.absoluteDeadline),
executedTime(instanceDTO.executedTime), currentQuantum(instanceDTO.currentQuantum),
state(instanceDTO.state), deadlineMissed(instanceDTO.deadlineMissed)
{ }


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
void TaskInstance::setTask(Task* _task) { task = _task; }
void TaskInstance::incrementExecutedTime() { executedTime++; }
void TaskInstance::incrementCurrentQuantum() { currentQuantum++; }
void TaskInstance::resetCurrentQuantum() { currentQuantum = 0; }
void TaskInstance::setTaskState(TaskState _state) { state = _state; }
void TaskInstance::setDeadlineMissed(bool _deadlineMissed) { deadlineMissed = _deadlineMissed; }
