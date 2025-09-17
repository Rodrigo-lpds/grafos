#include <iostream>
#include "../representacao_leitura/leitor_grafo.h"
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "bfs.h"
#include "dfs.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc < 5) {
        cout << "Uso: " << argv[0] << " <arquivo.txt> <algoritmo> <tipo> <vertice_inicial>\n";
        cout << "Algoritmos disponíveis:\n";
        cout << "  bfs    - Busca em Largura\n";
        cout << "  dfs    - Busca em Profundidade\n";
        cout << "  ambos  - Executa BFS e DFS para comparação\n";
        cout << "Tipos disponíveis:\n";
        cout << "  matriz - Usando matriz de adjacência\n";
        cout << "  lista  - Usando lista de adjacência\n";
        cout << "  ambos  - Usando ambas as representações\n";
        cout << "Exemplos:\n";
        cout << "  " << argv[0] << " tree_example.txt bfs matriz 1\n";
        cout << "  " << argv[0] << " tree_example.txt dfs lista 1\n";
        cout << "  " << argv[0] << " tree_example.txt ambos ambos 1\n";
        return 1;
    }

    string algoritmo = argv[2];
    string tipo = argv[3];
    int verticeInicial;
    
    try {
        verticeInicial = stoi(argv[4]);
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

        // Cria objetos BFS e DFS
        BFS bfs(dados.numVertices);
        DFS dfs(dados.numVertices);

        // Executa BFS se solicitado
        if (algoritmo == "bfs" || algoritmo == "ambos") {
            cout << "========== BUSCA EM LARGURA (BFS) ==========\n";
            
            if (tipo == "matriz" || tipo == "ambos") {
                cout << "=== BFS usando Matriz de Adjacência ===\n";
                MatrizAdjacencia matriz(dados);
                bfs.executarBFS_Matriz(matriz, verticeInicial);
                bfs.imprimirResultado();
                string nomeArquivoBFSMatriz = "bfs_matriz_" + nomeBase + ".txt";
                bfs.salvarResultado(nomeArquivoBFSMatriz);
                cout << "✓ Resultado da BFS (matriz) salvo em '" << nomeArquivoBFSMatriz << "'\n\n";
            }

            if (tipo == "lista" || tipo == "ambos") {
                cout << "=== BFS usando Lista de Adjacência ===\n";
                ListaAdjacencia lista(dados);
                bfs.executarBFS_Lista(lista, verticeInicial);
                bfs.imprimirResultado();
                string nomeArquivoBFSLista = "bfs_lista_" + nomeBase + ".txt";
                bfs.salvarResultado(nomeArquivoBFSLista);
                cout << "✓ Resultado da BFS (lista) salvo em '" << nomeArquivoBFSLista << "'\n\n";
            }
        }

        // Executa DFS se solicitado
        if (algoritmo == "dfs" || algoritmo == "ambos") {
            cout << "======= BUSCA EM PROFUNDIDADE (DFS) =======\n";
            
            if (tipo == "matriz" || tipo == "ambos") {
                cout << "=== DFS usando Matriz de Adjacência ===\n";
                MatrizAdjacencia matriz(dados);
                dfs.executarDFS_Matriz(matriz, verticeInicial);
                dfs.imprimirResultado();
                string nomeArquivoDFSMatriz = "dfs_matriz_" + nomeBase + ".txt";
                dfs.salvarResultado(nomeArquivoDFSMatriz);
                cout << "✓ Resultado da DFS (matriz) salvo em '" << nomeArquivoDFSMatriz << "'\n\n";
            }

            if (tipo == "lista" || tipo == "ambos") {
                cout << "=== DFS usando Lista de Adjacência ===\n";
                ListaAdjacencia lista(dados);
                dfs.executarDFS_Lista(lista, verticeInicial);
                dfs.imprimirResultado();
                string nomeArquivoDFSLista = "dfs_lista_" + nomeBase + ".txt";
                dfs.salvarResultado(nomeArquivoDFSLista);
                cout << "✓ Resultado da DFS (lista) salvo em '" << nomeArquivoDFSLista << "'\n\n";
            }
        }

        // Validação dos parâmetros
        if (algoritmo != "bfs" && algoritmo != "dfs" && algoritmo != "ambos") {
            cout << "Algoritmo inválido. Use: bfs, dfs ou ambos\n";
            return 1;
        }
        
        if (tipo != "matriz" && tipo != "lista" && tipo != "ambos") {
            cout << "Tipo inválido. Use: matriz, lista ou ambos\n";
            return 1;
        }
        
        // Mostra comparação se ambos algoritmos foram executados
        if (algoritmo == "ambos") {
            cout << "============= COMPARAÇÃO =============\n";
            cout << "BFS (Largura): ";
            const auto& ordem_bfs = bfs.getOrdemVisitacao();
            for (size_t i = 0; i < ordem_bfs.size(); i++) {
                if (i > 0) cout << " -> ";
                cout << ordem_bfs[i];
            }
            cout << "\n";
            
            cout << "DFS (Profundidade): ";
            const auto& ordem_dfs = dfs.getOrdemVisitacao();
            for (size_t i = 0; i < ordem_dfs.size(); i++) {
                if (i > 0) cout << " -> ";
                cout << ordem_dfs[i];
            }
            cout << "\n\n";
            
            if (ordem_bfs == ordem_dfs) {
                cout << "📝 Os algoritmos visitaram os vértices na mesma ordem!\n";
            } else {
                cout << "📝 Os algoritmos visitaram os vértices em ordens diferentes.\n";
                cout << "   BFS explora por largura (nível por nível)\n";
                cout << "   DFS explora por profundidade (caminho mais fundo primeiro)\n";
            }
        }
        
    } catch (exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}
