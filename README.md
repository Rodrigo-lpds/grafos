# 📊 Sistema de Análise de Grafos

Uma implementação completa em C++ para análise e manipulação de grafos, oferecendo diferentes representações (matriz e lista de adjacência) e algoritmos clássicos de busca e análise através de uma **interface interativa amigável**.

## 🎯 Funcionalidades

### 🖥️ Interface Interativa
- **Menu Intuitivo**: Sistema de navegação por console com emojis e formatação clara
- **Seleção Dinâmica**: Escolha automática de grafos disponíveis na pasta `grafos/`
- **Configuração Flexível**: Configure estruturas de dados e algoritmos conforme necessário
- **Resultados Detalhados**: Visualização clara de caminhos, distâncias e estatísticas

### 📋 Representações de Grafos
- **Matriz de Adjacência**: Representação eficiente para grafos densos
- **Lista de Adjacência**: Representação otimizada para grafos esparsos
- **Grafos com Peso**: Suporte completo para grafos ponderados

### 🔍 Algoritmos Implementados
- **BFS (Busca em Largura)**: Encontra caminhos mais curtos em grafos não ponderados
- **DFS (Busca em Profundidade)**: Exploração completa do grafo
- **Dijkstra**: Algoritmo para caminhos mínimos em grafos com peso
  - Implementação com **Vetor** (simples e didática)
  - Implementação com **Heap** (otimizada para grafos grandes)

### 📈 Análises Disponíveis
- **Estatísticas Gerais**: Número de vértices, arestas, graus (mínimo, máximo, médio)
- **Componentes Conexas**: Identifica e analisa grupos de vértices conectados
- **Conectividade**: Verifica se o grafo é conexo
- **Distâncias**: Cálculo de distâncias entre vértices específicos
- **Diâmetro**: Encontra a maior distância no grafo

## 🛠️ Compilação e Execução

### Pré-requisitos
- **Compilador**: g++ com suporte a C++17
- **Sistema**: Linux/Unix (testado em distribuições baseadas em Debian)

### Compilação
```bash
# Compilar o sistema completo
make all

# Compilar apenas o menu interativo
make menu_interativo

# Limpeza dos arquivos compilados
make clean
```

### Execução
```bash
# Executar o menu interativo principal
./menu_interativo
```

## 🚀 Como Usar o Menu Interativo

### 1. Formato dos Arquivos de Entrada

#### Grafos sem peso:
```
<número_de_vértices>
<vértice1> <vértice2>
<vértice1> <vértice3>
...
```

#### Grafos com peso:
```
<número_de_vértices>
<vértice1> <vértice2> <peso>
<vértice1> <vértice3> <peso>
...
```

**Exemplo de grafo sem peso** (`grafo_exemplo.txt`):
```
5
1 2
2 3
3 4
1 4
4 5
```

**Exemplo de grafo com peso** (`grafo_peso_exemplo.txt`):
```
5
1 2 0.5
2 3 1.2
3 4 2.0
1 4 1.8
4 5 0.7
```

### 2. Navegação no Menu

O sistema oferece uma interface intuitiva organizada em duas seções principais:

#### ⚙️ Configuração:
1. **Selecionar grafo**: Escolha entre os grafos disponíveis na pasta `grafos/`
2. **Definir estrutura**: Configure matriz ou lista de adjacência
3. **Configurar Dijkstra**: Escolha entre implementação com vetor ou heap (apenas para grafos com peso)

#### 🔍 Análises:
4. **Executar algoritmos de busca**: BFS, DFS ou Dijkstra
5. **Analisar estatísticas**: Relatório completo do grafo
6. **Calcular distâncias**: Análise de conectividade e diâmetro (grafos sem peso)

### 3. Exemplo de Fluxo de Uso

```
1. Selecionar grafo → grafo_peso_exemplo.txt ✅
2. Definir estrutura → Lista de adjacência ✅
3. Configurar Dijkstra → Implementação com Heap ✅
4. Executar busca → Dijkstra a partir do vértice 1
   → Resultados: distâncias mínimas para todos os vértices
   → Opção de visualizar caminho específico
```

## 📁 Estrutura do Projeto

```
grafos/
├── menu_interativo.cpp           # 🎯 Interface principal do sistema
├── representacao_leitura/        # 📊 Leitura e representação de grafos
│   ├── leitor_grafo.*            # Leitura de arquivos
│   ├── matriz_adjacencia.*       # Implementação de matriz
│   ├── lista_adjacencia.*        # Implementação de lista
│   ├── lista_adjacencia_peso.*   # Lista para grafos com peso
│   └── *_adapter.h              # Adaptadores para interfaces
├── busca/                        # 🔍 Algoritmos de busca
│   ├── bfs.*                     # Busca em Largura
│   ├── dfs.*                     # Busca em Profundidade
│   ├── dijkstra.*                # Algoritmo de Dijkstra
│   └── dijkstra_*_strategy.h     # Estratégias de implementação
├── analise/                      # 📈 Algoritmos de análise
│   ├── distancias.*              # Cálculo de distâncias
│   ├── componentes.*             # Componentes conexas
│   └── estatisticas.*            # Estatísticas gerais
├── grafos/                       # 📂 Arquivos de teste
│   ├── grafo_*.txt              # Grafos sem peso
│   └── grafo_peso_*.txt         # Grafos com peso
└── Makefile                      # 🔧 Sistema de compilação
```

## 📊 Exemplos de Saída

### Menu Principal
```
╔═══════════════════════════════════════════════════════════════╗
║                    SISTEMA DE ANÁLISE DE GRAFOS               ║
╚═══════════════════════════════════════════════════════════════╝

┌─────────────────────────────────────────────────────────────┐
│                    CONFIGURAÇÃO ATUAL                      │
└─────────────────────────────────────────────────────────────┘
🔸 Grafo: ✅ grafo_peso_exemplo.txt (Grafo com peso)
🔸 Estrutura: ✅ lista
🔸 Dijkstra: ✅ heap

⚙️  CONFIGURAÇÃO:
   1. Selecionar grafo
   2. Definir estrutura de dados (matriz/lista)
   3. Configurar Dijkstra (apenas grafos com peso)

🔍 ANÁLISES:
   4. Executar algoritmos de busca
   5. Analisar estatísticas do grafo
   6. Calcular distâncias (grafos sem peso)
```

### Resultados do Dijkstra
```
📊 RESULTADOS DO DIJKSTRA:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Origem: 1

Distâncias mínimas:
  Vértice   1: 0.00
  Vértice   2: 0.50
  Vértice   3: 1.70
  Vértice   4: 1.80
  Vértice   5: 2.50

📍 Caminho de 1 para 5: 1 → 2 → 3 → 5
📏 Distância total: 2.50
```

### Análise de Estatísticas
```
📊 ESTATÍSTICAS DO GRAFO:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
🔸 Vértices: 10000
🔸 Arestas: 54989
🔸 Grau mínimo: 3
🔸 Grau máximo: 15
🔸 Grau médio: 10.99
🔸 Mediana do grau: 11.00
🔸 Componentes conexas: 1
🔸 Grafo conexo: ✅ Sim
```

## 🔧 Desenvolvimento e Testes

### Comandos de Desenvolvimento
```bash
# Testar o menu interativo
make test_menu

# Compilar teste isolado do Dijkstra
make teste_dijkstra

# Executar teste do Dijkstra
./teste_dijkstra
```

### Adicionando Novos Grafos
1. Coloque arquivos `.txt` na pasta `grafos/`
2. Para grafos sem peso: use nomes como `grafo_nome.txt`
3. Para grafos com peso: inclua "peso" no nome, ex: `grafo_peso_nome.txt`
4. O sistema detectará automaticamente o tipo baseado no nome do arquivo

### Estrutura de Classes Principais

- **MenuInterativo**: Interface principal do sistema
- **LeitorGrafo**: Carregamento de arquivos de grafo
- **MatrizAdjacencia/ListaAdjacencia**: Representações de grafos
- **BFS/DFS/Dijkstra**: Algoritmos de busca
- **Estatisticas/Componentes/Distancias**: Ferramentas de análise

## 🎯 Recursos Avançados

### Algoritmo de Dijkstra
- **Estratégia com Vetor**: Simples, ideal para grafos pequenos e fins didáticos
- **Estratégia com Heap**: Otimizada, recomendada para grafos grandes (complexidade O((V+E)logV))

### Detecção Automática de Tipos
- O sistema identifica automaticamente se um grafo tem peso baseado no nome do arquivo
- Suporte para grafos com pesos negativos (detectados pelo algoritmo)

### Interface Adaptativa
- Menus contextuais que se adaptam ao tipo de grafo selecionado
- Validação automática de entrada do usuário
- Mensagens de erro claras e orientações para correção

## 🔧 Limpeza

```bash
# Remove arquivos compilados e temporários
make clean
```

## 📋 Requisitos do Sistema

- **Compilador**: g++ 7.0+ com suporte a C++17
- **Memória**: Varia conforme o tamanho do grafo
