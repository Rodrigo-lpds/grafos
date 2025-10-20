#ifndef DFS_H
#define DFS_H

#include <vector>
#include <stack>
#include <string>
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "../representacao_leitura/grafo_interface.h"
#include "../representacao_leitura/matriz_adjacencia_adapter.h"
#include "../representacao_leitura/lista_adjacencia_adapter.h"
using namespace std;

class DFS {
private:
    int n;
    vector<bool> marcado;
    vector<int> ordem_visitacao;
    vector<int> pai;
    vector<int> nivel;

public:

    DFS(int numVertices);

    void executarDFS(const IGrafo& grafo, int verticeInicial);

    void executarDFS_Matriz(const MatrizAdjacencia& matriz, int verticeInicial);
    void executarDFS_Lista(const ListaAdjacencia& lista, int verticeInicial);

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
