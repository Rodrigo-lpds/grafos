#ifndef LISTA_ADJACENCIA_H
#define LISTA_ADJACENCIA_H

#include <vector>
#include <list>
#include <string>
#include "leitor_grafo.h"
using namespace std;

class ListaAdjacencia {
private:
    int n; // número de vértices
    vector<list<int>> lista;

public:
    // Construtor que recebe dados do grafo
    ListaAdjacencia(const DadosGrafo& dados);
    
    // Salva lista em arquivo
    void salvarEmArquivo(const string& nomeSaida) const;
    
    // Imprime lista na tela
    void imprimir() const;
    
    // Getter para acessar a lista (útil para outras operações)
    const vector<list<int>>& getLista() const;
    
    // Getter para número de vértices
    int getNumVertices() const;
};

#endif
