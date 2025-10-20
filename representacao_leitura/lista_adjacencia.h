#ifndef LISTA_ADJACENCIA_H
#define LISTA_ADJACENCIA_H

#include <vector>
#include <string>
#include "leitor_grafo.h"
using namespace std;

class ListaAdjacencia {
private:
    int n;
    vector<vector<int>> lista;

    vector<vector<int>> getListaOrdenada() const;

public:

    ListaAdjacencia(const DadosGrafo& dados);

    void salvarEmArquivo(const string& nomeSaida) const;

    void imprimir() const;

    const vector<vector<int>>& getLista() const;

    int getNumVertices() const;
};

#endif
