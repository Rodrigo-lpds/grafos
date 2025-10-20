#ifndef LISTA_ADJACENCIA_PESO_H
#define LISTA_ADJACENCIA_PESO_H

#include <vector>
#include <string>
#include <tuple>
#include "leitor_grafo.h"
using namespace std;

class ListaAdjacenciaPeso {
private:
    int n; // número de vértices
    vector<vector<pair<int, double>>> lista; // lista[i] = {(vizinho, peso), ...}
    
    // Retorna uma cópia ordenada da lista (só quando necessário para visualização)
    vector<vector<pair<int, double>>> getListaOrdenada() const;

public:
    // Construtor que recebe dados do grafo com peso
    ListaAdjacenciaPeso(const DadosGrafoPeso& dados);
    
    // Salva lista em arquivo
    void salvarEmArquivo(const string& nomeSaida) const;
    
    // Imprime lista na tela
    void imprimir() const;
    
    // Getter para acessar a lista (útil para outras operações)
    const vector<vector<pair<int, double>>>& getLista() const;
    
    // Getter para número de vértices
    int getNumVertices() const;
};

#endif