#include "lista_adjacencia.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

ListaAdjacencia::ListaAdjacencia(const DadosGrafo& dados) {
    n = dados.numVertices;
    lista.resize(n);

    for (const auto& aresta : dados.arestas) {
        int u = aresta.first - 1;
        int v = aresta.second - 1;
        lista[u].push_back(v + 1);
        lista[v].push_back(u + 1);
    }
}

vector<vector<int>> ListaAdjacencia::getListaOrdenada() const {
    vector<vector<int>> listaOrdenada = lista;
    for (int i = 0; i < n; i++) {
        sort(listaOrdenada[i].begin(), listaOrdenada[i].end());
    }
    return listaOrdenada;
}

void ListaAdjacencia::salvarEmArquivo(const string& nomeSaida) const {
    ofstream saida(nomeSaida);
    if (!saida) {
        throw runtime_error("Erro ao criar arquivo de saída");
    }

    // Usa lista ordenada apenas para visualização
    auto listaOrdenada = getListaOrdenada();
    
    saida << "Lista de Adjacencia (" << n << " vertices):\n";
    for (int i = 0; i < n; i++) {
        saida << "Vertice " << (i + 1) << ": ";
        if (listaOrdenada[i].empty()) {
            saida << "(sem vizinhos)";
        } else {
            bool primeiro = true;
            for (auto vizinho : listaOrdenada[i]) {
                if (!primeiro) saida << ", ";
                saida << vizinho;
                primeiro = false;
            }
        }
        saida << "\n";
    }
}

void ListaAdjacencia::imprimir() const {
    // Usa lista ordenada apenas para visualização
    auto listaOrdenada = getListaOrdenada();
    
    cout << "Lista de Adjacencia (" << n << " vertices):\n";
    for (int i = 0; i < n; i++) {
        cout << "Vertice " << (i + 1) << ": ";
        if (listaOrdenada[i].empty()) {
            cout << "(sem vizinhos)";
        } else {
            bool primeiro = true;
            for (auto vizinho : listaOrdenada[i]) {
                if (!primeiro) cout << ", ";
                cout << vizinho;
                primeiro = false;
            }
        }
        cout << "\n";
    }
}

const vector<vector<int>>& ListaAdjacencia::getLista() const {
    return lista;
}

int ListaAdjacencia::getNumVertices() const {
    return n;
}
