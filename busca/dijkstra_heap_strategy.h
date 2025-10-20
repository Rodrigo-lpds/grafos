#ifndef DIJKSTRA_HEAP_STRATEGY_H
#define DIJKSTRA_HEAP_STRATEGY_H

#include "dijkstra_strategy.h"
#include <queue>
#include <limits>

/**
 * Estratégia de Dijkstra usando heap (priority_queue).
 * Complexidade: O((V + E) log V) - adequado para grafos esparsos.
 * Aplica o padrão Strategy e princípios SOLID.
 */
class DijkstraHeapStrategy : public IDijkstraStrategy {
private:
    // Priority queue: pair<distancia, vertice> - menor distância tem prioridade
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    vector<double> distancias;
    int n;
    
public:
    void inicializar(int numVertices) override {
        n = numVertices;
        distancias.assign(n, numeric_limits<double>::infinity());
        // Limpa a priority queue
        while (!pq.empty()) {
            pq.pop();
        }
    }
    
    void inserirOuAtualizar(int vertice, double distancia) override {
        distancias[vertice] = distancia;
        pq.push({distancia, vertice});
    }
    
    int extrairMinimo() override {
        while (!pq.empty()) {
            auto [distAtual, u] = pq.top();
            pq.pop();
            
            // Se a distância no heap é igual à atual, este é o vértice correto
            if (distAtual == distancias[u]) {
                return u;
            }
            // Caso contrário, esta é uma entrada obsoleta, continua buscando
        }
        
        return -1; // Heap vazio ou só com entradas obsoletas
    }
    
    bool vazio() const override {
        return pq.empty();
    }
    
    void marcarVisitado(int vertice) override {
        // Na implementação com heap, não precisamos marcar explicitamente como visitado
        // A lógica é tratada pela comparação de distâncias no extrairMinimo()
        (void)vertice; // Suprime aviso de parâmetro não usado
    }
};

#endif