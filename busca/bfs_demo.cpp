#include <iostream>
#include "../representacao_leitura/leitor_grafo.h"
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "bfs.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc < 4) {
        cout << "Uso: " << argv[0] << " <arquivo.txt> <tipo> <vertice_inicial>\n";
        cout << "Tipos disponíveis:\n";
        cout << "  matriz - Executa BFS usando matriz de adjacência\n";
        cout << "  lista  - Executa BFS usando lista de adjacência\n";
        cout << "  ambos  - Executa BFS usando ambas as representações\n";
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
        string nomeArquivoEntrada = argv[1];
        
        // Extrai o nome base do arquivo (remove caminho e extensão)
        string nomeBase = nomeArquivoEntrada;
        size_t ultimaBarra = nomeBase.find_last_of("/\\");
        if (ultimaBarra != string::npos) {
            nomeBase = nomeBase.substr(ultimaBarra + 1);
        }
        size_t ultimoPonto = nomeBase.find_last_of(".");
        if (ultimoPonto != string::npos) {
            nomeBase = nomeBase.substr(0, ultimoPonto);
        }
        
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

        // Cria objeto BFS
        BFS bfs(dados.numVertices);

        if (tipo == "matriz" || tipo == "ambos") {
            cout << "=== BFS usando Matriz de Adjacência ===\n";
            
            // Cria matriz de adjacência
            MatrizAdjacencia matriz(dados);
            
            // Executa BFS
            bfs.executarBFS_Matriz(matriz, verticeInicial);
            
            // Exibe resultado
            bfs.imprimirResultado();
            
            // Salva resultado
            string nomeArquivoMatriz = "bfs_matriz_" + nomeBase + ".txt";
            bfs.salvarResultado(nomeArquivoMatriz);
            cout << "✓ Resultado da BFS (matriz) salvo em '" << nomeArquivoMatriz << "'\n\n";
        }

        if (tipo == "lista" || tipo == "ambos") {
            cout << "=== BFS usando Lista de Adjacência ===\n";
            
            // Cria lista de adjacência
            ListaAdjacencia lista(dados);
            
            // Executa BFS (reset automático)
            bfs.executarBFS_Lista(lista, verticeInicial);
            
            // Exibe resultado
            bfs.imprimirResultado();
            
            // Salva resultado
            string nomeArquivoLista = "bfs_lista_" + nomeBase + ".txt";
            bfs.salvarResultado(nomeArquivoLista);
            cout << "✓ Resultado da BFS (lista) salvo em '" << nomeArquivoLista << "'\n\n";
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
