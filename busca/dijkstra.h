#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <limits>
#include <memory>
#include "../representacao_leitura/lista_adjacencia_peso.h"
#include "../representacao_leitura/grafo_peso_interface.h"
#include "../representacao_leitura/lista_adjacencia_peso_adapter.h"
#include "dijkstra_strategy.h"
#include "dijkstra_vetor_strategy.h"
#include "dijkstra_heap_strategy.h"
using namespace std;

extern const double INF;

struct ResultadoDijkstra {
    vector<double> distancias;
    vector<int> predecessores;
    int origem;
};

/**
 * Classe Dijkstra refatorada seguindo princípios SOLID.
 * - Single Responsibility: responsável apenas pelo algoritmo de Dijkstra
 * - Open/Closed: aberto para novas estratégias, fechado para modificação
 * - Liskov Substitution: qualquer estratégia IDijkstraStrategy pode ser usada
 * - Interface Segregation: depende apenas das interfaces necessárias
 * - Dependency Inversion: depende de abstrações, não de implementações concretas
 */
class Dijkstra {
public:
    // Nova implementação polimórfica usando Strategy Pattern
    static ResultadoDijkstra executar(const IGrafoPeso& grafo, int origem, 
                                     unique_ptr<IDijkstraStrategy> strategy);
    
    // Métodos de conveniência para compatibilidade (usam strategy internamente)
    static ResultadoDijkstra dijkstraVetor(const ListaAdjacenciaPeso& grafo, int origem);
    static ResultadoDijkstra dijkstraHeap(const ListaAdjacenciaPeso& grafo, int origem);
    
    // Função auxiliar para imprimir resultado
    static void imprimirResultado(const ResultadoDijkstra& resultado);
    
    // Função para reconstruir caminho até um vértice
    static vector<int> obterCaminho(const ResultadoDijkstra& resultado, int destino);
};

#endif