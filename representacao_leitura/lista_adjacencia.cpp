#include "lista_adjacencia.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

ListaAdjacencia::ListaAdjacencia(const DadosGrafo& dados) {
    n = dados.numVertices;
    direcionado = dados.direcionado;
    lista.resize(n);

    for (const auto& aresta : dados.arestas) {
        int u = aresta.first - 1;
        int v = aresta.second - 1;
        
        // Adiciona aresta u -> v
        lista[u].push_back(v + 1);
        
        // Se não é direcionado, adiciona também v -> u
        if (!direcionado) {
            lista[v].push_back(u + 1);
        }
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

    auto listaOrdenada = getListaOrdenada();

    saida << "Lista de Adjacencia (" << n << " vertices, ";
    saida << (direcionado ? "direcionado" : "nao direcionado") << "):\n";
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

    auto listaOrdenada = getListaOrdenada();

    cout << "Lista de Adjacencia (" << n << " vertices, ";
    cout << (direcionado ? "direcionado" : "nao direcionado") << "):\n";
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

bool ListaAdjacencia::isDirecionado() const {
    return direcionado;
}
