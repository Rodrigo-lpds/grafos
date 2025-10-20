#ifndef DIJKSTRA_VETOR_STRATEGY_H
#define DIJKSTRA_VETOR_STRATEGY_H

#include "dijkstra_strategy.h"
#include <limits>

/**
 * Estratégia de Dijkstra usando vetor com busca linear.
 * Complexidade: O(V²) - adequado para grafos densos ou pequenos.
 * Aplica o padrão Strategy e princípios SOLID.
 */
class DijkstraVetorStrategy : public IDijkstraStrategy {
private:
    vector<double> distancias;
    vector<bool> visitado;
    int n;
    
public:
    void inicializar(int numVertices) override {
        n = numVertices;
        distancias.assign(n, numeric_limits<double>::infinity());
        visitado.assign(n, false);
    }
    
    void inserirOuAtualizar(int vertice, double distancia) override {
        distancias[vertice] = distancia;
    }
    
    int extrairMinimo() override {
        int u = -1;
        double menorDist = numeric_limits<double>::infinity();
        
        // Busca linear pelo vértice não visitado com menor distância
        for (int v = 0; v < n; v++) {
            if (!visitado[v] && distancias[v] < menorDist) {
                menorDist = distancias[v];
                u = v;
            }
        }
        
        if (u != -1) {
            visitado[u] = true;
        }
        
        return u;
    }
    
    bool vazio() const override {
        // Verifica se ainda há vértices não visitados com distância finita
        for (int v = 0; v < n; v++) {
            if (!visitado[v] && distancias[v] != numeric_limits<double>::infinity()) {
                return false;
            }
        }
        return true;
    }
    
    void marcarVisitado(int vertice) override {
        visitado[vertice] = true;
    }
};

#endif