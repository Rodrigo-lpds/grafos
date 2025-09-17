#include "matriz_adjacencia.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

MatrizAdjacencia::MatrizAdjacencia(const DadosGrafo& dados) {
    n = dados.numVertices;
    matriz.assign(n, vector<int>(n, 0));

    // Adiciona arestas à matriz
    for (const auto& aresta : dados.arestas) {
        int u = aresta.first;
        int v = aresta.second;
        matriz[u-1][v-1] = 1;
        matriz[v-1][u-1] = 1; // grafo não-direcionado
    }
}

void MatrizAdjacencia::salvarEmArquivo(const string& nomeSaida) const {
    ofstream saida(nomeSaida);
    if (!saida) {
        throw runtime_error("Erro ao criar arquivo de saída");
    }

    saida << "Matriz de Adjacencia (" << n << "x" << n << "):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            saida << matriz[i][j] << " ";
        }
        saida << "\n";
    }
}

void MatrizAdjacencia::imprimir() const {
    cout << "Matriz de Adjacencia (" << n << "x" << n << "):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matriz[i][j] << " ";
        }
        cout << "\n";
    }
}

const vector<vector<int>>& MatrizAdjacencia::getMatriz() const {
    return matriz;
}
