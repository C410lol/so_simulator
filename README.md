# SO Simulator — Operating System Scheduling Simulator

A graphical application for **simulating task scheduling algorithms in operating systems**, with a focus on algorithms used in real-time systems. The simulator allows users to configure tasks and CPUs, choose a scheduling algorithm, and follow the simulation through its interface.

The project was developed in C++20 and uses Raylib and Dear ImGui for its visual layer.

## ⚙️ Scheduling algorithms

The simulator implements two algorithms:

- **Rate Monotonic (RM):** fixed-priority scheduling, where task priority is determined by its period.
- **Earliest Deadline First (EDF):** dynamic scheduling that prioritizes the task instance with the closest deadline.

The configuration allows users to define the number of CPUs and the quantum used in the simulation. Each task has an identifier, color, arrival time, execution duration, period, and deadline.

## ✨ Features

- Load configurations from TXT files.
- Validate global parameters and task data before starting the simulation.
- Choose between the RM and EDF algorithms.
- Configure multiple CPUs.
- Step through the simulation or run it until completion.
- Maintain a state history, allowing previous simulation states to be revisited.
- Graphical interface for interacting with and monitoring the system.

## 📝 Configuration file format

The configuration file uses fields separated by semicolons (`;`).

The first line defines the algorithm, quantum, and number of CPUs:

```text
ALGORITHM;QUANTUM;NUMBER_OF_CPUS
```

Each following line represents a task:

```text
ID;HEX_COLOR;ARRIVAL_TIME;DURATION;PERIOD;DEADLINE
```

Example:

```text
EDF;2;2
1;F94144;0;1;5;5
2;F3722C;1;2;7;7
3;F9C74F;0;1;8;8
```

The algorithm accepts the values `RM` and `EDF`. The color is provided in hexadecimal, without requiring a `#` prefix. The numeric values for quantum, number of CPUs, duration, period, and deadline must comply with the project's validation rules.

Example files are available in the `config-files/` directory.

## 🧰 Technologies

- **C++20**
- **CMake 3.21+**
- **Raylib 5.5**
- **Dear ImGui**
- **rlImGui**
- **tinyfiledialogs**

The visual dependencies are declared in CMake through FetchContent, which downloads the configured versions when the project is generated.

## 🗂️ Project structure

The application's source code is located in the `so_simulator/` directory:

- `include/core`: representation of tasks, task instances, CPUs, the ready queue, and the simulation.
- `include/scheduler`: scheduler interface and RM and EDF implementations.
- `include/config`: structures, parser, and configuration file validation.
- `include/history`: simulation state history.
- `include/ui`: application and graphical interface.
- `src/`: class implementations.
- `config-files/`: example configuration files.

## ▶️ How to build and run

### Prerequisites

- CMake 3.21 or higher.
- A C++ compiler with C++20 support.
- Git.
- The development tools and libraries required to build Raylib desktop applications on your operating system.

### Build

From the repository root, enter the project directory:

```bash
cd so_simulator
cmake -S . -B build
cmake --build build
```

### Run

After building, run the generated binary from the build directory:

```bash
./build/simulator
```

On the welcome screen, choose a TXT configuration file or use the default configuration option, provided the corresponding file is available in the environment.

---

Academic project for simulating task scheduling, focused on visualizing and comparing scheduling policies.
