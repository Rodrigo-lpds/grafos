#ifndef DIJKSTRA_VETOR_STRATEGY_H
#define DIJKSTRA_VETOR_STRATEGY_H

#include "dijkstra_strategy.h"
#include <limits>

class DijkstraVetorStrategy : public IDijkstraStrategy {
private:
    vector<double> distancias;
    vector<bool> visitado;
    int n;
    int proximoMinimo; // Cache do próximo vértice com menor distância

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
    }

    void inserirOuAtualizar(int vertice, double distancia) override {
        distancias[vertice] = distancia;
        
        // Se este vértice tem distância menor que o atual mínimo, atualize
        if (proximoMinimo == -1 || 
            (!visitado[vertice] && distancia < distancias[proximoMinimo])) {
            proximoMinimo = vertice;
        }
    }

    int extrairMinimo() override {
        // Se o cache está inválido ou o vértice já foi visitado, recalcular
        if (proximoMinimo == -1 || visitado[proximoMinimo]) {
            atualizarProximoMinimo();
        }
        
        if (proximoMinimo != -1) {
            visitado[proximoMinimo] = true;
            int resultado = proximoMinimo;
            proximoMinimo = -1; // Invalidar cache para forçar recálculo na próxima chamada
            return resultado;
        }
        
        return -1;
    }

    bool vazio() const override {
        // Versão otimizada: se há um próximo mínimo válido, não está vazio
        if (proximoMinimo != -1 && !visitado[proximoMinimo]) {
            return false;
        }
        
        // Caso contrário, verificar todos os vértices
        for (int v = 0; v < n; v++) {
            if (!visitado[v] && distancias[v] != numeric_limits<double>::infinity()) {
                return false;
            }
        }
        return true;
    }

    void marcarVisitado(int vertice) override {
        visitado[vertice] = true;
        if (proximoMinimo == vertice) {
            proximoMinimo = -1; // Invalidar cache
        }
    }
};

#endif