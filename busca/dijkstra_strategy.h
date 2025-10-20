#ifndef DIJKSTRA_STRATEGY_H
#define DIJKSTRA_STRATEGY_H

#include <vector>
using namespace std;

/**
 * Interface Strategy para diferentes implementações de busca do próximo vértice
 * no algoritmo de Dijkstra. Aplica o Princípio da Inversão de Dependência (SOLID).
 */
class IDijkstraStrategy {
public:
    virtual ~IDijkstraStrategy() = default;
    
    /**
     * Inicializa a estratégia com o número de vértices.
     */
    virtual void inicializar(int numVertices) = 0;
    
    /**
     * Adiciona ou atualiza um vértice na estrutura de dados.
     * @param vertice Índice do vértice (0-based)
     * @param distancia Nova distância do vértice
     */
    virtual void inserirOuAtualizar(int vertice, double distancia) = 0;
    
    /**
     * Remove e retorna o vértice com menor distância.
     * @return Índice do vértice com menor distância (0-based), ou -1 se vazio
     */
    virtual int extrairMinimo() = 0;
    
    /**
     * Verifica se ainda há vértices na estrutura.
     * @return true se vazio, false caso contrário
     */
    virtual bool vazio() const = 0;
    
    /**
     * Marca um vértice como visitado (para implementações que precisam).
     * @param vertice Índice do vértice (0-based)
     */
    virtual void marcarVisitado(int vertice) = 0;
};

#endif