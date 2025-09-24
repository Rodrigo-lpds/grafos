#ifndef MATRIZ_ADJACENCIA_H
#define MATRIZ_ADJACENCIA_H

#include <vector>
#include <string>
#include <cstdint>
#include "leitor_grafo.h"
using namespace std;

class MatrizAdjacencia {
private:
    int n; // número de vértices
    vector<vector<uint8_t>> matriz;  // uint8_t economiza 75% da memória (4→1 byte)

public:
    // Construtor que recebe dados do grafo
    MatrizAdjacencia(const DadosGrafo& dados);
    
    // Salva matriz em arquivo
    void salvarEmArquivo(const string& nomeSaida) const;
    
    // Imprime matriz na tela
    void imprimir() const;
    
    // Getter para acessar a matriz (útil para BFS)
    const vector<vector<uint8_t>>& getMatriz() const;
};

#endif
