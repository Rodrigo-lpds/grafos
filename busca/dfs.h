#ifndef DFS_H
#define DFS_H

#include <vector>
#include <stack>
#include <string>
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
using namespace std;

class DFS {
private:
    int n; // número de vértices
    vector<bool> marcado;
    vector<int> ordem_visitacao;
    vector<int> pai; // pai de cada vértice na árvore de busca
    vector<int> nivel; // nível de cada vértice na árvore (raiz = 0)
    
public:
    // Construtor
    DFS(int numVertices);
    
    // DFS usando matriz de adjacência
    void executarDFS_Matriz(const MatrizAdjacencia& matriz, int verticeInicial);
    
    // DFS usando lista de adjacência
    void executarDFS_Lista(const ListaAdjacencia& lista, int verticeInicial);
    
    // Imprime resultado da DFS
    void imprimirResultado() const;
    
    // Salva resultado em arquivo
    void salvarResultado(const string& nomeArquivo) const;
    
    // Reseta o estado para nova execução
    void reset();
    
    // Verifica se um vértice foi visitado
    bool foiVisitado(int vertice) const;
    
    // Retorna a ordem de visitação
    const vector<int>& getOrdemVisitacao() const;
    
    // Retorna o pai de cada vértice na árvore de busca
    const vector<int>& getPais() const;
    
    // Retorna o nível de cada vértice na árvore de busca
    const vector<int>& getNiveis() const;
    
    // Retorna o pai de um vértice específico
    int getPai(int vertice) const;
    
    // Retorna o nível de um vértice específico
    int getNivel(int vertice) const;
};

#endif
