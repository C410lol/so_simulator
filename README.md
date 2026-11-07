# SO Simulator — Simulador de Escalonamento

Aplicação gráfica para **simular algoritmos de escalonamento de tarefas em sistemas operacionais**, com foco em algoritmos utilizados em sistemas de tempo real. O simulador permite configurar tarefas e CPUs, escolher o algoritmo de escalonamento e acompanhar a execução da simulação pela interface.

O projeto foi desenvolvido em C++20 e utiliza Raylib e Dear ImGui para a camada visual.

## ⚙️ Algoritmos de escalonamento

O simulador implementa dois algoritmos:

- **Rate Monotonic (RM):** escalonamento de prioridade fixa, em que a prioridade das tarefas é determinada por seus períodos.
- **Earliest Deadline First (EDF):** escalonamento dinâmico, priorizando a instância de tarefa com o prazo mais próximo.

A configuração permite definir a quantidade de CPUs e o quantum utilizado na simulação. Cada tarefa possui identificador, cor, instante de chegada, duração, período e prazo.

## ✨ Recursos

- Carregamento de configurações a partir de arquivos TXT.
- Validação dos parâmetros globais e dos dados das tarefas antes de iniciar a simulação.
- Seleção entre os algoritmos RM e EDF.
- Configuração de múltiplas CPUs.
- Avanço da simulação e execução até sua conclusão.
- Histórico de estados, permitindo consultar estados anteriores da simulação.
- Interface gráfica para interação e acompanhamento do sistema.

## 📝 Formato do arquivo de configuração

O arquivo de configuração utiliza campos separados por ponto e vírgula (`;`).

A primeira linha define o algoritmo, o quantum e a quantidade de CPUs:

```text
ALGORITMO;QUANTUM;NUMERO_DE_CPUS
```

Cada linha seguinte representa uma tarefa:

```text
ID;COR_HEXADECIMAL;CHEGADA;DURACAO;PERIODO;PRAZO
```

Exemplo:

```text
EDF;2;2
1;F94144;0;1;5;5
2;F3722C;1;2;7;7
3;F9C74F;0;1;8;8
```

O algoritmo aceita os valores `RM` e `EDF`. A cor é informada em hexadecimal, sem necessidade de prefixo `#`. Os valores numéricos de quantum, quantidade de CPUs, duração, período e prazo devem respeitar as validações do projeto.

Há arquivos de exemplo no diretório `config-files/`.

## 🧰 Tecnologias

- **C++20**
- **CMake 3.21+**
- **Raylib 5.5**
- **Dear ImGui**
- **rlImGui**
- **tinyfiledialogs**

As dependências visuais são declaradas no CMake por meio de FetchContent, que realiza o download das versões configuradas durante a geração do projeto.

## 🗂️ Estrutura do projeto

O código-fonte da aplicação está dentro do diretório `so_simulator/`:

- `include/core`: representação de tarefas, instâncias, CPUs, fila de prontos e simulação.
- `include/scheduler`: interface dos escalonadores e implementações RM e EDF.
- `include/config`: estruturas, parser e validação dos arquivos de configuração.
- `include/history`: histórico de estados da simulação.
- `include/ui`: aplicação e interface gráfica.
- `src/`: implementações das classes.
- `config-files/`: configurações de exemplo.

## ▶️ Como compilar e executar

### Pré-requisitos

- CMake 3.21 ou superior.
- Compilador C++ com suporte a C++20.
- Git.
- Ferramentas e bibliotecas de desenvolvimento necessárias para compilar aplicações desktop com Raylib no seu sistema operacional.

### Build

A partir da raiz deste repositório, entre na pasta do projeto:

```bash
cd so_simulator
cmake -S . -B build
cmake --build build
```

### Execução

Após a compilação, execute o binário gerado na pasta de build:

```bash
./build/simulator
```

Na tela inicial, escolha um arquivo TXT de configuração ou utilize a opção de configuração padrão, caso o arquivo correspondente esteja disponível no ambiente.

---

Projeto acadêmico de simulação de escalonamento de tarefas, com foco na visualização e comparação de políticas de escalonamento.
