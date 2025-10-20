#include <iostream>
#include <fstream>
#include <chrono>
#include "../representacao_leitura/leitor_grafo.h"
#include "../representacao_leitura/lista_adjacencia_peso.h"
#include "dijkstra.h"

using namespace std;
using namespace chrono;

int main() {
    try {
        cout << "=== DEMONSTRACAO DO ALGORITMO DE DIJKSTRA ===\n\n";
        
        // Criar um arquivo de teste com o novo formato (apenas pesos positivos)
        cout << "Criando arquivo de teste com formato: vertice1 vertice2 peso\n";
        cout << "NOTA: Dijkstra funciona apenas com pesos nao-negativos!\n";
        ofstream arquivoTeste("../grafos/grafo_peso_teste.txt");
        arquivoTeste << "5\n";
        arquivoTeste << "1 2 0.1\n";
        arquivoTeste << "2 5 0.2\n";
        arquivoTeste << "5 3 5.0\n";
        arquivoTeste << "3 4 1.5\n";
        arquivoTeste << "4 5 2.3\n";
        arquivoTeste << "1 5 1.0\n";
        arquivoTeste.close();
        
        // Ler grafo com peso
        cout << "Lendo grafo do arquivo...\n";
        DadosGrafoPeso dados = LeitorGrafo::lerArquivoComPeso("../grafos/grafo_peso_teste.txt");
        
        // Criar lista de adjacência com peso
        ListaAdjacenciaPeso grafo(dados);
        
        cout << "\nGrafo carregado:\n";
        grafo.imprimir();
        
        // Escolher vértice de origem
        int origem;
        cout << "\nDigite o vertice de origem (1 a " << dados.numVertices << "): ";
        cin >> origem;
        
        if (origem < 1 || origem > dados.numVertices) {
            cout << "Vertice invalido!\n";
            return 1;
        }
        
        cout << "\n=== DIJKSTRA COM VETOR ===\n";
        auto inicio = high_resolution_clock::now();
        ResultadoDijkstra resultadoVetor = Dijkstra::dijkstraVetor(grafo, origem);
        auto fim = high_resolution_clock::now();
        auto duracaoVetor = duration_cast<microseconds>(fim - inicio);
        
        Dijkstra::imprimirResultado(resultadoVetor);
        cout << "Tempo de execucao (vetor): " << duracaoVetor.count() << " microsegundos\n";
        
        cout << "\n=== DIJKSTRA COM HEAP ===\n";
        inicio = high_resolution_clock::now();
        ResultadoDijkstra resultadoHeap = Dijkstra::dijkstraHeap(grafo, origem);
        fim = high_resolution_clock::now();
        auto duracaoHeap = duration_cast<microseconds>(fim - inicio);
        
        Dijkstra::imprimirResultado(resultadoHeap);
        cout << "Tempo de execucao (heap): " << duracaoHeap.count() << " microsegundos\n";
        
        // Testar reconstrução de caminho
        int destino;
        cout << "\nDigite um vertice de destino para ver o caminho (1 a " << dados.numVertices << "): ";
        cin >> destino;
        
        if (destino >= 1 && destino <= dados.numVertices) {
            vector<int> caminho = Dijkstra::obterCaminho(resultadoVetor, destino);
            
            if (caminho.empty()) {
                cout << "Nao ha caminho de " << origem << " para " << destino << "\n";
            } else {
                cout << "Caminho minimo de " << origem << " para " << destino << ": ";
                for (int i = 0; i < caminho.size(); i++) {
                    if (i > 0) cout << " -> ";
                    cout << caminho[i];
                }
                cout << "\n";
                cout << "Distancia total: " << resultadoVetor.distancias[destino - 1] << "\n";
            }
        }
        
    } catch (const exception& e) {
        cout << "Erro: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}