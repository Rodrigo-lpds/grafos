#ifndef LISTA_ADJACENCIA_PESO_H
#define LISTA_ADJACENCIA_PESO_H

#include <vector>
#include <string>
#include <tuple>
#include "leitor_grafo.h"
using namespace std;

class ListaAdjacenciaPeso {
private:
    int n;
    vector<vector<pair<int, double>>> lista;

    vector<vector<pair<int, double>>> getListaOrdenada() const;

public:

    ListaAdjacenciaPeso(const DadosGrafoPeso& dados);

    void salvarEmArquivo(const string& nomeSaida) const;

    void imprimir() const;

    const vector<vector<pair<int, double>>>& getLista() const;

    int getNumVertices() const;
};

#endif