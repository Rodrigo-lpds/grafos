#ifndef LISTA_ADJACENCIA_PESO_ADAPTER_H
#define LISTA_ADJACENCIA_PESO_ADAPTER_H

#include "grafo_peso_interface.h"
#include "lista_adjacencia_peso.h"

class ListaAdjacenciaPesoAdapter : public IGrafoPeso {
private:
    const ListaAdjacenciaPeso& lista;

public:
    explicit ListaAdjacenciaPesoAdapter(const ListaAdjacenciaPeso& l) : lista(l) {}

    int getNumVertices() const override {
        return lista.getLista().size();
    }

    vector<pair<int, double>> getVizinhosComPeso(int vertice) const override {
        const auto& L = lista.getLista();
        const auto& vizinhos_1based = L[vertice];

        vector<pair<int, double>> vizinhos_0based;
        vizinhos_0based.reserve(vizinhos_1based.size());

        for (const auto& [vizinho_1based, peso] : vizinhos_1based) {
            vizinhos_0based.emplace_back(vizinho_1based - 1, peso);
        }

        return vizinhos_0based;
    }

    pair<bool, double> getAresta(int origem, int destino) const override {
        const auto& L = lista.getLista();
        const auto& vizinhos = L[origem];

        int destino_1based = destino + 1;
        for (const auto& [vizinho, peso] : vizinhos) {
            if (vizinho == destino_1based) {
                return {true, peso};
            }
        }
        return {false, 0.0};
    }
};

#endif