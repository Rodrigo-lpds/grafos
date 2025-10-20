#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <limits>
#include "../representacao_leitura/lista_adjacencia_peso.h"
using namespace std;

const double INF = numeric_limits<double>::infinity();

struct ResultadoDijkstra {
    vector<double> distancias;
    vector<int> predecessores;
    int origem;
};

class Dijkstra {
public:
    // Implementação usando vetor (busca linear pelo mínimo)
    static ResultadoDijkstra dijkstraVetor(const ListaAdjacenciaPeso& grafo, int origem);
    
    // Implementação usando heap (priority_queue)
    static ResultadoDijkstra dijkstraHeap(const ListaAdjacenciaPeso& grafo, int origem);
    
    // Função auxiliar para imprimir resultado
    static void imprimirResultado(const ResultadoDijkstra& resultado);
    
    // Função para reconstruir caminho até um vértice
    static vector<int> obterCaminho(const ResultadoDijkstra& resultado, int destino);
};

#endif