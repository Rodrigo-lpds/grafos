#ifndef GRAFO_PESO_INTERFACE_H
#define GRAFO_PESO_INTERFACE_H

#include <vector>
using namespace std;

/**
 * Interface abstrata para representação de grafos com peso.
 * Estende o conceito da IGrafo para grafos ponderados.
 * Aplica o Princípio da Inversão de Dependência (SOLID).
 */
class IGrafoPeso {
public:
    virtual ~IGrafoPeso() = default;
    
    /**
     * Retorna o número de vértices do grafo.
     */
    virtual int getNumVertices() const = 0;
    
    /**
     * Retorna os vizinhos de um vértice com seus pesos (em formato 0-based).
     * @param vertice Vértice em formato 0-based (0 a n-1)
     * @return Vector com pares (vizinho_0based, peso)
     */
    virtual vector<pair<int, double>> getVizinhosComPeso(int vertice) const = 0;
    
    /**
     * Verifica se existe uma aresta entre dois vértices e retorna seu peso.
     * @param origem Vértice de origem (0-based)
     * @param destino Vértice de destino (0-based)
     * @return pair<bool, double> - (existe_aresta, peso)
     */
    virtual pair<bool, double> getAresta(int origem, int destino) const = 0;
};

#endif