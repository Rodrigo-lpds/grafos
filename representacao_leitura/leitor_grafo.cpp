#include "leitor_grafo.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

DadosGrafo LeitorGrafo::lerArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw runtime_error("Erro ao abrir arquivo de entrada");
    }

    DadosGrafo dados;
    string linha;
    
    // Lê número de vértices
    if (getline(arquivo, linha)) {
        dados.numVertices = stoi(linha);
    }

    // Lê arestas
    while (getline(arquivo, linha)) {
        if (!linha.empty()) {
            istringstream iss(linha);
            int u, v;
            if (iss >> u >> v) {
                dados.arestas.push_back({u, v});
            }
        }
    }

    return dados;
}
