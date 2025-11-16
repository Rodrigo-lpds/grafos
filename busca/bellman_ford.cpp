#include "bellman_ford.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

const double INFINITO = numeric_limits<double>::infinity();

ResultadoBellmanFord BellmanFord::executar(const IGrafoPeso& grafo, int origem) {
    int n = grafo.getNumVertices();
    int t = origem - 1; // Converter para indexação 0-based
    
    // Array M[0,...,n-1 ; 1,...,n] - usando vector de vectors
    vector<vector<double>> M(n, vector<double>(n, INFINITO));
    vector<vector<int>> predecessores(n, vector<int>(n, -1));
    
    // M[0, v] = ∞ para todo v (já inicializado)
    // M[0, t] = 0
    M[0][t] = 0;
    
    // For i = 1, ..., n-1
    for (int i = 1; i < n; i++) {
        // For v = 0, ..., n-1 (convertido para 0-based)
        for (int v = 0; v < n; v++) {
            // M[i, v] = M[i-1, v]
            M[i][v] = M[i-1][v];
            predecessores[i][v] = predecessores[i-1][v];
            
            // Para cada vizinho w de v (arestas ENTRANTES em v)
            // Precisamos iterar sobre todos os vértices para encontrar quem aponta para v
            for (int w = 0; w < n; w++) {
                auto [existeAresta, peso] = grafo.getAresta(w, v);
                if (existeAresta && M[i-1][w] != INFINITO) {
                    double novaDistancia = M[i-1][w] + peso;
                    // M[i, v] = min(M[i, v], M[i-1, w] + c_wv)
                    if (novaDistancia < M[i][v]) {
                        M[i][v] = novaDistancia;
                        predecessores[i][v] = w + 1; // Converter para 1-based
                    }
                }
            }
        }
    }
    
    // Preparar resultado
    ResultadoBellmanFord resultado;
    resultado.origem = origem;
    resultado.distancias = M[n-1]; // Retorna M[n-1, *]
    resultado.predecessores.resize(n);
    
    // Copiar predecessores da última iteração
    for (int v = 0; v < n; v++) {
        resultado.predecessores[v] = predecessores[n-1][v];
    }
    
    // Verificar ciclo negativo (executar mais uma iteração)
    vector<double> verificacao = M[n-1];
    bool mudou = false;
    
    for (int v = 0; v < n; v++) {
        for (int w = 0; w < n; w++) {
            auto [existeAresta, peso] = grafo.getAresta(w, v);
            if (existeAresta && M[n-1][w] != INFINITO) {
                double novaDistancia = M[n-1][w] + peso;
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