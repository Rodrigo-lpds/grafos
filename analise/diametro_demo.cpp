#include <iostream>
#include "../representacao_leitura/leitor_grafo.h"
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "distancias.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Uso: " << argv[0] << " <arquivo.txt> <tipo>\n";
        cout << "Tipos disponíveis:\n";
        cout << "  matriz - Calcula diâmetro usando matriz de adjacência\n";
        cout << "  lista  - Calcula diâmetro usando lista de adjacência\n";
        cout << "  ambos  - Calcula usando ambas as representações\n";
        cout << "Exemplos:\n";
        cout << "  " << argv[0] << " grafo_1.txt matriz\n";
        cout << "  " << argv[0] << " grafo_1.txt ambos\n";
        return 1;
    }

    string tipo = argv[2];

    try {
        // Lê os dados do arquivo TXT
        DadosGrafo dados = LeitorGrafo::lerArquivo(argv[1]);
        cout << "Grafo lido: " << dados.numVertices << " vertices, " 
             << dados.arestas.size() << " arestas\n\n";

        // Cria objeto de distâncias
        Distancias dist(dados.numVertices);
        
        // Cria estruturas que serão usadas para verificação de conectividade
        MatrizAdjacencia matriz(dados);
        ListaAdjacencia lista(dados);
        bool conexo = false;

        if (tipo == "matriz" || tipo == "ambos") {
            cout << "===== CÁLCULO DE DIÂMETRO - MATRIZ =====\n";
            
            // Calcula distâncias
            dist.calcularDistancias_Matriz(matriz);
            
            // Verifica conectividade
            conexo = dist.isConexo_Matriz(matriz);
            
            // Exibe resultados
            dist.imprimirDiametro();
            
            // Salva resultado
            dist.salvarResultado("diametro_matriz_resultado.txt");
            cout << "✓ Resultado do diâmetro (matriz) salvo em 'diametro_matriz_resultado.txt'\n\n";
        }

        if (tipo == "lista" || tipo == "ambos") {
            cout << "===== CÁLCULO DE DIÂMETRO - LISTA =====\n";
            
            // Calcula distâncias
            dist.calcularDistancias_Lista(lista);
            
            // Verifica conectividade
            conexo = dist.isConexo_Lista(lista);
            
            // Exibe resultados
            dist.imprimirDiametro();
            
            // Salva resultado
            dist.salvarResultado("diametro_lista_resultado.txt");
            cout << "✓ Resultado do diâmetro (lista) salvo em 'diametro_lista_resultado.txt'\n\n";
        }

        if (tipo != "matriz" && tipo != "lista" && tipo != "ambos") {
            cout << "Tipo inválido. Use: matriz, lista ou ambos\n";
            return 1;
        }

        // Resumo final
        cout << "========== RESUMO ==========\n";
        if (conexo) {
            cout << "📊 Grafo CONEXO\n";
            cout << "📏 Diâmetro: " << dist.getDiametro() << " arestas\n";
            auto vertices_diam = dist.getVerticesDiametro();
            cout << "🎯 Vértices do diâmetro: " << vertices_diam.first 
                 << " ↔ " << vertices_diam.second << "\n";
        } else {
            cout << "📊 Grafo NÃO CONEXO\n";
            cout << "📏 Diâmetro: ∞ (infinito)\n";
            cout << "📐 Maior distância finita: " << dist.getDiametro() << " arestas\n";
        }
        
    } catch (exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}
