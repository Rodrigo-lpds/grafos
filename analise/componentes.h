#ifndef COMPONENTES_H
#define COMPONENTES_H

#include <vector>
#include <string>
#include <algorithm>
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
using namespace std;

struct Componente {
    int id;                    // ID da componente
    int tamanho;              // Número de vértices na componente
    vector<int> vertices;     // Lista de vértices (1-based)
    
    // Construtor
    Componente(int _id, const vector<int>& _vertices) 
        : id(_id), tamanho(_vertices.size()), vertices(_vertices) {
        sort(vertices.begin(), vertices.end()); // Ordena vértices
    }
};

class ComponentesConexas {
private:
    int n; // número de vértices
    vector<Componente> componentes;
    vector<bool> visitado;
    
public:
    // Construtor
    ComponentesConexas(int numVertices);
    
    // Encontra componentes usando matriz de adjacência
    void encontrarComponentes_Matriz(const MatrizAdjacencia& matriz);
    
    // Encontra componentes usando lista de adjacência
    void encontrarComponentes_Lista(const ListaAdjacencia& lista);
    
    // Retorna número de componentes
    int getNumComponentes() const;
    
    // Retorna todas as componentes (ordenadas por tamanho decrescente)
    const vector<Componente>& getComponentes() const;
    
    // Retorna a componente de um vértice específico
    int getComponenteDoVertice(int vertice) const;
    
    // Verifica se dois vértices estão na mesma componente
    bool estaoConectados(int v1, int v2) const;
    
    // Imprime informações das componentes
    void imprimirComponentes() const;
    
    // Salva resultado em arquivo
    void salvarResultado(const string& nomeArquivo) const;
    
    // Retorna estatísticas das componentes
    void imprimirEstatisticas() const;
    
    // Reset para nova análise
    void reset();
};

#endif
