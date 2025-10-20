#ifndef GRAFO_PESO_INTERFACE_H
#define GRAFO_PESO_INTERFACE_H

#include <vector>
using namespace std;

class IGrafoPeso {
public:
    virtual ~IGrafoPeso() = default;

    virtual int getNumVertices() const = 0;

    virtual vector<pair<int, double>> getVizinhosComPeso(int vertice) const = 0;

    virtual pair<bool, double> getAresta(int origem, int destino) const = 0;
};

#endif