#include "bfs.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

BFS::BFS(int numVertices) : n(numVertices) {
    marcado.resize(n, false);
    pai.resize(n, -1);
    nivel.resize(n, -1);
    ordem_visitacao.clear();
}

void BFS::executarBFS(const IGrafo& grafo, int verticeInicial, int destino) {

    if (verticeInicial < 1 || verticeInicial > n) {
        throw invalid_argument("Vértice inicial inválido");
    }

    if (destino != -1 && (destino < 1 || destino > n)) {
        throw invalid_argument("Vértice destino inválido");
    }

    reset();

    int s = verticeInicial - 1;
    int d = (destino == -1) ? -1 : destino - 1;

    queue<int> Q;

    marcado[s] = true;
    nivel[s] = 0;
    pai[s] = -1;
    Q.push(s);
    ordem_visitacao.push_back(s + 1);

    if (d != -1 && s == d) {
        return;
    }

    while (!Q.empty()) {

        int v = Q.front();
        Q.pop();

        vector<int> vizinhos = grafo.getVizinhos(v);

        for (int w : vizinhos) {

            if (!marcado[w]) {

                marcado[w] = true;
                nivel[w] = nivel[v] + 1;
                pai[w] = v;

                Q.push(w);
                ordem_visitacao.push_back(w + 1);

                if (d != -1 && w == d) {
                    return;
                }
            }
        }
    }
}

void BFS::executarBFS_Matriz(const MatrizAdjacencia& matriz, int verticeInicial, int destino) {
    MatrizAdjacenciaAdapter adapter(matriz);
    executarBFS(adapter, verticeInicial, destino);
}

void BFS::executarBFS_Lista(const ListaAdjacencia& lista, int verticeInicial, int destino) {
    ListaAdjacenciaAdapter adapter(lista);
    executarBFS(adapter, verticeInicial, destino);
}

void BFS::imprimirResultado() const {
    cout << "BFS - Ordem de visitação:\n";
    if (ordem_visitacao.empty()) {
        cout << "Nenhum vértice foi visitado.\n";
        return;
    }

    for (size_t i = 0; i < ordem_visitacao.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << ordem_visitacao[i];
    }
    cout << "\n\n";

    cout << "Árvore de Busca (Pai | Nível):\n";
    for (int i = 0; i < n; i++) {
        if (marcado[i]) {
            cout << "Vértice " << (i + 1) << ": ";
            if (pai[i] == -1) {
                cout << "Raiz | Nível " << nivel[i];
            } else {
                cout << "Pai " << (pai[i] + 1) << " | Nível " << nivel[i];
            }
            cout << "\n";
        }
    }
    cout << "\n";
}

void BFS::salvarResultado(const string& nomeArquivo) const {
    ofstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw runtime_error("Erro ao criar arquivo de resultado");
    }

    arquivo << "BFS - Resultado da Busca em Largura\n";
    arquivo << "=====================================\n\n";

    arquivo << "Ordem de visitação:\n";
    if (ordem_visitacao.empty()) {
        arquivo << "Nenhum vértice foi visitado.\n";
        return;
    }

    for (size_t i = 0; i < ordem_visitacao.size(); i++) {
        if (i > 0) arquivo << " -> ";
        arquivo << ordem_visitacao[i];
    }
    arquivo << "\n\n";

    arquivo << "Árvore de Busca:\n";
    arquivo << "Vértice | Pai | Nível\n";
    arquivo << "--------|-----|-------\n";
    for (int i = 0; i < n; i++) {
        if (marcado[i]) {
            arquivo << "   " << (i + 1) << "    | ";
            if (pai[i] == -1) {
                arquivo << " - ";
            } else {
                arquivo << " " << (pai[i] + 1) << " ";
            }
            arquivo << " |   " << nivel[i] << "\n";
        }
    }
    arquivo << "\n";
}

void BFS::reset() {
    fill(marcado.begin(), marcado.end(), false);
    fill(pai.begin(), pai.end(), -1);
    fill(nivel.begin(), nivel.end(), -1);
    ordem_visitacao.clear();
}

bool BFS::foiVisitado(int vertice) const {
    if (vertice < 1 || vertice > n) {
        return false;
    }
    return marcado[vertice - 1];
}

const vector<int>& BFS::getOrdemVisitacao() const {
    return ordem_visitacao;
}

const vector<int>& BFS::getPais() const {
    return pai;
}

const vector<int>& BFS::getNiveis() const {
    return nivel;
}

int BFS::getPai(int vertice) const {
    if (vertice < 1 || vertice > n) {
        return -1;
    }
    int paiIndex = pai[vertice - 1];
    return (paiIndex == -1) ? -1 : paiIndex + 1;
}

int BFS::getNivel(int vertice) const {
    if (vertice < 1 || vertice > n) {
        return -1;
    }
    return nivel[vertice - 1];
}
