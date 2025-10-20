#ifndef DISTANCIAS_H
#define DISTANCIAS_H

#include <vector>
#include <string>
#include <limits>
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "../busca/bfs.h"
using namespace std;

class Distancias {
private:
    int n;
    int diametro;
    pair<int, int> vertices_diametro;

    vector<int> bfs_distancias_matriz(const MatrizAdjacencia& matriz, int origem);
    vector<int> bfs_distancias_lista(const ListaAdjacencia& lista, int origem);

    void calcularDiametroAproximado_Lista(const ListaAdjacencia& lista);
    bool verificarConectividade_Lista(const ListaAdjacencia& lista) const;
    vector<int> selecionarVerticesAmostra(int numAmostras) const;

public:
    static constexpr int INFINITO = 999999;

    Distancias(int numVertices);

    void calcularDistancias_Matriz(const MatrizAdjacencia& matriz);

    void calcularDistancias_Lista(const ListaAdjacencia& lista);

    void calcularDiametroApenas_Lista(const ListaAdjacencia& lista);

    int calcularDistanciaEspecifica_Matriz(const MatrizAdjacencia& matriz, int origem, int destino) const;
    int calcularDistanciaEspecifica_Lista(const ListaAdjacencia& lista, int origem, int destino) const;

    int getDistancia_Lista(const ListaAdjacencia& lista, int origem, int destino) const;
    int getDistancia_Matriz(const MatrizAdjacencia& matriz, int origem, int destino) const;

    int getDiametro() const;

    pair<int, int> getVerticesDiametro() const;

    void imprimirMatrizDistancias() const;

    void imprimirDiametro() const;

    void salvarResultado(const string& nomeArquivo) const;

    bool isConexo_Lista(const ListaAdjacencia& lista) const;
    bool isConexo_Matriz(const MatrizAdjacencia& matriz) const;
};

#endif
