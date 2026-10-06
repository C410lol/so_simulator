#ifndef SO_SIMULATOR_CPUID_H
#define SO_SIMULATOR_CPUID_H


class CpuId {
private:
    inline static int currentId = 0;
public:
    static int next() {
        return currentId++;
    }
};


#endif //SO_SIMULATOR_CPUID_H
