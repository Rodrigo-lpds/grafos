#include "bellman_ford.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

const double INFINITO = numeric_limits<double>::infinity();

ResultadoBellmanFord BellmanFord::executar(const IGrafoPeso& grafo, int origem) {
    int n = grafo.getNumVertices();
    int t = origem; // origem já é 0-based quando vem do adapter

    // MELHORIA 1: Usar apenas um vetor M[v] ao invés de matriz M[i,v]
    // Custo de memória: O(n) ao invés de O(n²)
    vector<double> M(n, INFINITO);
    vector<int> predecessores(n, -1);

    // M[t] = 0 (origem)
    M[t] = 0;

    // MELHORIA 2: Terminar quando nenhuma distância for atualizada
    // Reduz tempo de execução na prática
    bool houveMudanca = true;
    int iteracao = 0;
    bool cicloNegativoDetectado = false;

    // For i = 1, ..., n-1 (mas pode terminar antes)
    // OTIMIZAÇÃO: Iterar sobre arestas O(m) ao invés de pares O(n²)
    // Complexidade total: O(n·m) ao invés de O(n³)
    while (houveMudanca && iteracao < n - 1) {
        houveMudanca = false;
        iteracao++;

        // Para cada vértice u, relaxar todas as arestas saindo de u
        for (int u = 0; u < n; u++) {
            if (M[u] == INFINITO) continue; // Otimização: pular vértices não alcançados

            // Para cada vizinho v de u (aresta u -> v)
            for (const auto& [v, peso] : grafo.getVizinhosComPeso(u)) {
                double novaDistancia = M[u] + peso;

                // Relaxamento: M[v] = min(M[v], M[u] + peso)
                if (novaDistancia < M[v]) {
                    M[v] = novaDistancia;
                    predecessores[v] = u + 1; // Converter para 1-based
                    houveMudanca = true;
                }
            }
        }
    }

    // Verificar ciclo negativo (executar mais uma iteração - iteração n)
    // Se ainda houver relaxamento, existe ciclo negativo
    if (houveMudanca) {
        for (int u = 0; u < n && !cicloNegativoDetectado; u++) {
            if (M[u] == INFINITO) continue;

            for (const auto& [v, peso] : grafo.getVizinhosComPeso(u)) {
                if (M[u] + peso < M[v]) {
                    cicloNegativoDetectado = true;
                    break;
                }
            }
        }
    }

    // Preparar resultado
    ResultadoBellmanFord resultado;
    resultado.origem = origem + 1; // Converter para 1-based no resultado
    resultado.distancias = M;
    resultado.predecessores = predecessores;
    resultado.temCicloNegativo = cicloNegativoDetectado;

    if (resultado.temCicloNegativo) {
        resultado.cicloNegativo = detectarCicloNegativo(grafo, resultado.distancias, resultado.predecessores);
    }

    return resultado;
}

ResultadoBellmanFord BellmanFord::executar(const ListaAdjacenciaPeso& grafo, int origem) {
    ListaAdjacenciaPesoAdapter adapter(grafo);
    return executar(adapter, origem);
}

vector<int> BellmanFord::detectarCicloNegativo(const IGrafoPeso& grafo,
                                             const vector<double>& distancias,
                                             const vector<int>& predecessores) {
    int n = grafo.getNumVertices();
    vector<int> ciclo;

    // Encontrar um vértice que está em um ciclo negativo
    // OTIMIZAÇÃO: Iterar sobre arestas O(m) ao invés de pares O(n²)
    int verticeNoCiclo = -1;
    for (int u = 0; u < n && verticeNoCiclo == -1; u++) {
        if (distancias[u] == INFINITO) continue;

        for (const auto& [v, peso] : grafo.getVizinhosComPeso(u)) {
            if (distancias[u] + peso < distancias[v]) {
                verticeNoCiclo = v;
                break;
            }
        }
    }

    if (verticeNoCiclo == -1) return ciclo;

    // Reconstruir o ciclo
    int atual = verticeNoCiclo;

    // Ir para trás até encontrar o ciclo
    for (int i = 0; i < n; i++) {
        if (predecessores[atual] != -1) {
            atual = predecessores[atual] - 1; // Converter para 0-based
        }
    }

    // Agora reconstruir o ciclo
    int inicio = atual;
    do {
        ciclo.push_back(atual + 1); // Converter para 1-based
        if (predecessores[atual] != -1) {
            atual = predecessores[atual] - 1;
        }
    } while (atual != inicio && (int)ciclo.size() < n);

    reverse(ciclo.begin(), ciclo.end());
    return ciclo;
}

void BellmanFord::imprimirResultado(const ResultadoBellmanFord& resultado) {
    cout << "=== Resultado do Algoritmo de Bellman-Ford ===\n";
    cout << "Origem: Vértice " << resultado.origem << "\n\n";
    
    if (resultado.temCicloNegativo) {
        cout << "⚠️  ATENÇÃO: Grafo contém ciclo negativo!\n";
        if (!resultado.cicloNegativo.empty()) {
            cout << "Ciclo negativo detectado: ";
            for (size_t i = 0; i < resultado.cicloNegativo.size(); i++) {
                if (i > 0) cout << " -> ";
                cout << resultado.cicloNegativo[i];
            }
            cout << "\n";
        }
        cout << "As distâncias podem não estar corretas.\n\n";
    }
    
    cout << "Distâncias mínimas:\n";
    cout << fixed << setprecision(2);
    
    for (size_t i = 0; i < resultado.distancias.size(); i++) {
        cout << "Vértice " << (i + 1) << ": ";
        if (resultado.distancias[i] == INFINITO) {
            cout << "∞ (não alcançável)";
        } else {
            cout << resultado.distancias[i];
        }
        cout << "\n";
    }
    
    cout << "\nÁrvore de caminhos mínimos (predecessores):\n";
    for (size_t i = 0; i < resultado.predecessores.size(); i++) {
        cout << "Vértice " << (i + 1) << ": ";
        if (resultado.predecessores[i] == -1) {
            cout << "(origem ou não alcançável)";
        } else {
            cout << resultado.predecessores[i];
        }
        cout << "\n";
    }
}

vector<int> BellmanFord::obterCaminho(const ResultadoBellmanFord& resultado, int destino) {
    vector<int> caminho;
    
    if (resultado.temCicloNegativo) {
        // Se há ciclo negativo, o caminho pode não estar bem definido
        return caminho;
    }
    
    int destinoIdx = destino - 1;
    
    // Verificar se o destino é alcançável
    if (resultado.distancias[destinoIdx] == INFINITO) {
        return caminho;
    }
    
    // Reconstruir caminho do destino à origem
    int atual = destino;
    while (atual != -1) {
        caminho.push_back(atual);
        if (atual == resultado.origem) break;
        atual = resultado.predecessores[atual - 1];
    }
    
    // Reverter para obter caminho da origem ao destino
    reverse(caminho.begin(), caminho.end());
    
    return caminho;
}