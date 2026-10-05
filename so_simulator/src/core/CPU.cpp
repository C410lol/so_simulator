#include "../../include/core/CPU.h"
#include "../../include/IDs/CpuId.h"


CPU::CPU(): id(CpuId::next()), active(false), idleTicks(0), currentTask(nullptr) { }


//  GETS
int CPU::getId() const { return id; }
bool CPU::isActive() const { return active; }
int CPU::getIdleTicks() const { return idleTicks; }
TaskInstance* CPU::getTaskInstance() const { return currentTask; }


//  SETS
void CPU::setActive(bool _active) { active = _active; }
void CPU::incrementIdleTicks() { idleTicks++; }
void CPU::setTaskInstance(TaskInstance* _taskInstance) { currentTask = _taskInstance; }
void CPU::removeTaskInstance() { currentTask = nullptr; }