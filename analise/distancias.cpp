#include "distancias.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>

Distancias::Distancias(int numVertices) : n(numVertices) {
    distancias.assign(n, vector<int>(n, INFINITO));
    diametro = 0;
    vertices_diametro = {-1, -1};
    
    // Distância de um vértice para ele mesmo é 0
    for (int i = 0; i < n; i++) {
        distancias[i][i] = 0;
    }
}

vector<int> Distancias::bfs_distancias_matriz(const MatrizAdjacencia& matriz, int origem) {
    BFS bfs(n);
    bfs.executarBFS_Matriz(matriz, origem + 1); // Converte para 1-based
    
    // Converte os níveis da BFS para distâncias
    vector<int> dist(n, INFINITO);
    const vector<int>& niveis = bfs.getNiveis();
    
    for (int i = 0; i < n; i++) {
        if (bfs.foiVisitado(i + 1)) { // Verifica se foi visitado (1-based)
            dist[i] = niveis[i];
        }
    }
    
    return dist;
}

vector<int> Distancias::bfs_distancias_lista(const ListaAdjacencia& lista, int origem) {
    BFS bfs(n);
    bfs.executarBFS_Lista(lista, origem + 1); // Converte para 1-based
    
    // Converte os níveis da BFS para distâncias
    vector<int> dist(n, INFINITO);
    const vector<int>& niveis = bfs.getNiveis();
    
    for (int i = 0; i < n; i++) {
        if (bfs.foiVisitado(i + 1)) { // Verifica se foi visitado (1-based)
            dist[i] = niveis[i];
        }
    }
    
    return dist;
}

int Distancias::calcularDistanciaEspecifica_Matriz(const MatrizAdjacencia& matriz, int origem, int destino) {
    if (origem < 1 || origem > n || destino < 1 || destino > n) {
        return INFINITO;
    }
    
    if (origem == destino) {
        return 0;
    }
    
    // Usa BFS com parada antecipada
    BFS bfs(n);
    bfs.executarBFS_Matriz(matriz, origem, destino);
    
    // Se o destino foi visitado, retorna o nível (distância)
    if (bfs.foiVisitado(destino)) {
        return bfs.getNivel(destino);
    } else {
        return INFINITO;
    }
}

int Distancias::calcularDistanciaEspecifica_Lista(const ListaAdjacencia& lista, int origem, int destino) {
    if (origem < 1 || origem > n || destino < 1 || destino > n) {
        return INFINITO;
    }
    
    if (origem == destino) {
        return 0;
    }
    
    // Usa BFS com parada antecipada
    BFS bfs(n);
    bfs.executarBFS_Lista(lista, origem, destino);
    
    // Se o destino foi visitado, retorna o nível (distância)
    if (bfs.foiVisitado(destino)) {
        return bfs.getNivel(destino);
    } else {
        return INFINITO;
    }
}

void Distancias::calcularDistancias_Matriz(const MatrizAdjacencia& matriz) {
    diametro = 0;
    vertices_diametro = {-1, -1};
    
    // Calcula distâncias de cada vértice para todos os outros usando BFS
    for (int i = 0; i < n; i++) {
        vector<int> dist_de_i = bfs_distancias_matriz(matriz, i);
        
        for (int j = 0; j < n; j++) {
            distancias[i][j] = dist_de_i[j];
            
            // Atualiza diâmetro se encontrar distância maior válida
            if (dist_de_i[j] != INFINITO && dist_de_i[j] > diametro) {
                diametro = dist_de_i[j];
                vertices_diametro = {i + 1, j + 1}; // Converte para 1-based
            }
        }
    }
}

void Distancias::calcularDistancias_Lista(const ListaAdjacencia& lista) {
    diametro = 0;
    vertices_diametro = {-1, -1};
    
    // Calcula distâncias de cada vértice para todos os outros usando BFS
    for (int i = 0; i < n; i++) {
        vector<int> dist_de_i = bfs_distancias_lista(lista, i);
        
        for (int j = 0; j < n; j++) {
            distancias[i][j] = dist_de_i[j];
            
            // Atualiza diâmetro se encontrar distância maior válida
            if (dist_de_i[j] != INFINITO && dist_de_i[j] > diametro) {
                diametro = dist_de_i[j];
                vertices_diametro = {i + 1, j + 1}; // Converte para 1-based
            }
        }
    }
}

int Distancias::getDistancia(int origem, int destino) const {
    if (origem < 1 || origem > n || destino < 1 || destino > n) {
        return INFINITO;
    }
    return distancias[origem - 1][destino - 1];
}

int Distancias::getDiametro() const {
    return diametro;
}

pair<int, int> Distancias::getVerticesDiametro() const {
    return vertices_diametro;
}

void Distancias::imprimirMatrizDistancias() const {
    cout << "Matriz de Distâncias:\n";
    cout << "     ";
    for (int j = 1; j <= n; j++) {
        cout << setw(4) << j;
    }
    cout << "\n";
    
    for (int i = 0; i < n; i++) {
        cout << setw(4) << (i + 1) << " ";
        for (int j = 0; j < n; j++) {
            if (distancias[i][j] == INFINITO) {
                cout << setw(4) << "∞";
            } else {
                cout << setw(4) << distancias[i][j];
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

void Distancias::imprimirDiametro() const {
    cout << "Informações sobre o Diâmetro:\n";
    if (isConexo()) {
        cout << "Diâmetro do grafo: " << diametro << "\n";
        cout << "Vértices que formam o diâmetro: " << vertices_diametro.first 
             << " e " << vertices_diametro.second << "\n";
        cout << "Caminho mais longo (mínimo): " << diametro << " arestas\n";
    } else {
        cout << "Grafo não é conexo - diâmetro é infinito\n";
        cout << "Maior distância finita encontrada: " << diametro << "\n";
        if (vertices_diametro.first != -1) {
            cout << "Entre vértices: " << vertices_diametro.first 
                 << " e " << vertices_diametro.second << "\n";
        }
    }
    cout << "\n";
}

void Distancias::salvarResultado(const string& nomeArquivo) const {
    ofstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw runtime_error("Erro ao criar arquivo de distâncias");
    }
    
    arquivo << "Análise de Distâncias e Diâmetro\n";
    arquivo << "================================\n\n";
    
    arquivo << "Matriz de Distâncias:\n";
    arquivo << "     ";
    for (int j = 1; j <= n; j++) {
        arquivo << setw(4) << j;
    }
    arquivo << "\n";
    
    for (int i = 0; i < n; i++) {
        arquivo << setw(4) << (i + 1) << " ";
        for (int j = 0; j < n; j++) {
            if (distancias[i][j] == INFINITO) {
                arquivo << setw(4) << "∞";
            } else {
                arquivo << setw(4) << distancias[i][j];
            }
        }
        arquivo << "\n";
    }
    arquivo << "\n";
    
    arquivo << "Análise do Diâmetro:\n";
    if (isConexo()) {
        arquivo << "Grafo é conexo\n";
        arquivo << "Diâmetro: " << diametro << "\n";
        arquivo << "Vértices do diâmetro: " << vertices_diametro.first 
                << " e " << vertices_diametro.second << "\n";
        arquivo << "Interpretação: O caminho mínimo mais longo tem " 
                << diametro << " arestas\n";
    } else {
        arquivo << "Grafo não é conexo\n";
        arquivo << "Diâmetro: ∞ (infinito)\n";
        arquivo << "Maior distância finita: " << diametro << "\n";
        if (vertices_diametro.first != -1) {
            arquivo << "Entre vértices: " << vertices_diametro.first 
                    << " e " << vertices_diametro.second << "\n";
        }
    }
    arquivo << "\n";
    
    arquivo << "Estatísticas:\n";
    arquivo << "- Vértices: " << n << "\n";
    arquivo << "- Conexo: " << (isConexo() ? "Sim" : "Não") << "\n";
    
    // Conta quantos pares têm distância finita
    int pares_conectados = 0;
    int total_pares = n * (n - 1); // Exclui diagonal
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && distancias[i][j] != INFINITO) {
                pares_conectados++;
            }
        }
    }
    
    arquivo << "- Pares conectados: " << pares_conectados << "/" << total_pares << "\n";
    arquivo << "- Taxa de conectividade: " 
            << fixed << setprecision(1) 
            << (100.0 * pares_conectados / total_pares) << "%\n";
}

bool Distancias::isConexo() const {
    // Um grafo é conexo se não existem distâncias infinitas (exceto diagonal)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && distancias[i][j] == INFINITO) {
                return false;
            }
        }
    }
    return true;
}

const vector<vector<int>>& Distancias::getMatrizDistancias() const {
    return distancias;
}
