#ifndef GRAFO_INTERFACE_H
#define GRAFO_INTERFACE_H

#include <vector>
using namespace std;

/**
 * Interface abstrata para representação de grafos.
 * Aplica o Princípio da Inversão de Dependência (SOLID) - 
 * as classes de busca dependem de abstrações, não de implementações concretas.
 */
class IGrafo {
public:
    virtual ~IGrafo() = default;
    
    /**
     * Retorna o número de vértices do grafo.
     */
    virtual int getNumVertices() const = 0;
    
    /**
     * Retorna os vizinhos de um vértice (em formato 0-based).
     * @param vertice Vértice em formato 0-based (0 a n-1)
     * @return Vector com os vizinhos em formato 0-based
     */
    virtual vector<int> getVizinhos(int vertice) const = 0;
    
    /**
     * Verifica se existe uma aresta entre dois vértices.
     * @param origem Vértice de origem (0-based)
     * @param destino Vértice de destino (0-based)
     * @return true se existe aresta, false caso contrário
     */
    virtual bool existeAresta(int origem, int destino) const = 0;
};

#endif