#ifndef SIMULADOR_ESCALONAMENTO_TASKSTATE_H
#define SIMULADOR_ESCALONAMENTO_TASKSTATE_H

enum class TaskState
{
   READY,
   RUNNING,
   FINISHED
};

inline std::string getEnumValue(const TaskState state)
{
   switch (state)
   {
      case TaskState::READY:
         return "PRONTA";
      case TaskState::RUNNING:
         return "RODANDO";
      case TaskState::FINISHED:
         return "FINALIZADA";
      default:
         return "";
   }
}

#endif //SIMULADOR_ESCALONAMENTO_TASKSTATE_H
