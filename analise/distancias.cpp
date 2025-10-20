#include "distancias.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>

Distancias::Distancias(int numVertices) : n(numVertices) {
    diametro = 0;
    vertices_diametro = {-1, -1};
}

vector<int> Distancias::bfs_distancias_matriz(const MatrizAdjacencia& matriz, int origem) {
    BFS bfs(n);
    bfs.executarBFS_Matriz(matriz, origem + 1);

    vector<int> dist(n, INFINITO);
    const vector<int>& niveis = bfs.getNiveis();

    for (int i = 0; i < n; i++) {
        if (bfs.foiVisitado(i + 1)) {
            dist[i] = niveis[i];
        }
    }

    return dist;
}

vector<int> Distancias::bfs_distancias_lista(const ListaAdjacencia& lista, int origem) {
    BFS bfs(n);
    bfs.executarBFS_Lista(lista, origem + 1);

    vector<int> dist(n, INFINITO);
    const vector<int>& niveis = bfs.getNiveis();

    for (int i = 0; i < n; i++) {
        if (bfs.foiVisitado(i + 1)) {
            dist[i] = niveis[i];
        }
    }

    return dist;
}

// Implementações dos métodos genéricos usando interface IGrafo

vector<int> Distancias::bfs_distancias(const IGrafo& grafo, int origem) {
    vector<int> distancias(n, INFINITO);
    vector<bool> visitado(n, false);
    queue<int> fila;

    // Convertendo origem de base 1 para base 0
    int origem_0based = origem - 1;
    
    fila.push(origem_0based);
    distancias[origem_0based] = 0;
    visitado[origem_0based] = true;

    while (!fila.empty()) {
        int atual = fila.front();
        fila.pop();

        vector<int> vizinhos = grafo.getVizinhos(atual);
        for (int vizinho : vizinhos) {
            if (!visitado[vizinho]) {
                visitado[vizinho] = true;
                distancias[vizinho] = distancias[atual] + 1;
                fila.push(vizinho);
            }
        }
    }

    return distancias;
}

void Distancias::calcularDistancias(const IGrafo& grafo) {
    cout << "🔄 Calculando matriz de distâncias...\n";
    
    diametro = 0;
    vertices_diametro = {1, 1};

    for (int origem = 1; origem <= n; origem++) {
        vector<int> dist_de_origem = bfs_distancias(grafo, origem);

        auto maxDist = max_element(dist_de_origem.begin(), dist_de_origem.end(),
            [](int a, int b) { 
                if (a == INFINITO) return true;
                if (b == INFINITO) return false;
                return a < b; 
            });

        if (maxDist != dist_de_origem.end() && *maxDist != INFINITO && *maxDist > diametro) {
            int maxIndex = maxDist - dist_de_origem.begin();
            diametro = *maxDist;
            vertices_diametro = {origem, maxIndex + 1}; // Convertendo de volta para base 1
        }
    }

    cout << "✅ Diâmetro calculado: " << diametro 
         << " (vértices " << vertices_diametro.first << " e " << vertices_diametro.second << ")\n";
}

int Distancias::calcularDistanciaEspecifica(const IGrafo& grafo, int origem, int destino) const {
    vector<int> distancias = const_cast<Distancias*>(this)->bfs_distancias(grafo, origem);
    
    // Convertendo destino de base 1 para base 0
    int destino_0based = destino - 1;
    
    if (destino_0based >= 0 && destino_0based < n) {
        return distancias[destino_0based];
    }
    return INFINITO;
}

int Distancias::getDistancia(const IGrafo& grafo, int origem, int destino) const {
    return calcularDistanciaEspecifica(grafo, origem, destino);
}

int Distancias::getDiametro() const {
    return diametro;
}

pair<int, int> Distancias::getVerticesDiametro() const {
    return vertices_diametro;
}
