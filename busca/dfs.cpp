#include "dfs.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

DFS::DFS(int numVertices) : n(numVertices) {
    marcado.resize(n, false);
    pai.resize(n, -1); // -1 indica que não tem pai (ou é raiz)
    nivel.resize(n, -1); // -1 indica que não foi visitado
    ordem_visitacao.clear();
}

void DFS::executarDFS_Matriz(const MatrizAdjacencia& matriz, int verticeInicial) {
    // Valida entrada
    if (verticeInicial < 1 || verticeInicial > n) {
        throw invalid_argument("Vértice inicial inválido");
    }
    
    reset(); // Limpa estado anterior
    
    // Converte para índice 0-based
    int s = verticeInicial - 1;
    
    // 1. DFS(s)
    // 2. Desmarcar todos os vértices (já feito no reset())
    
    // 3. Definir pilha P com um elemento s
    stack<pair<int, int>> P; // Par (vértice, pai) para rastrear árvore
    P.push({s, -1});
    
    // 4. Enquanto P não estiver vazia
    while (!P.empty()) {
        // 5. Remover u de P // no topo da pilha
        auto [u, pai_u] = P.top();
        P.pop();
        
        // 6. Se u não estiver marcado
        if (!marcado[u]) {
            // 7. Marcar u
            marcado[u] = true;
            pai[u] = pai_u;
            
            // Define nível baseado no pai
            if (pai_u == -1) {
                nivel[u] = 0; // Raiz tem nível 0
            } else {
                nivel[u] = nivel[pai_u] + 1;
            }
            
            ordem_visitacao.push_back(u + 1); // Salva como 1-based
            
            // 8. Para cada aresta (u,v) incidente a u
            // Percorremos em ordem reversa para manter a ordem lexicográfica
            // quando vários vizinhos são adicionados à pilha
            for (int v = n - 1; v >= 0; v--) {
                // Verifica se existe aresta u->v na matriz (matriz[u][v] == 1)
                if (matriz.getMatriz()[u][v] == 1 && !marcado[v]) {
                    // 9. Adicionar v em P // no topo
                    P.push({v, u});
                }
            }
        }
    }
}

void DFS::executarDFS_Lista(const ListaAdjacencia& lista, int verticeInicial) {
    // Valida entrada
    if (verticeInicial < 1 || verticeInicial > n) {
        throw invalid_argument("Vértice inicial inválido");
    }
    
    reset(); // Limpa estado anterior
    
    // Converte para índice 0-based
    int s = verticeInicial - 1;
    
    // 1. DFS(s)
    // 2. Desmarcar todos os vértices (já feito no reset())
    
    // 3. Definir pilha P com um elemento s
    stack<pair<int, int>> P; // Par (vértice, pai) para rastrear árvore
    P.push({s, -1});
    
    // 4. Enquanto P não estiver vazia
    while (!P.empty()) {
        // 5. Remover u de P // no topo da pilha
        auto [u, pai_u] = P.top();
        P.pop();
        
        // 6. Se u não estiver marcado
        if (!marcado[u]) {
            // 7. Marcar u
            marcado[u] = true;
            pai[u] = pai_u;
            
            // Define nível baseado no pai
            if (pai_u == -1) {
                nivel[u] = 0; // Raiz tem nível 0
            } else {
                nivel[u] = nivel[pai_u] + 1;
            }
            
            ordem_visitacao.push_back(u + 1); // Salva como 1-based
            
            // 8. Para cada aresta (u,v) incidente a u
            const list<int>& vizinhos = lista.getLista()[u];
            
            // Percorremos em ordem reversa para manter a ordem lexicográfica
            // quando vários vizinhos são adicionados à pilha
            vector<int> vizinhos_vec(vizinhos.begin(), vizinhos.end());
            for (int i = vizinhos_vec.size() - 1; i >= 0; i--) {
                int v_1based = vizinhos_vec[i];
                int v = v_1based - 1; // Converte para 0-based
                
                if (!marcado[v]) {
                    // 9. Adicionar v em P // no topo
                    P.push({v, u});
                }
            }
        }
    }
}

void DFS::imprimirResultado() const {
    cout << "DFS - Ordem de visitação:\n";
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
    
    cout << "Vértices visitados: ";
    for (int i = 0; i < n; i++) {
        if (marcado[i]) {
            cout << (i + 1) << " ";
        }
    }
    cout << "\n";
    
    cout << "Vértices não visitados: ";
    for (int i = 0; i < n; i++) {
        if (!marcado[i]) {
            cout << (i + 1) << " ";
        }
    }
    cout << "\n";
}

void DFS::salvarResultado(const string& nomeArquivo) const {
    ofstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw runtime_error("Erro ao criar arquivo de resultado");
    }
    
    arquivo << "DFS - Resultado da Busca em Profundidade\n";
    arquivo << "========================================\n\n";
    
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
    
    arquivo << "Vértices visitados: ";
    for (int i = 0; i < n; i++) {
        if (marcado[i]) {
            arquivo << (i + 1) << " ";
        }
    }
    arquivo << "\n";
    
    arquivo << "Vértices não visitados: ";
    for (int i = 0; i < n; i++) {
        if (!marcado[i]) {
            arquivo << (i + 1) << " ";
        }
    }
    arquivo << "\n";
}

void DFS::reset() {
    fill(marcado.begin(), marcado.end(), false);
    fill(pai.begin(), pai.end(), -1);
    fill(nivel.begin(), nivel.end(), -1);
    ordem_visitacao.clear();
}

bool DFS::foiVisitado(int vertice) const {
    if (vertice < 1 || vertice > n) {
        return false;
    }
    return marcado[vertice - 1];
}

const vector<int>& DFS::getOrdemVisitacao() const {
    return ordem_visitacao;
}

const vector<int>& DFS::getPais() const {
    return pai;
}

const vector<int>& DFS::getNiveis() const {
    return nivel;
}

int DFS::getPai(int vertice) const {
    if (vertice < 1 || vertice > n) {
        return -1;
    }
    int paiIndex = pai[vertice - 1];
    return (paiIndex == -1) ? -1 : paiIndex + 1; // Retorna em 1-based ou -1
}

int DFS::getNivel(int vertice) const {
    if (vertice < 1 || vertice > n) {
        return -1;
    }
    return nivel[vertice - 1];
}
