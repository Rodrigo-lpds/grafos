#include <iostream>
#include "leitor_grafo.h"
#include "matriz_adjacencia.h"
#include "lista_adjacencia.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Uso: " << argv[0] << " <arquivo.txt> <tipo>\n";
        cout << "Tipos disponíveis:\n";
        cout << "  matriz - Gera matriz de adjacência\n";
        cout << "  lista  - Gera lista de adjacência\n";
        cout << "  ambos  - Gera ambas as representações\n";
        return 1;
    }

    string tipo = argv[2];

    try {
        // 1. Lê os dados do arquivo TXT
        DadosGrafo dados = LeitorGrafo::lerArquivo(argv[1]);
        cout << "Grafo lido: " << dados.numVertices << " vertices, " 
             << dados.arestas.size() << " arestas\n\n";

        if (tipo == "matriz" || tipo == "ambos") {
            // 2a. Monta a matriz de adjacência
            MatrizAdjacencia matriz(dados);
            matriz.salvarEmArquivo("matriz.txt");
            cout << "✓ Matriz de adjacencia salva em 'matriz.txt'\n";
        }

        if (tipo == "lista" || tipo == "ambos") {
            // 2b. Monta a lista de adjacência
            ListaAdjacencia lista(dados);
            lista.salvarEmArquivo("lista.txt");
            cout << "✓ Lista de adjacencia salva em 'lista.txt'\n";
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
