#include "core/Simulation.h"

int main(int argc, char* argv[])
{
    Simulation simulation(argv[1]);
    simulation.start();
    return 0;
}
