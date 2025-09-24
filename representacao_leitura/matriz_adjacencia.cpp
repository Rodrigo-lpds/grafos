#include "matriz_adjacencia.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

MatrizAdjacencia::MatrizAdjacencia(const DadosGrafo& dados) {
    n = dados.numVertices;
    matriz.assign(n, vector<uint8_t>(n, 0));

    for (const auto& aresta : dados.arestas) {
        int u = aresta.first;
        int v = aresta.second;
        matriz[u-1][v-1] = 1;
        matriz[v-1][u-1] = 1;
    }
}

void MatrizAdjacencia::salvarEmArquivo(const string& nomeSaida) const {
    ofstream saida(nomeSaida);
    if (!saida) {
        throw runtime_error("Erro ao criar arquivo de saída");
    }

    saida << "Matriz de Adjacencia (" << n << "x" << n << "):\n";
    
    // Otimização: guarda referência da matriz fora do laço
    const auto& M = matriz;
    for (int i = 0; i < n; i++) {
        const auto& linha = M[i];  // Evita acesso repetido à matriz[i]
        for (int j = 0; j < n; j++) {
            saida << static_cast<int>(linha[j]) << " ";  // Cast para impressão
        }
        saida << "\n";
    }
}

void MatrizAdjacencia::imprimir() const {
    cout << "Matriz de Adjacencia (" << n << "x" << n << "):\n";
    
    // Otimização: guarda referência da matriz fora do laço
    const auto& M = matriz;
    for (int i = 0; i < n; i++) {
        const auto& linha = M[i];  // Evita acesso repetido à matriz[i]
        for (int j = 0; j < n; j++) {
            cout << static_cast<int>(linha[j]) << " ";  // Cast para impressão
        }
        cout << "\n";
    }
}

const vector<vector<uint8_t>>& MatrizAdjacencia::getMatriz() const {
    return matriz;
}
