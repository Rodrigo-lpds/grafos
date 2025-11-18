#include "lista_adjacencia_peso.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

ListaAdjacenciaPeso::ListaAdjacenciaPeso(const DadosGrafoPeso& dados) {
    n = dados.numVertices;
    direcionado = dados.direcionado;
    lista.resize(n);

    for (const auto& aresta : dados.arestas) {
        int u = get<0>(aresta) - 1;
        int v = get<1>(aresta) - 1;
        double peso = get<2>(aresta);

        // Adiciona aresta u -> v
        lista[u].push_back({v + 1, peso});
        
        // Se não é direcionado, adiciona também v -> u
        if (!direcionado) {
            lista[v].push_back({u + 1, peso});
        }
    }
}

vector<vector<pair<int, double>>> ListaAdjacenciaPeso::getListaOrdenada() const {
    vector<vector<pair<int, double>>> listaOrdenada = lista;
    for (int i = 0; i < n; i++) {
        sort(listaOrdenada[i].begin(), listaOrdenada[i].end());
    }
    return listaOrdenada;
}

void ListaAdjacenciaPeso::salvarEmArquivo(const string& nomeSaida) const {
    ofstream saida(nomeSaida);
    if (!saida) {
        throw runtime_error("Erro ao criar arquivo de saída");
    }

    auto listaOrdenada = getListaOrdenada();

    saida << "Lista de Adjacencia com Peso (" << n << " vertices, ";
    saida << (direcionado ? "direcionado" : "nao direcionado") << "):\n";
    saida << fixed << setprecision(1);

    for (int i = 0; i < n; i++) {
        saida << "Vertice " << (i + 1) << ": ";
        if (listaOrdenada[i].empty()) {
            saida << "(sem vizinhos)";
        } else {
            bool primeiro = true;
            for (const auto& vizinho : listaOrdenada[i]) {
                if (!primeiro) saida << ", ";
                saida << vizinho.first << " (peso: " << vizinho.second << ")";
                primeiro = false;
            }
        }
        saida << "\n";
    }
}

void ListaAdjacenciaPeso::imprimir() const {

    auto listaOrdenada = getListaOrdenada();

    cout << "Lista de Adjacencia com Peso (" << n << " vertices, ";
    cout << (direcionado ? "direcionado" : "nao direcionado") << "):\n";
    cout << fixed << setprecision(1);

    for (int i = 0; i < n; i++) {
        cout << "Vertice " << (i + 1) << ": ";
        if (listaOrdenada[i].empty()) {
            cout << "(sem vizinhos)";
        } else {
            bool primeiro = true;
            for (const auto& vizinho : listaOrdenada[i]) {
                if (!primeiro) cout << ", ";
                cout << vizinho.first << " (peso: " << vizinho.second << ")";
                primeiro = false;
            }
        }
        cout << "\n";
    }
}

const vector<vector<pair<int, double>>>& ListaAdjacenciaPeso::getLista() const {
    return lista;
}

int ListaAdjacenciaPeso::getNumVertices() const {
    return n;
}

bool ListaAdjacenciaPeso::isDirecionado() const {
    return direcionado;
}