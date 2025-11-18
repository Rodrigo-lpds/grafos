#ifndef LEITOR_GRAFO_H
#define LEITOR_GRAFO_H

#include <vector>
#include <string>
using namespace std;

struct DadosGrafo {
    int numVertices;
    vector<pair<int, int>> arestas;
    bool direcionado;
    
    DadosGrafo() : numVertices(0), direcionado(false) {}
    DadosGrafo(int n, bool dir = false) : numVertices(n), direcionado(dir) {}
};

struct DadosGrafoPeso {
    int numVertices;
    vector<tuple<int, int, double>> arestas;
    bool direcionado;
    
    DadosGrafoPeso() : numVertices(0), direcionado(false) {}
    DadosGrafoPeso(int n, bool dir = false) : numVertices(n), direcionado(dir) {}
};

class LeitorGrafo {
public:
    static DadosGrafo lerArquivo(const string& nomeArquivo, bool direcionado = false);
    static DadosGrafoPeso lerArquivoComPeso(const string& nomeArquivo, bool direcionado = false);
};

#endif
