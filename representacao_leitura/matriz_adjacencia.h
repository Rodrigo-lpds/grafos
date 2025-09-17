#ifndef MATRIZ_ADJACENCIA_H
#define MATRIZ_ADJACENCIA_H

#include <vector>
#include <string>
#include "leitor_grafo.h"
using namespace std;

class MatrizAdjacencia {
private:
    int n; // número de vértices
    vector<vector<int>> matriz;

public:
    // Construtor que recebe dados do grafo
    MatrizAdjacencia(const DadosGrafo& dados);
    
    // Salva matriz em arquivo
    void salvarEmArquivo(const string& nomeSaida) const;
    
    // Imprime matriz na tela
    void imprimir() const;
    
    // Getter para acessar a matriz (útil para BFS)
    const vector<vector<int>>& getMatriz() const;
};

#endif
