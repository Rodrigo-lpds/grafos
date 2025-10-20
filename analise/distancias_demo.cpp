#include <iostream>
#include "../representacao_leitura/leitor_grafo.h"
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "distancias.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc < 5) {
        cout << "Uso: " << argv[0] << " <arquivo.txt> <tipo> <origem> <destino>\n";
        cout << "Tipos disponíveis:\n";
        cout << "  matriz - Calcula distância usando matriz de adjacência\n";
        cout << "  lista  - Calcula distância usando lista de adjacência\n";
        cout << "Exemplos:\n";
        cout << "  " << argv[0] << " grafo_1.txt matriz 1 6\n";
        cout << "  " << argv[0] << " grafo_1.txt lista 2 5\n";
        return 1;
    }

    string tipo = argv[2];
    int origem, destino;

    try {
        origem = stoi(argv[3]);
        destino = stoi(argv[4]);
    } catch (exception& e) {
        cerr << "Erro: Vértices devem ser números inteiros\n";
        return 1;
    }

    try {

        DadosGrafo dados = LeitorGrafo::lerArquivo(argv[1]);

        Distancias dist(dados.numVertices);

        int d;
        if (tipo == "matriz") {

            MatrizAdjacencia matriz(dados);

            d = dist.calcularDistanciaEspecifica_Matriz(matriz, origem, destino);
        }
        else if (tipo == "lista") {

            ListaAdjacencia lista(dados);

            d = dist.calcularDistanciaEspecifica_Lista(lista, origem, destino);
        }
        else {
            cout << "Tipo inválido. Use: matriz ou lista\n";
            return 1;
        }

        if (d == Distancias::INFINITO) {
            cout << "∞" << endl;
        } else {
            cout << d << endl;
        }

    } catch (exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}
