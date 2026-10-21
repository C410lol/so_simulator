#include "../../include/ui/Application.h"

#include "config/Config.h"
#include "core/TaskState.h"
#include "rlImGui.h"
#include "raylib.h"
#include "tinyfiledialogs.h"

#include <imgui.h>

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_map>

#ifndef DEFAULT_CONFIG_PATH
#define DEFAULT_CONFIG_PATH "config-files/teste1.txt"
#endif

namespace
{
    // Convert task color strings into the packed format used by ImGui.
    ImU32 taskColor(const std::string& value)
    {
        unsigned int rgb = 0x808080;
        std::string hex = value;
        // Accept both RGB forms with and without the leading '#'.
        if (!hex.empty() && hex.front() == '#')
            hex.erase(hex.begin());
        // Keep the fallback gray when the input is not a six-digit color.
        if (hex.size() == 6)
        {
            std::stringstream parser;
            parser << std::hex << hex;
            parser >> rgb;
        }
        return IM_COL32((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff, 255);
    }

    ImVec4 taskColorVec(const std::string& value)
    {
        const ImU32 packed = taskColor(value);
        return ImVec4(((packed >> IM_COL32_R_SHIFT) & 0xff) / 255.0f,
                      ((packed >> IM_COL32_G_SHIFT) & 0xff) / 255.0f,
                      ((packed >> IM_COL32_B_SHIFT) & 0xff) / 255.0f, 1.0f);
    }

    std::string svgColor(const std::string& value)
    {
        if (value.empty()) return "#808080";
        return value.front() == '#' ? value : "#" + value;
    }
}

bool Application::loadConfiguration(const std::string& path)
{
    // Keep the welcome screen active if initialization fails, and retain its error for display.
    if (!simulation.initialize(path))
    {
        configError = simulation.getLastError();
        showConfigError = true;
        return false;
    }

    configPath = path;
    initialized = true;
    showConfigError = false;
    ganttExported = false;
    return true;
}

void Application::start()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "Simulador de Escalonamento");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    // Drive the Raylib frame loop and render the appropriate screen through ImGui.
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{15, 19, 28, 255});
        rlImGuiBegin();
        // Show controls only after a configuration has initialized the simulation.
        if (initialized)
            renderSimulation();
        else
            renderWelcome();
        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
}

void Application::renderWelcome()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(600, 360), ImGuiCond_Always);
    ImGui::Begin("Simulador de SO", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::TextUnformatted("SIMULADOR DE ESCALONAMENTO");
    ImGui::Separator();
    ImGui::TextWrapped("Deseja escolher um arquivo TXT de configuração próprio?");
    ImGui::Spacing();

    // Open a filtered file picker and load the selection when the dialog returns a path.
    if (ImGui::Button("Escolher arquivo TXT", ImVec2(-1, 44)))
    {
        const char* filters[] = {"*.txt"};
        const char* selected = tinyfd_openFileDialog("Selecione a configuração", nullptr, 1, filters,
                                                     "Arquivo de configuração TXT", 0);
        if (selected != nullptr)
            loadConfiguration(selected);
    }
    if (ImGui::Button("Usar configuração padrão", ImVec2(-1, 44)))
        loadConfiguration(DEFAULT_CONFIG_PATH);
    if (ImGui::Button("Sair", ImVec2(-1, 38)))
        CloseWindow();

    // Display initialization errors here so users can choose another configuration.
    if (showConfigError)
    {
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.38f, 0.35f, 1.0f));
        ImGui::TextWrapped("Não foi possível carregar a configuração: %s", configError.c_str());
        ImGui::PopStyleColor();
    }
    ImGui::End();
}

void Application::renderSimulation()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("Simulador", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::Text("SIMULADOR DE ESCALONAMENTO    |    Tick %d", simulation.getCurrentClock());
    ImGui::SameLine();
    ImGui::TextDisabled("Configuração: %s", configPath.c_str());
    ImGui::Separator();

    // Keep status and controls in a fixed-width column beside the resizable Gantt area.
    const float sideWidth = 350.0f;
    ImGui::BeginChild("Painel lateral", ImVec2(sideWidth, -1), ImGuiChildFlags_Borders);
    renderSchedulerPanel();
    ImGui::Spacing();
    renderCpusPanel();
    ImGui::Spacing();
    renderReadyQueuePanel();
    ImGui::Spacing();
    renderTasksPanel();
    ImGui::Spacing();
    renderControls();
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("Painel Gantt", ImVec2(0, -1), ImGuiChildFlags_Borders);
    renderGanttPanel();
    ImGui::EndChild();

    ImGui::End();
}

void Application::renderSchedulerPanel()
{
    // Update the active scheduler immediately when the selection changes.
    if (ImGui::CollapsingHeader("Escalonamento", ImGuiTreeNodeFlags_DefaultOpen))
    {
        int selected = simulation.getSchedulerName() == "RM" ? 0 : 1;
        const char* algorithms[] = {"RM - Rate Monotonic", "EDF - Earliest Deadline First"};
        if (ImGui::Combo("Algoritmo", &selected, algorithms, IM_ARRAYSIZE(algorithms)))
            simulation.setScheduler(selected == 0 ? "RM" : "EDF");

        ImGui::Text("Processadores: %zu", simulation.getCpus().size());
        ImGui::Text("Estado: %s", simulation.isSimulationFinished() ? "Concluído" : "Em execução");
    }
}

void Application::renderCpusPanel()
{
    // Skip the panel contents while the section is collapsed.
    if (!ImGui::CollapsingHeader("CPUs", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    const auto cpus = simulation.getCpus();
    if (cpus.empty())
    {
        ImGui::TextDisabled("Nenhuma CPU disponível");
        return;
    }

    // Render each processor, ignoring null entries and showing its current assignment.
    for (const CPU* cpu : cpus)
    {
        if (cpu == nullptr)
            continue;

        const TaskInstance* instance = cpu->getTaskInstance();
        const bool isExecuting = cpu->isActive() && instance != nullptr && instance->getTask() != nullptr;

        ImGui::PushID(cpu->getId());
        ImGui::Text("CPU %d", cpu->getId());
        ImGui::SameLine(76.0f);
        if (isExecuting)
        {
            ImGui::TextColored(taskColorVec(instance->getTask()->getColor()), "Executando T%d.%d",
                instance->getTask()->getId(), instance->getInstanceNumber());
        }
        else
        {
            ImGui::TextDisabled("Inativa");
        }
        ImGui::Text("Ticks ociosos: %d", cpu->getIdleTicks());
        ImGui::Separator();
        ImGui::PopID();
    }
}

void Application::renderReadyQueuePanel()
{
    // Collapsing the header avoids drawing queue entries when the panel is hidden.
    if (!ImGui::CollapsingHeader("Instâncias na fila de prontos", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    const auto ready = simulation.getReadyQueue();
    if (ready.empty())
        ImGui::TextDisabled("Fila vazia");
    // List each valid ready instance with its deadline and remaining execution time.
    for (const TaskInstance* instance : ready)
    {
        if (instance == nullptr || instance->getTask() == nullptr)
            continue;

        ImGui::PushStyleColor(ImGuiCol_Text, taskColorVec(instance->getTask()->getColor()));
        ImGui::BulletText("T%d.%d  |  prazo %d  |  executado %d/%d",
            instance->getTask()->getId(), instance->getInstanceNumber(),
            instance->getAbsoluteDeadline(), instance->getExecutedTime(),
            instance->getTask()->getDuration());
        ImGui::PopStyleColor();
    }
}

void Application::renderTasksPanel()
{
    // Each task expands into its parameters and manually editable instances.
    if (!ImGui::CollapsingHeader("Tarefas fixas", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    const auto tasks = simulation.getTasks();
    for (Task* task : tasks)
    {
        if (task == nullptr)
            continue;

        const std::string label = "Tarefa T" + std::to_string(task->getId());
        if (ImGui::TreeNode(label.c_str()))
        {
            ImGui::Text("Cor: %s", task->getColor().c_str());
            ImGui::Text("Ingresso: %d   Duração: %d", task->getStart(), task->getDuration());
            ImGui::Text("Período: %d   Prazo: %d", task->getPeriod(), task->getDeadline());
            ImGui::Text("Ativações: %d/10   %s", task->getActivationCount(),
                        task->isFinished() ? "Finalizada" : "Ativa");

            ImGui::SeparatorText("Instâncias e estado manual");
            const char* stateLabels[] = {"Pronta", "Executando", "Finalizada"};
            for (TaskInstance* instance : task->getInstances())
            {
                if (instance == nullptr)
                    continue;

                ImGui::PushID(instance->getId());
                ImGui::Text("T%d.%d", task->getId(), instance->getInstanceNumber());
                ImGui::SameLine();

                int selectedState = static_cast<int>(instance->getTaskState());
                if (ImGui::Combo("##Estado", &selectedState, stateLabels, IM_ARRAYSIZE(stateLabels)))
                {
                    std::string error;
                    // Manual state changes are delegated to Simulation so they enter its history.
                    if (simulation.setInstanceState(instance->getId(), static_cast<TaskState>(selectedState), error))
                    {
                        stateEditMessage = "Estado da instância atualizado e salvo no histórico.";
                        stateEditFailed = false;
                    }
                    else
                    {
                        stateEditMessage = error;
                        stateEditFailed = true;
                    }
                }
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
    }
    if (!stateEditMessage.empty())
    {
        const ImVec4 color = stateEditFailed ? ImVec4(1.0f, 0.38f, 0.35f, 1.0f)
                                              : ImVec4(0.45f, 0.85f, 0.65f, 1.0f);
        ImGui::TextColored(color, "%s", stateEditMessage.c_str());
    }
}

void Application::renderControls()
{
    // Prepare simulation state before enabling the step and run controls.
    simulation.prerun();

    ImGui::SeparatorText("Controles");
    const bool finished = simulation.isSimulationFinished();
    // Only step backward when history contains an earlier simulation state.
    if (ImGui::Button("Voltar", ImVec2(-1, 34)) && simulation.canGoBack())
        simulation.back();
    // Prevent forward execution once the simulation has reached its end.
    ImGui::BeginDisabled(finished);
    if (ImGui::Button("Avançar um tick", ImVec2(-1, 34)))
    {
        simulation.run();
        if (simulation.isSimulationFinished())
        {
            exportGanttSvg();
            ganttExported = true;
        }
    }
    // Advance once to initialize the run, then complete all remaining ticks.
    if (ImGui::Button("Rodar tudo", ImVec2(-1, 34)))
    {
        simulation.run();
        simulation.runAll();
        exportGanttSvg();
        ganttExported = true;
    }
    ImGui::EndDisabled();
    // Allow exporting the current chart before the simulation finishes.
    if (ImGui::Button("Exportar Gantt como SVG", ImVec2(-1, 34)))
    {
        exportGanttSvg();
        ganttExported = true;
    }
    if (ganttExported)
        ImGui::TextWrapped("Imagem salva como gantt-final.svg");
    if (ImGui::Button("Sair", ImVec2(-1, 34)))
        CloseWindow();
}

void Application::renderGanttPanel()
{
    ImGui::TextUnformatted("GRÁFICO DE GANTT");
    ImGui::SameLine();
    ImGui::TextDisabled("Execução por tarefa e processador ao longo dos ticks");
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.65f, 0.7f, 0.8f, 1), "Legenda:");
    ImGui::SameLine(); ImGui::Text("bloco colorido = execução   |   contorno cinza = ocioso   |   ! = prazo perdido");
    ImGui::SameLine(); ImGui::Text("   ◆ chegada   ✓ conclusão");

    // Use saved snapshots so the chart follows the same timeline as back navigation.
    History history = simulation.getHistory();
    const auto& states = history.getStates();
    auto tasks = simulation.getTasks();
    std::sort(tasks.begin(), tasks.end(), [](const Task* a, const Task* b) { return a->getId() > b->getId(); });

    const float labelWidth = 82.0f;
    const float cellWidth = 52.0f;
    const float rowHeight = 34.0f;
    const int tickCount = std::max(1, simulation.getCurrentClock());
    const float contentWidth = labelWidth + tickCount * cellWidth + 40.0f;

    ImGui::BeginChild("Área do Gantt", ImVec2(0, 0), ImGuiChildFlags_Borders,
                      ImGuiWindowFlags_HorizontalScrollbar);
    ImDrawList* draw = ImGui::GetWindowDrawList();
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const float chartTop = origin.y + 12.0f;
    const float chartBottom = chartTop + (tasks.size() + 1) * rowHeight;
    const float chartLeft = origin.x + labelWidth;

    // Draw tick boundaries and labels across the full visible timeline.
    for (int tick = 0; tick <= simulation.getCurrentClock() + 1; ++tick)
    {
        const float x = chartLeft + tick * cellWidth;
        draw->AddLine(ImVec2(x, chartTop), ImVec2(x, chartBottom), IM_COL32(54, 65, 82, 255));
        char label[24];
        std::snprintf(label, sizeof(label), "%d", tick);
        draw->AddText(ImVec2(x + 4, chartBottom + 5), IM_COL32(170, 182, 201, 255), label);
    }
    draw->AddLine(ImVec2(chartLeft, chartBottom),
                  ImVec2(chartLeft + (simulation.getCurrentClock() + 1) * cellWidth, chartBottom),
                  IM_COL32(120, 133, 153, 255));

    draw->AddText(ImVec2(origin.x + 4, chartTop + 8), IM_COL32(170, 182, 201, 255), "Aleatório");
    draw->AddLine(ImVec2(chartLeft, chartTop + rowHeight),
                  ImVec2(chartLeft + (simulation.getCurrentClock() + 1) * cellWidth, chartTop + rowHeight),
                  IM_COL32(39, 47, 61, 255));

    std::unordered_map<int, int> rowByTask;
    // Reserve row zero for random-scheduling markers; task rows start below it.
    for (size_t row = 0; row < tasks.size(); ++row)
    {
        const Task* task = tasks[row];
        rowByTask[task->getId()] = static_cast<int>(row) + 1;
        const float y = chartTop + (row + 1) * rowHeight;
        draw->AddText(ImVec2(origin.x + 4, y + 8), taskColor(task->getColor()),
                      ("T" + std::to_string(task->getId())).c_str());
        draw->AddLine(ImVec2(chartLeft, y + rowHeight), ImVec2(chartLeft + (simulation.getCurrentClock() + 1) * cellWidth, y + rowHeight),
                      IM_COL32(39, 47, 61, 255));
        draw->AddLine(ImVec2(chartLeft, y + rowHeight), ImVec2(chartLeft + (simulation.getCurrentClock() + 1) * cellWidth, y + rowHeight),
                      IM_COL32(39, 47, 61, 255));
    }

    const int lastState = static_cast<int>(states.size()) - 1;
    for (int stateIndex = 0; stateIndex <= lastState; ++stateIndex)
    {
        const SystemState& state = states[stateIndex];
        const int tick = state.getClock();

        // Mark only ticks whose saved system state records a random scheduling decision.
        if (state.getSchedulingWasRandom() && tick >= 0 && tick <= simulation.getCurrentClock())
        {
            const float x = chartLeft + tick * cellWidth;
            draw->AddText(ImVec2(x + 4, chartTop + 9), IM_COL32(255, 205, 94, 255), "Sim");
        }

        std::unordered_map<int, int> taskForInstance;
        for (TaskInstanceDTO instance : state.getInstances())
            taskForInstance[instance.id] = instance.taskId;

        // Place a colored execution block in the row for each active CPU.
        for (CpuDTO cpu : state.getCpus())
        {
            if (!cpu.active || !taskForInstance.contains(cpu.currentInstanceId))
                continue;

            const int taskId = taskForInstance[cpu.currentInstanceId];
            if (!rowByTask.contains(taskId))
                continue;

            const int row = rowByTask[taskId];
            const Task* task = *std::find_if(tasks.begin(), tasks.end(), [taskId](const Task* item) {
                return item->getId() == taskId;
            });

            const ImVec2 min(chartLeft + tick * cellWidth + 2, chartTop + row * rowHeight + 4);
            const ImVec2 max(min.x + cellWidth - 4, min.y + rowHeight - 8);
            draw->AddRectFilled(min, max, taskColor(task->getColor()), 4.0f);
            draw->AddText(ImVec2(min.x + 7, min.y + 5), IM_COL32(17, 22, 31, 255),
                          ("CPU " + std::to_string(cpu.id)).c_str());
        }

        // Add activation, completion, and missed-deadline markers to each task row.
        for (TaskInstanceDTO instance : state.getInstances())
        {
            const auto rowIt = rowByTask.find(instance.taskId);
            if (rowIt == rowByTask.end())
                continue;

            const float x = chartLeft + tick * cellWidth;
            const float y = chartTop + rowIt->second * rowHeight;
            if (instance.activationTime == tick)
                draw->AddTriangleFilled(ImVec2(x + 4, y + 4), ImVec2(x + 12, y + 4), ImVec2(x + 8, y + 12),
                                        IM_COL32(93, 199, 255, 255));
            if (instance.state == TaskState::FINISHED && instance.finishedTime == state.getClock())
                draw->AddText(ImVec2(x + 3, y + 16), IM_COL32(108, 230, 170, 255), "✓");
            if (instance.deadlineMissed && instance.deadlineMissedTime == state.getClock())
                draw->AddText(ImVec2(x + 20, y + 16), IM_COL32(150, 0, 24, 255), "x");
        }
    }

    ImGui::Dummy(ImVec2(contentWidth, chartBottom - origin.y + 24));
    ImGui::EndChild();
}

void Application::exportGanttSvg() const
{
    // Export the current simulation history using the same task and tick layout as the panel.
    const History history = simulation.getHistory();
    const auto& states = history.getStates();
    auto tasks = simulation.getTasks();
    std::sort(tasks.begin(), tasks.end(), [](const Task* a, const Task* b) { return a->getId() > b->getId(); });
    const int ticks = std::max(1, simulation.getCurrentClock());
    constexpr int left = 100, top = 55, cell = 52, row = 38;
    const int width = left + ticks * cell + 30;
    const int chartBottom = top + (static_cast<int>(tasks.size()) + 1) * row;
    const int height = chartBottom + 36;

    // Replace the previous export with a fresh SVG for the current simulation state.
    std::ofstream svg("gantt-final.svg", std::ios::trunc);
    if (!svg) return;
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width << "\" height=\"" << height
        << "\" viewBox=\"0 0 " << width << ' ' << height << "\">\n"
        << "<rect width=\"100%\" height=\"100%\" fill=\"#0f131c\"/>\n"
        << "<text x=\"16\" y=\"28\" fill=\"#edf2fa\" font-family=\"sans-serif\" font-size=\"18\">Gantt - "
        << simulation.getSchedulerName() << "</text>\n";
    svg << "<text x=\"16\" y=\"" << top + 24
        << "\" fill=\"#aab6c9\" font-family=\"sans-serif\" font-size=\"14\">Aleatório</text>\n"
        << "<line x1=\"" << left << "\" y1=\"" << top + row << "\" x2=\""
        << left + (ticks + 1) * cell << "\" y2=\"" << top + row << "\" stroke=\"#272f3d\"/>\n";

    std::unordered_map<int, int> taskRows;
    // Match the panel layout by keeping the random-scheduling row above all tasks.
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        taskRows[tasks[i]->getId()] = static_cast<int>(i) + 1;
        const int y = top + (static_cast<int>(i) + 1) * row;
        svg << "<text x=\"16\" y=\"" << y + 24 << "\" fill=\"" << svgColor(tasks[i]->getColor())
            << "\" font-family=\"sans-serif\" font-size=\"14\">T" << tasks[i]->getId() << "</text>\n";
    }

    // Write shared vertical grid lines and tick labels for the SVG chart.
    for (int tick = 0; tick <= ticks; ++tick)
        svg << "<line x1=\"" << left + tick * cell << "\" y1=\"" << top << "\" x2=\""
            << left + tick * cell << "\" y2=\"" << chartBottom << "\" stroke=\"#364152\"/>\n"
            << "<text x=\"" << left + tick * cell + 4 << "\" y=\"" << chartBottom + 16
            << "\" fill=\"#aab6c9\" font-family=\"sans-serif\" font-size=\"11\">" << tick << "</text>\n";
    svg << "<line x1=\"" << left << "\" y1=\"" << chartBottom << "\" x2=\""
        << left + (ticks + 1) * cell << "\" y2=\"" << chartBottom << "\" stroke=\"#788599\"/>\n";

    for (int stateIndex = 0; stateIndex <= history.getCurrentIndex(); ++stateIndex)
    {
        const SystemState& state = states[stateIndex];
        // For repeated snapshots at one tick, render only the latest saved version.
        if (stateIndex < history.getCurrentIndex() && states[stateIndex + 1].getClock() == state.getClock())
            continue;

        const int tick = state.getClock();
        if (tick < 0 || tick > ticks)
            continue;

        if (state.getSchedulingWasRandom())
            svg << "<text x=\"" << left + tick * cell + 4 << "\" y=\"" << top + 24
                << "\" fill=\"#ffcd5e\" font-family=\"sans-serif\" font-size=\"12\">Sim</text>\n";

        std::unordered_map<int, int> taskForInstance;
        for (TaskInstanceDTO instance : state.getInstances()) taskForInstance[instance.id] = instance.taskId;
        // The final clock value has no execution interval after it.
        if (tick < ticks) for (CpuDTO cpu : state.getCpus())
        {
            if (!cpu.active || !taskForInstance.contains(cpu.currentInstanceId))
                continue;

            const int taskId = taskForInstance[cpu.currentInstanceId];
            const int y = top + taskRows[taskId] * row + 4;
            const auto task = std::find_if(tasks.begin(), tasks.end(), [taskId](const Task* item) { return item->getId() == taskId; });
            if (task == tasks.end())
                continue;

            svg << "<rect x=\"" << left + tick * cell + 2 << "\" y=\"" << y << "\" width=\"" << cell - 4
                << "\" height=\"30\" rx=\"4\" fill=\"" << svgColor((*task)->getColor()) << "\"/>\n"
                << "<text x=\"" << left + tick * cell + 7 << "\" y=\"" << y + 19
                << "\" fill=\"#111722\" font-family=\"sans-serif\" font-size=\"11\">CPU " << cpu.id << "</text>\n";
        }

        for (TaskInstanceDTO instance : state.getInstances())
        {
            const auto rowIt = taskRows.find(instance.taskId);
            if (rowIt == taskRows.end())
                continue;

            const int markerX = left + tick * cell;
            const int markerY = top + rowIt->second * row + 4;
            if (instance.activationTime == tick)
                svg << "<path d=\"M" << markerX + 4 << ' ' << markerY + 2 << " l8 8 -8 8 z\" fill=\"#5dc7ff\"/>\n";
            if (instance.state == TaskState::FINISHED && instance.finishedTime == tick)
                svg << "<text x=\"" << markerX + 3 << "\" y=\"" << markerY + 28
                    << "\" fill=\"#6ce6aa\" font-family=\"sans-serif\" font-size=\"14\">&#10003;</text>\n";
            if (instance.deadlineMissed && instance.deadlineMissedTime == tick)
                svg << "<text x=\"" << markerX + 20 << "\" y=\"" << markerY + 28
                    << "\" fill=\"#960018\" font-family=\"sans-serif\" font-size=\"14\">x</text>\n";
        }
    }
    svg << "</svg>\n";
}
