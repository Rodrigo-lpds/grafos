#include "dijkstra.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <iomanip>

ResultadoDijkstra Dijkstra::dijkstraVetor(const ListaAdjacenciaPeso& grafo, int origem) {
    int n = grafo.getNumVertices();
    const auto& lista = grafo.getLista();
    
    ResultadoDijkstra resultado;
    resultado.origem = origem;
    resultado.distancias.assign(n, INF);
    resultado.predecessores.assign(n, -1);
    
    vector<bool> visitado(n, false);
    
    // Ajustar para indexação baseada em 0
    int origemIdx = origem - 1;
    resultado.distancias[origemIdx] = 0;
    
    for (int i = 0; i < n; i++) {
        // Encontrar o vértice não visitado com menor distância
        int u = -1;
        for (int v = 0; v < n; v++) {
            if (!visitado[v] && (u == -1 || resultado.distancias[v] < resultado.distancias[u])) {
                u = v;
            }
        }
        
        if (u == -1 || resultado.distancias[u] == INF) {
            break; // Não há mais vértices alcançáveis
        }
        
        visitado[u] = true;
        
        // Relaxar todas as arestas saindo de u
        for (const auto& vizinho : lista[u]) {
            int v = vizinho.first - 1; // Converter para indexação baseada em 0
            double peso = vizinho.second;
            
            if (!visitado[v] && resultado.distancias[u] + peso < resultado.distancias[v]) {
                resultado.distancias[v] = resultado.distancias[u] + peso;
                resultado.predecessores[v] = u + 1; // Manter indexação baseada em 1
            }
        }
    }
    
    return resultado;
}

ResultadoDijkstra Dijkstra::dijkstraHeap(const ListaAdjacenciaPeso& grafo, int origem) {
    int n = grafo.getNumVertices();
    const auto& lista = grafo.getLista();
    
    ResultadoDijkstra resultado;
    resultado.origem = origem;
    resultado.distancias.assign(n, INF);
    resultado.predecessores.assign(n, -1);
    
    // Priority queue: pair<distancia, vertice> - menor distância tem prioridade
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    
    // Ajustar para indexação baseada em 0
    int origemIdx = origem - 1;
    resultado.distancias[origemIdx] = 0;
    pq.push({0, origemIdx});
    
    while (!pq.empty()) {
        auto [distAtual, u] = pq.top();
        pq.pop();
        
        // Se a distância no heap é maior que a já computada, ignorar
        if (distAtual > resultado.distancias[u]) {
            continue;
        }
        
        // Relaxar todas as arestas saindo de u
        for (const auto& vizinho : lista[u]) {
            int v = vizinho.first - 1; // Converter para indexação baseada em 0
            double peso = vizinho.second;
            
            if (resultado.distancias[u] + peso < resultado.distancias[v]) {
                resultado.distancias[v] = resultado.distancias[u] + peso;
                resultado.predecessores[v] = u + 1; // Manter indexação baseada em 1
                pq.push({resultado.distancias[v], v});
            }
        }
    }
    
    return resultado;
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