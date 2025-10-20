#include "dijkstra.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <iomanip>

const double INF = numeric_limits<double>::infinity();

ResultadoDijkstra Dijkstra::executar(const IGrafoPeso& grafo, int origem, 
                                    unique_ptr<IDijkstraStrategy> strategy) {
    int n = grafo.getNumVertices();
    
    ResultadoDijkstra resultado;
    resultado.origem = origem;
    resultado.distancias.assign(n, INF);
    resultado.predecessores.assign(n, -1);
    
    // Inicializar estratégia
    strategy->inicializar(n);
    
    // Ajustar para indexação baseada em 0
    int origemIdx = origem - 1;
    resultado.distancias[origemIdx] = 0;
    strategy->inserirOuAtualizar(origemIdx, 0);
    
    // Algoritmo de Dijkstra usando polimorfismo
    while (!strategy->vazio()) {
        int u = strategy->extrairMinimo();
        
        if (u == -1 || resultado.distancias[u] == INF) {
            break; // Não há mais vértices alcançáveis
        }
        
        // Relaxar todas as arestas saindo de u (usando polimorfismo)
        vector<pair<int, double>> vizinhos = grafo.getVizinhosComPeso(u);
        
        for (const auto& [v, peso] : vizinhos) {
            if (resultado.distancias[u] + peso < resultado.distancias[v]) {
                resultado.distancias[v] = resultado.distancias[u] + peso;
                resultado.predecessores[v] = u + 1; // Manter indexação baseada em 1
                strategy->inserirOuAtualizar(v, resultado.distancias[v]);
            }
        }
    }
    
    return resultado;
}

ResultadoDijkstra Dijkstra::dijkstraVetor(const ListaAdjacenciaPeso& grafo, int origem) {
    ListaAdjacenciaPesoAdapter adapter(grafo);
    auto strategy = make_unique<DijkstraVetorStrategy>();
    return executar(adapter, origem, move(strategy));
}

ResultadoDijkstra Dijkstra::dijkstraHeap(const ListaAdjacenciaPeso& grafo, int origem) {
    ListaAdjacenciaPesoAdapter adapter(grafo);
    auto strategy = make_unique<DijkstraHeapStrategy>();
    return executar(adapter, origem, move(strategy));
}

void Dijkstra::imprimirResultado(const ResultadoDijkstra& resultado) {
    cout << "Distancias minimas a partir do vertice " << resultado.origem << ":\n";
    cout << fixed << setprecision(1);
    
    for (int i = 0; i < resultado.distancias.size(); i++) {
        cout << "Vertice " << (i + 1) << ": ";
        if (resultado.distancias[i] == INF) {
            cout << "INFINITO (nao alcancavel)";
        } else {
            cout << resultado.distancias[i];
        }
        cout << "\n";
    }
    
    cout << "\nArvore de caminhos minimos (predecessores):\n";
    for (int i = 0; i < resultado.predecessores.size(); i++) {
        cout << "Vertice " << (i + 1) << ": ";
        if (resultado.predecessores[i] == -1) {
            if (i + 1 == resultado.origem) {
                cout << "(origem)";
            } else {
                cout << "(nao alcancavel)";
            }
        } else {
            cout << resultado.predecessores[i];
        }
        cout << "\n";
    }
}

vector<int> Dijkstra::obterCaminho(const ResultadoDijkstra& resultado, int destino) {
    vector<int> caminho;
    int atual = destino;
    
    // Se o destino não é alcançável
    if (resultado.distancias[destino - 1] == INF) {
        return caminho;
    }
    
    // Reconstruir caminho usando predecessores
    while (atual != -1) {
        caminho.push_back(atual);
        if (atual == resultado.origem) {
            break;
        }
        atual = resultado.predecessores[atual - 1];
    }
    
    // Inverter para obter caminho da origem ao destino
    reverse(caminho.begin(), caminho.end());
    return caminho;
}