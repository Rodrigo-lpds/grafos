#ifndef DIJKSTRA_VETOR_STRATEGY_H
#define DIJKSTRA_VETOR_STRATEGY_H

#include "dijkstra_strategy.h"
#include <limits>

using namespace std;

class DijkstraVetorStrategy : public IDijkstraStrategy {
private:
    vector<double> distancias;
    vector<bool> visitado;
    int n;
    int proximoMinimo;
    int verticesVisitados;

    void atualizarProximoMinimo() {
        proximoMinimo = -1;
        double menorDist = numeric_limits<double>::infinity();
        
        for (int v = 0; v < n; v++) {
            if (!visitado[v] && distancias[v] < menorDist) {
                menorDist = distancias[v];
                proximoMinimo = v;
            }
        }
    }

public:
    void inicializar(int numVertices) override {
        n = numVertices;
        distancias.assign(n, numeric_limits<double>::infinity());
        visitado.assign(n, false);
        proximoMinimo = -1;
        verticesVisitados = 0; // Inicializar contador
    }

    void inserirOuAtualizar(int vertice, double distancia) override {
        distancias[vertice] = distancia;
        
        if (proximoMinimo == -1 || 
            (!visitado[vertice] && distancia < distancias[proximoMinimo])) {
            proximoMinimo = vertice;
        }
    }

    int extrairMinimo() override {
        if (proximoMinimo == -1 || visitado[proximoMinimo]) {
            atualizarProximoMinimo();
        }

        if (proximoMinimo != -1) {
            visitado[proximoMinimo] = true;
            verticesVisitados++;
            int resultado = proximoMinimo;
            proximoMinimo = -1;
            return resultado;
        }

        return -1;
    }

    bool vazio() const override {
        return verticesVisitados >= n;
    }

    void marcarVisitado(int vertice) override {
        if (!visitado[vertice]) {
            visitado[vertice] = true;
            verticesVisitados++; 
            if (proximoMinimo == vertice) {
                proximoMinimo = -1;
            }
        }
    }
};

#endif