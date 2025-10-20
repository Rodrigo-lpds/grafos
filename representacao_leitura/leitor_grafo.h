#ifndef LEITOR_GRAFO_H
#define LEITOR_GRAFO_H

#include <vector>
#include <string>
using namespace std;

struct DadosGrafo {
    int numVertices;
    vector<pair<int, int>> arestas;
};

struct DadosGrafoPeso {
    int numVertices;
    vector<tuple<int, int, double>> arestas;
};

class LeitorGrafo {
public:
    static DadosGrafo lerArquivo(const string& nomeArquivo);
    static DadosGrafoPeso lerArquivoComPeso(const string& nomeArquivo);
};

#endif
