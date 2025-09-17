# 📊 Biblioteca de Algoritmos em Grafos

Uma implementação completa em C++ para análise e manipulação de grafos, oferecendo diferentes representações (matriz e lista de adjacência) e algoritmos clássicos de busca e análise.

## 🎯 Funcionalidades

### 📋 Representações de Grafos
- **Matriz de Adjacência**: Representação eficiente para grafos densos
- **Lista de Adjacência**: Representação otimizada para grafos esparsos
- **Conversor**: Transforma arquivos de texto em ambas as representações

### 🔍 Algoritmos de Busca
- **BFS (Busca em Largura)**: Encontra o menor caminho em grafos não-ponderados
- **DFS (Busca em Profundidade)**: Explora grafos de forma recursiva
- **Comparação de Desempenho**: Análise comparativa entre matriz e lista

### 📈 Análises Avançadas
- **Distâncias**: Cálculo de distâncias entre vértices
- **Diâmetro**: Encontra a maior distância entre qualquer par de vértices
- **Componentes Conexas**: Identifica grupos de vértices conectados
- **Estatísticas Completas**: Relatórios detalhados sobre propriedades do grafo

### ⚡ Ferramentas de Performance
- **Teste de Desempenho**: Comparação de tempo e memória entre representações
- **Análise Consolidada**: Relatórios comparativos para múltiplos grafos

## 🛠️ Compilação

### Pré-requisitos
- **Compilador**: g++ com suporte a C++17
- **Sistema**: Linux/Unix (testado em distribuições baseadas em Debian)

### Compilar todos os programas
```bash
make all
```

### Compilar programas específicos
```bash
# Algoritmos de busca
make bfs_demo          # Demo do BFS
make dfs_demo          # Demo do DFS
make busca_demo        # Comparação BFS vs DFS

# Análises de grafos
make distancias_demo   # Análise de distâncias
make diametro_demo     # Cálculo do diâmetro
make componentes_demo  # Componentes conexas
make estatisticas_demo # Estatísticas completas

# Ferramentas auxiliares
make conversor         # Conversor de representações
make teste_desempenho  # Análise de performance
```

## 🚀 Como Usar

### 1. Formato dos Arquivos de Entrada
Os grafos devem estar em formato de texto simples:
```
<número_de_vértices>
<vértice1> <vértice2>
<vértice1> <vértice3>
...
```

**Exemplo** (`grafo_exemplo.txt`):
```
5
1 2
2 3
3 4
1 4
4 5
```

### 2. Executando os Algoritmos

#### Busca em Largura (BFS)
```bash
# Usando matriz de adjacência
./bfs_demo grafos/grafo_1.txt matriz 1

# Usando lista de adjacência
./bfs_demo grafos/grafo_1.txt lista 1

# Comparando ambas as representações
./bfs_demo grafos/grafo_1.txt ambos 1
```

#### Busca em Profundidade (DFS)
```bash
# Sintaxe similar ao BFS
./dfs_demo grafos/grafo_1.txt ambos 1
```

#### Comparação Completa de Algoritmos
```bash
# Compara BFS e DFS usando ambas as representações
./busca_demo grafos/grafo_1.txt ambos ambos 1
```

#### Análise de Distâncias
```bash
# Calcula distâncias entre todos os pares de vértices
./distancias_demo grafos/grafo_1.txt ambos
```

#### Cálculo do Diâmetro
```bash
# Encontra a maior distância no grafo
./diametro_demo grafos/grafo_1.txt ambos
```

#### Componentes Conexas
```bash
# Identifica componentes conexas do grafo
./componentes_demo grafos/grafo_1.txt ambos

# Com consulta de conectividade entre vértices específicos
./componentes_demo grafos/grafo_1.txt ambos 5 8
```

#### Estatísticas Completas
```bash
# Gera relatório completo do grafo
./estatisticas_demo grafos/grafo_1.txt
```

#### Análise de Performance
```bash
# Testa um único grafo
./teste_desempenho grafos/grafo_1.txt

# Compara múltiplos grafos
./teste_desempenho grafos/grafo_1.txt grafos/grafo_2.txt grafos/grafo_3.txt
```

### 3. Testes Rápidos
```bash
# Testes individuais
make test_bfs         # Teste rápido do BFS
make test_dfs         # Teste rápido do DFS
make test_distancias  # Teste de distâncias
make test_componentes # Teste de componentes
make test_diametro    # Teste de diâmetro
make test_estatisticas # Teste de estatísticas

# Teste completo
make test             # Executa testes básicos
make test_desempenho  # Executa análise de performance
```

## 📁 Estrutura do Projeto

```
grafos/
├── representacao_leitura/     # Leitura e representação de grafos
│   ├── leitor_grafo.*         # Leitura de arquivos
│   ├── matriz_adjacencia.*    # Implementação de matriz
│   ├── lista_adjacencia.*     # Implementação de lista
│   └── conversor_grafo.*      # Conversor entre representações
├── busca/                     # Algoritmos de busca
│   ├── bfs.*                  # Busca em Largura
│   ├── dfs.*                  # Busca em Profundidade
│   └── *_demo.cpp            # Programas de demonstração
├── analise/                   # Algoritmos de análise
│   ├── distancias.*           # Cálculo de distâncias
│   ├── componentes.*          # Componentes conexas
│   ├── estatisticas.*         # Estatísticas gerais
│   └── *_demo.cpp            # Programas de demonstração
├── grafos/                    # Arquivos de teste
│   └── grafo_*.txt           # Grafos de exemplo
├── teste_desempenho.cpp       # Análise de performance
└── Makefile                   # Sistema de compilação
```

## 📊 Exemplos de Saída

### BFS/DFS
```
=== BFS usando Lista de Adjacência ===
Vértice inicial: 1
Ordem de visita: 1 2 4 3 5
Árvore de busca salva em: bfs_lista_grafo_1.txt
Tempo de execução: 0.123ms
```

### Componentes Conexas
```
=== Análise de Componentes Conexas ===
Número de componentes conexas: 2

Componente 1 (5 vértices): 1 2 3 4 5
Componente 2 (3 vértices): 6 7 8

Conectividade entre vértices 1 e 5: SIM (mesma componente)
```

### Teste de Performance
```
=============================================
  ANÁLISE COMPARATIVA DE GRAFOS
=============================================

| Grafo          | Vértices | Arestas | Mem.Matriz | Mem.Lista | BFS.M | BFS.L |
|----------------|----------|---------|------------|-----------|-------|-------|
| grafo_1.txt    | 10000    | 54989   | 381.47 MB  | 1.68 MB   | 45ms  | 12ms  |
```

## 🔧 Limpeza

```bash
# Remove arquivos compilados e saídas temporárias
make clean
```

## 📋 Requisitos do Sistema

- **Compilador**: g++ 7.0+ com suporte a C++17
- **Memória**: Varia conforme o tamanho do grafo
