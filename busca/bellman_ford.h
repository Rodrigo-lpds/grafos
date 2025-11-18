#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <vector>
#include <limits>
#include "../representacao_leitura/lista_adjacencia_peso.h"
#include "../representacao_leitura/grafo_peso_interface.h"
#include "../representacao_leitura/lista_adjacencia_peso_adapter.h"
using namespace std;

struct ResultadoBellmanFord {
    vector<double> distancias;
    vector<int> predecessores;
    int origem;
    bool temCicloNegativo;
    vector<int> cicloNegativo; // vértices do ciclo negativo, se existir
};

class BellmanFord {
public:
    // Método principal usando a interface IGrafoPeso
    static ResultadoBellmanFord executar(const IGrafoPeso& grafo, int origem);

    // Método de conveniência para ListaAdjacenciaPeso
    static ResultadoBellmanFord executar(const ListaAdjacenciaPeso& grafo, int origem);

    // Utilitários
    static void imprimirResultado(const ResultadoBellmanFord& resultado);
    static vector<int> obterCaminho(const ResultadoBellmanFord& resultado, int destino);

private:
    // Detecta ciclo negativo e reconstrói o caminho
    static vector<int> detectarCicloNegativo(const IGrafoPeso& grafo,
                                           const vector<double>& distancias,
                                           const vector<int>& predecessores);
};

#endif