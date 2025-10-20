#ifndef COMPONENTES_H
#define COMPONENTES_H

#include <vector>
#include <string>
#include <algorithm>
#include "../representacao_leitura/grafo_interface.h"
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
using namespace std;

struct Componente {
    int id;
    int tamanho;
    vector<int> vertices;

    Componente(int _id, const vector<int>& _vertices)
        : id(_id), tamanho(_vertices.size()), vertices(_vertices) {
        sort(vertices.begin(), vertices.end());
    }
};

class ComponentesConexas {
private:
    int n;
    vector<Componente> componentes;
    vector<bool> visitado;

public:

    ComponentesConexas(int numVertices);

    // Método genérico usando interface IGrafo
    void encontrarComponentes(const IGrafo& grafo);

    int getNumComponentes() const;

    const vector<Componente>& getComponentes() const;

    int getComponenteDoVertice(int vertice) const;

    bool estaoConectados(int v1, int v2) const;

    void imprimirComponentes() const;

    void salvarResultado(const string& nomeArquivo) const;

    void imprimirEstatisticas() const;

    void reset();
};

#endif
