#ifndef GRAFO_INTERFACE_H
#define GRAFO_INTERFACE_H

#include <vector>
using namespace std;

class IGrafo {
public:
    virtual ~IGrafo() = default;

    virtual int getNumVertices() const = 0;

    virtual vector<int> getVizinhos(int vertice) const = 0;

    virtual bool existeAresta(int origem, int destino) const = 0;
};

#endif