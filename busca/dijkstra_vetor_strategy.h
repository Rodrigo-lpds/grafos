#ifndef DIJKSTRA_VETOR_STRATEGY_H
#define DIJKSTRA_VETOR_STRATEGY_H

#include "dijkstra_strategy.h"
#include <limits>

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