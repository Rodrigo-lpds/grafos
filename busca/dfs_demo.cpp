#include <iostream>
#include "../representacao_leitura/leitor_grafo.h"
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "dfs.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc < 4) {
        cout << "Uso: " << argv[0] << " <arquivo.txt> <tipo> <vertice_inicial>\n";
        cout << "Tipos disponíveis:\n";
        cout << "  matriz - Executa DFS usando matriz de adjacência\n";
        cout << "  lista  - Executa DFS usando lista de adjacência\n";
        cout << "  ambos  - Executa DFS usando ambas as representações\n";
        cout << "Exemplo: " << argv[0] << " graph_example.txt matriz 1\n";
        return 1;
    }

    string tipo = argv[2];
    int verticeInicial;
    
    try {
        verticeInicial = stoi(argv[3]);
    } catch (exception& e) {
        cerr << "Erro: Vértice inicial deve ser um número inteiro\n";
        return 1;
    }

    try {
        // 1. Lê os dados do arquivo TXT
        DadosGrafo dados = LeitorGrafo::lerArquivo(argv[1]);
        cout << "Grafo lido: " << dados.numVertices << " vertices, " 
             << dados.arestas.size() << " arestas\n";
        cout << "Vértice inicial: " << verticeInicial << "\n\n";

        // Valida vértice inicial
        if (verticeInicial < 1 || verticeInicial > dados.numVertices) {
            cerr << "Erro: Vértice inicial deve estar entre 1 e " 
                 << dados.numVertices << "\n";
            return 1;
        }

        // Cria objeto DFS
        DFS dfs(dados.numVertices);

        if (tipo == "matriz" || tipo == "ambos") {
            cout << "=== DFS usando Matriz de Adjacência ===\n";
            
            // Cria matriz de adjacência
            MatrizAdjacencia matriz(dados);
            
            // Executa DFS
            dfs.executarDFS_Matriz(matriz, verticeInicial);
            
            // Exibe resultado
            dfs.imprimirResultado();
            
            // Salva resultado
            dfs.salvarResultado("dfs_matriz_resultado.txt");
            cout << "✓ Resultado da DFS (matriz) salvo em 'dfs_matriz_resultado.txt'\n\n";
        }

        if (tipo == "lista" || tipo == "ambos") {
            cout << "=== DFS usando Lista de Adjacência ===\n";
            
            // Cria lista de adjacência
            ListaAdjacencia lista(dados);
            
            // Executa DFS (reset automático)
            dfs.executarDFS_Lista(lista, verticeInicial);
            
            // Exibe resultado
            dfs.imprimirResultado();
            
            // Salva resultado
            dfs.salvarResultado("dfs_lista_resultado.txt");
            cout << "✓ Resultado da DFS (lista) salvo em 'dfs_lista_resultado.txt'\n\n";
        }

        if (tipo != "matriz" && tipo != "lista" && tipo != "ambos") {
            cout << "Tipo inválido. Use: matriz, lista ou ambos\n";
            return 1;
        }
        
    } catch (exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}
