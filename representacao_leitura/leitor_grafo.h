#ifndef LEITOR_GRAFO_H
#define LEITOR_GRAFO_H

#include <vector>
#include <string>
using namespace std;

struct DadosGrafo {
    int numVertices;
    vector<pair<int, int>> arestas;
};

class LeitorGrafo {
public:
    static DadosGrafo lerArquivo(const string& nomeArquivo);
};

#endif
