#ifndef MATRIZ_ADJACENCIA_H
#define MATRIZ_ADJACENCIA_H

#include <vector>
#include <string>
#include <cstdint>
#include "leitor_grafo.h"
using namespace std;

class MatrizAdjacencia {
private:
    int n;
    vector<vector<uint8_t>> matriz;

public:

    MatrizAdjacencia(const DadosGrafo& dados);

    void salvarEmArquivo(const string& nomeSaida) const;

    void imprimir() const;

    const vector<vector<uint8_t>>& getMatriz() const;
};

#endif
