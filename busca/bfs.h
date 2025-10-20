#ifndef BFS_H
#define BFS_H

#include <vector>
#include <queue>
#include <string>
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "../representacao_leitura/grafo_interface.h"
#include "../representacao_leitura/matriz_adjacencia_adapter.h"
#include "../representacao_leitura/lista_adjacencia_adapter.h"
using namespace std;

class BFS {
private:
    int n;
    vector<bool> marcado;
    vector<int> ordem_visitacao;
    vector<int> pai;
    vector<int> nivel;

public:

    BFS(int numVertices);

    void executarBFS(const IGrafo& grafo, int verticeInicial, int destino = -1);

    void executarBFS_Matriz(const MatrizAdjacencia& matriz, int verticeInicial, int destino = -1);
    void executarBFS_Lista(const ListaAdjacencia& lista, int verticeInicial, int destino = -1);

    void imprimirResultado() const;

    void salvarResultado(const string& nomeArquivo) const;

    void reset();

    bool foiVisitado(int vertice) const;

    const vector<int>& getOrdemVisitacao() const;

    const vector<int>& getPais() const;

    const vector<int>& getNiveis() const;

    int getPai(int vertice) const;

    int getNivel(int vertice) const;
};

#endif
