#ifndef DIJKSTRA_STRATEGY_H
#define DIJKSTRA_STRATEGY_H

#include <vector>
using namespace std;

class IDijkstraStrategy {
public:
    virtual ~IDijkstraStrategy() = default;

    virtual void inicializar(int numVertices) = 0;

    virtual void inserirOuAtualizar(int vertice, double distancia) = 0;

    virtual int extrairMinimo() = 0;

    virtual bool vazio() const = 0;

    virtual void marcarVisitado(int vertice) = 0;
};

#endif