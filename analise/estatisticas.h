#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include <vector>
#include <string>
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
    vector<int> graus; // Para cálculos
    
    // Informações adicionais
    int maiorComponente;
    int menorComponente;
    int diametro;
    pair<int, int> verticesDiametro;
};

class Estatisticas {
private:
    int n;
    EstatisticasGrafo stats;
    
    vector<int> calcularGraus_Lista(const ListaAdjacencia& lista);
    void calcularEstatisticasGrau(const vector<int>& graus);
    double calcularMediana(const vector<int>& graus);
    void calcularDiametro_Lista(const ListaAdjacencia& lista);
    void analisarBuscas_Lista(const ListaAdjacencia& lista);
    void calcularDistanciasEspecificas_Lista(const ListaAdjacencia& lista);

public:
    Estatisticas(int numVertices);
    
    void calcularEstatisticas_Lista(const ListaAdjacencia& lista, int numArestas);
    
    void adicionarInformacoesComponentes(const ComponentesConexas& componentes);
    
    void imprimirEstatisticas() const;
    void salvarRelatorio(const string& nomeArquivo) const;
    
    // Análises adicionais
    void executarAnaliseCompleta_Lista(const ListaAdjacencia& lista);
    
    const EstatisticasGrafo& getEstatisticas() const;
};

#endif
