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
    // Para grafos grandes (>500 vértices), usa algoritmo otimizado por padrão
    if (n > 500) {
        cout << "🔄 Grafo grande detectado (" << n << " vértices) - usando algoritmo otimizado\n";
        cout << "💡 Para matriz completa de distâncias, use um grafo menor ou implemente versão otimizada específica\n";
        
        // Calcula apenas o diâmetro sem matriz completa
        calcularDiametroAproximado_Lista(lista);
        return;
    }
    
    // Para grafos menores, usa algoritmo completo original
    diametro = 0;
    vertices_diametro = {-1, -1};
    
    cout << "🔄 Calculando matriz completa de distâncias (" << n << " vértices)...\n";
    
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
    
    cout << "✅ Matriz de distâncias completa calculada\n";
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

// IMPLEMENTAÇÕES OTIMIZADAS PARA CÁLCULO DO DIÂMETRO

vector<int> Distancias::selecionarVerticesAmostra(int numAmostras) const {
    vector<int> vertices;
    
    // Se o grafo é pequeno, usa todos os vértices
    if (n <= numAmostras) {
        for (int i = 0; i < n; i++) {
            vertices.push_back(i);
        }
        return vertices;
    }
    
    // Para grafos maiores, seleciona amostra distribuída
    int intervalo = n / numAmostras;
    for (int i = 0; i < numAmostras; i++) {
        vertices.push_back((i * intervalo) % n);
    }
    
    // Adiciona alguns vértices aleatórios extras
    if (numAmostras < n / 2) {
        vertices.push_back(0);           // Primeiro vértice
        vertices.push_back(n - 1);       // Último vértice
        vertices.push_back(n / 2);       // Vértice do meio
    }
    
    return vertices;
}

bool Distancias::verificarConectividade_Lista(const ListaAdjacencia& lista) {
    // Executa BFS do vértice 0 para verificar se alcança todos
    BFS bfs(n);
    bfs.executarBFS_Lista(lista, 1); // Vértice 1 (1-based)
    
    // Verifica se todos os vértices foram visitados
    for (int i = 1; i <= n; i++) {
        if (!bfs.foiVisitado(i)) {
            return false; // Grafo não é conexo
        }
    }
    return true; // Grafo é conexo
}

void Distancias::calcularDiametroAproximado_Lista(const ListaAdjacencia& lista) {
    diametro = 0;
    vertices_diametro = {-1, -1};
    
    // Primeiro verifica se o grafo é conexo (parada antecipada)
    bool grafoConexo = verificarConectividade_Lista(lista);
    
    // Define número de amostras baseado no tamanho do grafo
    int numAmostras;
    if (n <= 100) {
        numAmostras = n; // Grafo pequeno: usa todos os vértices
    } else if (n <= 1000) {
        numAmostras = min(50, n / 4); // Grafo médio: ~25% dos vértices
    } else {
        numAmostras = min(100, n / 10); // Grafo grande: ~10% dos vértices
    }
    
    vector<int> vertices_amostra = selecionarVerticesAmostra(numAmostras);
    
    cout << "🚀 Calculando diâmetro aproximado usando " << vertices_amostra.size() 
         << " vértices de " << n << " (" << (100.0 * vertices_amostra.size() / n) 
         << "% amostra)\n";
    
    // Para grafos não conexos, usa estratégia diferente
    if (!grafoConexo) {
        cout << "⚠️  Grafo não é conexo - calculando maior distância finita\n";
        
        // Para grafos não conexos, testa mais vértices para encontrar 
        // a maior distância finita entre componentes
        for (int origem : vertices_amostra) {
            vector<int> dist_de_origem = bfs_distancias_lista(lista, origem);
            
            for (int j = 0; j < n; j++) {
                if (dist_de_origem[j] != INFINITO && dist_de_origem[j] > diametro) {
                    diametro = dist_de_origem[j];
                    vertices_diametro = {origem + 1, j + 1}; // Converte para 1-based
                }
            }
        }
    } else {
        // Para grafos conexos, usa amostragem mais agressiva
        for (int origem : vertices_amostra) {
            vector<int> dist_de_origem = bfs_distancias_lista(lista, origem);
            
            // Para grafos conexos, pode amostrar também os destinos
            vector<int> destinos_amostra = selecionarVerticesAmostra(numAmostras);
            
            for (int destino : destinos_amostra) {
                if (origem != destino && dist_de_origem[destino] != INFINITO && 
                    dist_de_origem[destino] > diametro) {
                    diametro = dist_de_origem[destino];
                    vertices_diametro = {origem + 1, destino + 1}; // Converte para 1-based
                }
            }
            
            // Para o vértice com maior distância, testa todos os destinos
            auto maxDist = max_element(dist_de_origem.begin(), dist_de_origem.end());
            if (maxDist != dist_de_origem.end() && *maxDist != INFINITO && *maxDist > diametro) {
                int maxIndex = maxDist - dist_de_origem.begin();
                diametro = *maxDist;
                vertices_diametro = {origem + 1, maxIndex + 1}; // Converte para 1-based
            }
        }
    }
    
    cout << "✅ Diâmetro aproximado calculado: " << diametro 
         << " (vértices " << vertices_diametro.first << " e " << vertices_diametro.second << ")\n";
}

void Distancias::calcularDiametroApenas_Lista(const ListaAdjacencia& lista) {
    // Não aloca a matriz completa de distâncias - apenas calcula o diâmetro
    calcularDiametroAproximado_Lista(lista);
}
