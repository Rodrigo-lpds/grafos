#ifndef LISTA_ADJACENCIA_ADAPTER_H
#define LISTA_ADJACENCIA_ADAPTER_H

#include "grafo_interface.h"
#include "lista_adjacencia.h"

/**
 * Adapter para ListaAdjacencia implementar IGrafo.
 * Aplica o Padrão Adapter e o Princípio Aberto/Fechado (SOLID) - 
 * estende funcionalidade sem modificar código existente.
 */
class ListaAdjacenciaAdapter : public IGrafo {
private:
    const ListaAdjacencia& lista;
    
public:
    explicit ListaAdjacenciaAdapter(const ListaAdjacencia& l) : lista(l) {}
    
    int getNumVertices() const override {
        return lista.getLista().size();
    }
    
    vector<int> getVizinhos(int vertice) const override {
        const auto& L = lista.getLista();
        const auto& vizinhos_1based = L[vertice];
        
        // Converte de 1-based para 0-based
        vector<int> vizinhos_0based;
        vizinhos_0based.reserve(vizinhos_1based.size());
        
        for (int v : vizinhos_1based) {
            vizinhos_0based.push_back(v - 1);
        }
        
        return vizinhos_0based;
    }
    
    bool existeAresta(int origem, int destino) const override {
        const auto& L = lista.getLista();
        const auto& vizinhos = L[origem];
        
        // Procura destino+1 (convertendo para 1-based)
        int destino_1based = destino + 1;
        for (int v : vizinhos) {
            if (v == destino_1based) {
                return true;
            }
        }
        return false;
    }
};

#endif