#include "bfs.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

BFS::BFS(int numVertices) : n(numVertices) {
    marcado.resize(n, false);
    pai.resize(n, -1); // -1 indica que não tem pai (ou é raiz)
    nivel.resize(n, -1); // -1 indica que não foi visitado
    ordem_visitacao.clear();
}

void BFS::executarBFS_Matriz(const MatrizAdjacencia& matriz, int verticeInicial, int destino) {
    // Valida entrada
    if (verticeInicial < 1 || verticeInicial > n) {
        throw invalid_argument("Vértice inicial inválido");
    }
    
    if (destino != -1 && (destino < 1 || destino > n)) {
        throw invalid_argument("Vértice destino inválido");
    }
    
    reset(); // Limpa estado anterior
    
    // Converte para índice 0-based
    int s = verticeInicial - 1;
    int d = (destino == -1) ? -1 : destino - 1;
    
    // Definir fila Q vazia
    queue<int> Q;
    
    // Marcar s e inserir s na fila Q
    marcado[s] = true;
    nivel[s] = 0; // Raiz tem nível 0
    pai[s] = -1; // Raiz não tem pai
    Q.push(s);
    ordem_visitacao.push_back(s + 1); // Salva como 1-based
    
    // Se origem é igual ao destino, já terminou
    if (d != -1 && s == d) {
        return;
    }
    
    // Enquanto Q não estiver vazia
    while (!Q.empty()) {
        // Retirar v de Q
        int v = Q.front();
        Q.pop();
        
        // Para todo vizinho w de v faça
        for (int w = 0; w < n; w++) {
            // Verifica se existe aresta v->w na matriz (matriz[v][w] == 1)
            if (matriz.getMatriz()[v][w] == 1) {
                // Se w não estiver marcado
                if (!marcado[w]) {
                    // marcar w
                    marcado[w] = true;
                    nivel[w] = nivel[v] + 1; // Nível do filho = nível do pai + 1
                    pai[w] = v; // Define o pai na árvore de busca
                    // inserir w em Q
                    Q.push(w);
                    ordem_visitacao.push_back(w + 1); // Salva como 1-based
                    
                    // Se encontrou o destino, pode parar (apenas se destino foi especificado)
                    if (d != -1 && w == d) {
                        return;
                    }
                }
            }
        }
    }
}

void BFS::executarBFS_Lista(const ListaAdjacencia& lista, int verticeInicial, int destino) {
    // Valida entrada
    if (verticeInicial < 1 || verticeInicial > n) {
        throw invalid_argument("Vértice inicial inválido");
    }
    
    if (destino != -1 && (destino < 1 || destino > n)) {
        throw invalid_argument("Vértice destino inválido");
    }
    
    reset(); // Limpa estado anterior
    
    // Converte para índice 0-based
    int s = verticeInicial - 1;
    int d = (destino == -1) ? -1 : destino - 1;
    
    // Definir fila Q vazia
    queue<int> Q;
    
    // Marcar s e inserir s na fila Q
    marcado[s] = true;
    nivel[s] = 0; // Raiz tem nível 0
    pai[s] = -1; // Raiz não tem pai
    Q.push(s);
    ordem_visitacao.push_back(s + 1); // Salva como 1-based
    
    // Se origem é igual ao destino, já terminou
    if (d != -1 && s == d) {
        return;
    }
    
    // Enquanto Q não estiver vazia
    while (!Q.empty()) {
        // Retirar v de Q
        int v = Q.front();
        Q.pop();
        
        // Para todo vizinho w de v faça
        const list<int>& vizinhos = lista.getLista()[v];
        for (int w_1based : vizinhos) {
            int w = w_1based - 1; // Converte para 0-based
            
            // Se w não estiver marcado
            if (!marcado[w]) {
                // marcar w
                marcado[w] = true;
                nivel[w] = nivel[v] + 1; // Nível do filho = nível do pai + 1
                pai[w] = v; // Define o pai na árvore de busca
                // inserir w em Q
                Q.push(w);
                ordem_visitacao.push_back(w + 1); // Salva como 1-based
                
                // Se encontrou o destino, pode parar (apenas se destino foi especificado)
                if (d != -1 && w == d) {
                    return;
                }
            }
        }
    }
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
    return (paiIndex == -1) ? -1 : paiIndex + 1; // Retorna em 1-based ou -1
}

int BFS::getNivel(int vertice) const {
    if (vertice < 1 || vertice > n) {
        return -1;
    }
    return nivel[vertice - 1];
}
