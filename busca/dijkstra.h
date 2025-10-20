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

class Dijkstra {
public:

    static ResultadoDijkstra executar(const IGrafoPeso& grafo, int origem,
                                     unique_ptr<IDijkstraStrategy> strategy);

    static ResultadoDijkstra dijkstraVetor(const ListaAdjacenciaPeso& grafo, int origem);
    static ResultadoDijkstra dijkstraHeap(const ListaAdjacenciaPeso& grafo, int origem);

    static void imprimirResultado(const ResultadoDijkstra& resultado);

    static vector<int> obterCaminho(const ResultadoDijkstra& resultado, int destino);
};

#endif