#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include <vector>
#include <string>
#include "../representacao_leitura/grafo_interface.h"
#include "../representacao_leitura/lista_adjacencia_adapter.h"
#include "../representacao_leitura/matriz_adjacencia_adapter.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "componentes.h"
#include "distancias.h"
#include "../busca/bfs.h"
#include "../busca/dfs.h"

using namespace std;

struct EstatisticasGrafo {
    int numVertices;
    int numArestas;
    int grauMinimo;
    int grauMaximo;
    double grauMedio;
    double medianaGrau;
    int numComponentes;
    vector<int> tamanhosComponentes;
    bool isConexo;
    vector<int> graus;

    int maiorComponente;
    int menorComponente;
    int diametro;
    pair<int, int> verticesDiametro;
};

class Estatisticas {
private:
    int n;
    EstatisticasGrafo stats;

    // Métodos genéricos usando interface IGrafo
    vector<int> calcularGraus(const IGrafo& grafo);
    void calcularDiametro(const IGrafo& grafo);
    void analisarBuscas(const IGrafo& grafo);
    void calcularDistanciasEspecificas(const IGrafo& grafo);

    // Métodos auxiliares
    void calcularEstatisticasGrau(const vector<int>& graus);
    double calcularMediana(const vector<int>& graus);

public:
    Estatisticas(int numVertices);

    // Métodos genéricos usando interface IGrafo
    void calcularEstatisticas(const IGrafo& grafo, int numArestas);
    void executarAnaliseCompleta(const IGrafo& grafo);

    void adicionarInformacoesComponentes(const ComponentesConexas& componentes);

    void imprimirEstatisticas() const;
    void salvarRelatorio(const string& nomeArquivo) const;

    const EstatisticasGrafo& getEstatisticas() const;
};

#endif
