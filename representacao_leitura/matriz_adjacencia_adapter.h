#ifndef MATRIZ_ADJACENCIA_ADAPTER_H
#define MATRIZ_ADJACENCIA_ADAPTER_H

#include "grafo_interface.h"
#include "matriz_adjacencia.h"

/**
 * Adapter para MatrizAdjacencia implementar IGrafo.
 * Aplica o Padrão Adapter e o Princípio Aberto/Fechado (SOLID) - 
 * estende funcionalidade sem modificar código existente.
 */
class MatrizAdjacenciaAdapter : public IGrafo {
private:
    const MatrizAdjacencia& matriz;
    
public:
    explicit MatrizAdjacenciaAdapter(const MatrizAdjacencia& m) : matriz(m) {}
    
    int getNumVertices() const override {
        return matriz.getMatriz().size();
    }
    
    vector<int> getVizinhos(int vertice) const override {
        vector<int> vizinhos;
        const auto& M = matriz.getMatriz();
        const auto& linha = M[vertice];
        
        for (int i = 0; i < static_cast<int>(linha.size()); i++) {
            if (linha[i] == 1) {
                vizinhos.push_back(i);
            }
        }
        
        return vizinhos;
    }
    
    bool existeAresta(int origem, int destino) const override {
        const auto& M = matriz.getMatriz();
        return M[origem][destino] == 1;
    }
};

#endif