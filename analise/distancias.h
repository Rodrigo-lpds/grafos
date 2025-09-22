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
    int n; // número de vértices
    int diametro; // diâmetro do grafo
    pair<int, int> vertices_diametro; // par de vértices que formam o diâmetro
    
    // Função auxiliar para calcular distâncias usando BFS
    vector<int> bfs_distancias_matriz(const MatrizAdjacencia& matriz, int origem);
    vector<int> bfs_distancias_lista(const ListaAdjacencia& lista, int origem);
    
    // Funções otimizadas para cálculo do diâmetro
    void calcularDiametroAproximado_Lista(const ListaAdjacencia& lista);
    bool verificarConectividade_Lista(const ListaAdjacencia& lista) const;
    vector<int> selecionarVerticesAmostra(int numAmostras) const;
    
public:
    static constexpr int INFINITO = 999999; // Valor grande para representar infinito
    
    // Construtor
    Distancias(int numVertices);
    
    // Calcula todas as distâncias usando matriz de adjacência
    void calcularDistancias_Matriz(const MatrizAdjacencia& matriz);
    
    // Calcula todas as distâncias usando lista de adjacência
    void calcularDistancias_Lista(const ListaAdjacencia& lista);
    
    // Versão otimizada para calcular apenas o diâmetro (sem matriz completa)
    void calcularDiametroApenas_Lista(const ListaAdjacencia& lista);
    
    // Métodos eficientes para consultas específicas (apenas uma BFS)
    int calcularDistanciaEspecifica_Matriz(const MatrizAdjacencia& matriz, int origem, int destino) const;
    int calcularDistanciaEspecifica_Lista(const ListaAdjacencia& lista, int origem, int destino) const;
    
    // Retorna a distância entre dois vértices (calcula sob demanda)
    int getDistancia_Lista(const ListaAdjacencia& lista, int origem, int destino) const;
    int getDistancia_Matriz(const MatrizAdjacencia& matriz, int origem, int destino) const;
    
    // Retorna o diâmetro do grafo
    int getDiametro() const;
    
    // Retorna o par de vértices que forma o diâmetro
    pair<int, int> getVerticesDiametro() const;
    
    // Imprime matriz de distâncias
    void imprimirMatrizDistancias() const;
    
    // Imprime informações sobre o diâmetro
    void imprimirDiametro() const;
    
    // Salva resultados em arquivo
    void salvarResultado(const string& nomeArquivo) const;
    
    // Verifica se o grafo é conexo (usando BFS de um vértice)
    bool isConexo_Lista(const ListaAdjacencia& lista) const;
    bool isConexo_Matriz(const MatrizAdjacencia& matriz) const;
};

#endif
