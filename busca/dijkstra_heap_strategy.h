#ifndef DIJKSTRA_HEAP_STRATEGY_H
#define DIJKSTRA_HEAP_STRATEGY_H

#include "dijkstra_strategy.h"
#include <queue>
#include <limits>

class DijkstraHeapStrategy : public IDijkstraStrategy {
private:

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    vector<double> distancias;
    int n;

public:
    void inicializar(int numVertices) override {
        n = numVertices;
        distancias.assign(n, numeric_limits<double>::infinity());

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

            if (distAtual == distancias[u]) {
                return u;
            }

        }

        return -1;
    }

    bool vazio() const override {
        return pq.empty();
    }

    void marcarVisitado(int vertice) override {

        (void)vertice;
    }
};

#endif