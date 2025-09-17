#include "lista_adjacencia.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

ListaAdjacencia::ListaAdjacencia(const DadosGrafo& dados) {
    n = dados.numVertices;
    lista.resize(n);

    // Adiciona arestas à lista
    for (const auto& aresta : dados.arestas) {
        int u = aresta.first - 1;  // converte para índice 0-based
        int v = aresta.second - 1; // converte para índice 0-based
        
        lista[u].push_back(v + 1); // salva como 1-based para exibição
        lista[v].push_back(u + 1); // grafo não-direcionado
    }
    
    // Ordena as listas de adjacência para melhor visualização
    for (int i = 0; i < n; i++) {
        lista[i].sort();
    }
}

void ListaAdjacencia::salvarEmArquivo(const string& nomeSaida) const {
    ofstream saida(nomeSaida);
    if (!saida) {
        throw runtime_error("Erro ao criar arquivo de saída");
    }

    saida << "Lista de Adjacencia (" << n << " vertices):\n";
    for (int i = 0; i < n; i++) {
        saida << "Vertice " << (i + 1) << ": ";
        if (lista[i].empty()) {
            saida << "(sem vizinhos)";
        } else {
            bool primeiro = true;
            for (auto vizinho : lista[i]) {
                if (!primeiro) saida << ", ";
                saida << vizinho;
                primeiro = false;
            }
        }
        saida << "\n";
    }
}

void ListaAdjacencia::imprimir() const {
    cout << "Lista de Adjacencia (" << n << " vertices):\n";
    for (int i = 0; i < n; i++) {
        cout << "Vertice " << (i + 1) << ": ";
        if (lista[i].empty()) {
            cout << "(sem vizinhos)";
        } else {
            bool primeiro = true;
            for (auto vizinho : lista[i]) {
                if (!primeiro) cout << ", ";
                cout << vizinho;
                primeiro = false;
            }
        }
        cout << "\n";
    }
}

const vector<list<int>>& ListaAdjacencia::getLista() const {
    return lista;
}

int ListaAdjacencia::getNumVertices() const {
    return n;
}
