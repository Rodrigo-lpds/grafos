#include "bellman_ford.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

const double INFINITO = numeric_limits<double>::infinity();

ResultadoBellmanFord BellmanFord::executar(const IGrafoPeso& grafo, int origem) {
    int n = grafo.getNumVertices();
    int t = origem - 1; // Converter para indexação 0-based
    
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
    
    // For i = 1, ..., n-1 (mas pode terminar antes)
    while (houveMudanca && iteracao < n - 1) {
        houveMudanca = false;
        iteracao++;
        
        cout << "  [Otimizado] Iteração " << iteracao << "... ";
        int atualizacoes = 0;
        
        // Para cada vértice v
        for (int v = 0; v < n; v++) {
            double melhorDistancia = M[v];
            int melhorPredecessor = predecessores[v];
            
            // Para cada vértice w que pode levar a v
            for (int w = 0; w < n; w++) {
                auto [existeAresta, peso] = grafo.getAresta(w, v);
                if (existeAresta && M[w] != INFINITO) {
                    double novaDistancia = M[w] + peso;
                    // M[v] = min(M[v], M[w] + c_wv)
                    if (novaDistancia < melhorDistancia) {
                        melhorDistancia = novaDistancia;
                        melhorPredecessor = w + 1; // Converter para 1-based
                    }
                }
            }
            
            // Atualizar apenas se houve melhoria
            if (melhorDistancia < M[v]) {
                M[v] = melhorDistancia;
                predecessores[v] = melhorPredecessor;
                houveMudanca = true;
                atualizacoes++;
            }
        }
        
        cout << atualizacoes << " atualizações\n";
        
        // Se não houve mudanças, algoritmo pode terminar
        if (!houveMudanca) {
            cout << "  [Otimizado] Convergência antecipada na iteração " << iteracao << "!\n";
            break;
        }
    }
    
    // Preparar resultado
    ResultadoBellmanFord resultado;
    resultado.origem = origem;
    resultado.distancias = M;
    resultado.predecessores = predecessores;
    
    // Verificar ciclo negativo (executar mais uma iteração)
    vector<double> verificacao = M;
    bool mudou = false;
    
    for (int v = 0; v < n; v++) {
        for (int w = 0; w < n; w++) {
            auto [existeAresta, peso] = grafo.getAresta(w, v);
            if (existeAresta && M[w] != INFINITO) {
                double novaDistancia = M[w] + peso;
                if (novaDistancia < verificacao[v]) {
                    verificacao[v] = novaDistancia;
                    mudou = true;
                }
            }
        }
    }
    
    resultado.temCicloNegativo = mudou;
    
    if (resultado.temCicloNegativo) {
        resultado.cicloNegativo = detectarCicloNegativo(grafo, resultado.distancias, resultado.predecessores);
    }
    
    return resultado;
}

ResultadoBellmanFord BellmanFord::bellmanFord(const ListaAdjacenciaPeso& grafo, int origem) {
    ListaAdjacenciaPesoAdapter adapter(grafo);
    return executar(adapter, origem);
}

ResultadoBellmanFord BellmanFord::bellmanFordOtimizado(const ListaAdjacenciaPeso& grafo, int origem) {
    ListaAdjacenciaPesoAdapter adapter(grafo);
    return executar(adapter, origem);
}

vector<int> BellmanFord::detectarCicloNegativo(const IGrafoPeso& grafo,
                                             const vector<double>& distancias,
                                             const vector<int>& predecessores) {
    int n = grafo.getNumVertices();
    vector<int> ciclo;
    
    // Encontrar um vértice que está em um ciclo negativo
    int verticeNoCiclo = -1;
    for (int v = 0; v < n; v++) {
        for (int w = 0; w < n; w++) {
            auto [existeAresta, peso] = grafo.getAresta(w, v);
            if (existeAresta && distancias[w] != INFINITO) {
                if (distancias[w] + peso < distancias[v]) {
                    verticeNoCiclo = v;
                    break;
                }
            }
        }
        if (verticeNoCiclo != -1) break;
    }
    
    if (verticeNoCiclo == -1) return ciclo;
    
    // Reconstruir o ciclo
    vector<bool> visitado(n, false);
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
    } while (atual != inicio && ciclo.size() < n);
    
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