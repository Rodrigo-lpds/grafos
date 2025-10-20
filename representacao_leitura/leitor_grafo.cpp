#include "leitor_grafo.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <tuple>

DadosGrafo LeitorGrafo::lerArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw runtime_error("Erro ao abrir arquivo de entrada");
    }

    DadosGrafo dados;
    string linha;

    if (getline(arquivo, linha)) {
        dados.numVertices = stoi(linha);
    }

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

DadosGrafoPeso LeitorGrafo::lerArquivoComPeso(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw runtime_error("Erro ao abrir arquivo de entrada");
    }

    DadosGrafoPeso dados;
    string linha;

    if (getline(arquivo, linha)) {
        dados.numVertices = stoi(linha);
    }

    while (getline(arquivo, linha)) {
        if (!linha.empty()) {
            istringstream iss(linha);
            int u, v;
            double peso;
            if (iss >> u >> v >> peso) {
                dados.arestas.push_back(make_tuple(u, v, peso));
            }
        }
    }

    return dados;
}
