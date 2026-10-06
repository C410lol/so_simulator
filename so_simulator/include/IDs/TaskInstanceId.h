#ifndef SO_SIMULATOR_TASKINSTANCEID_H
#define SO_SIMULATOR_TASKINSTANCEID_H

class TaskInstanceId {
private:
    inline static int currentId = 0;
public:
    static int next() {
        return currentId++;
    }
};

#endif //SO_SIMULATOR_TASKINSTANCEID_H
