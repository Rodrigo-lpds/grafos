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

int Distancias::calcularDistanciaEspecifica_Matriz(const MatrizAdjacencia& matriz, int origem, int destino) const {
    if (origem < 1 || origem > n || destino < 1 || destino > n) {
        return INFINITO;
    }

    if (origem == destino) {
        return 0;
    }

    BFS bfs(n);
    bfs.executarBFS_Matriz(matriz, origem, destino);

    if (bfs.foiVisitado(destino)) {
        return bfs.getNivel(destino);
    } else {
        return INFINITO;
    }
}

int Distancias::calcularDistanciaEspecifica_Lista(const ListaAdjacencia& lista, int origem, int destino) const {
    if (origem < 1 || origem > n || destino < 1 || destino > n) {
        return INFINITO;
    }

    if (origem == destino) {
        return 0;
    }

    BFS bfs(n);
    bfs.executarBFS_Lista(lista, origem, destino);

    if (bfs.foiVisitado(destino)) {
        return bfs.getNivel(destino);
    } else {
        return INFINITO;
    }
}

void Distancias::calcularDistancias_Matriz(const MatrizAdjacencia& matriz) {
    diametro = 0;
    vertices_diametro = {-1, -1};

    if (n > 500) {
        cout << "🔄 Grafo grande detectado (" << n << " vértices) - usando algoritmo aproximado para matriz\n";

        bool grafoConexo = isConexo_Matriz(matriz);

        int numAmostras = n <= 1000 ? min(50, n / 4) : min(100, n / 10);
        vector<int> vertices_amostra = selecionarVerticesAmostra(numAmostras);

        cout << "🚀 Calculando diâmetro aproximado usando " << vertices_amostra.size()
             << " vértices de " << n << "\n";

        for (int origem : vertices_amostra) {
            vector<int> dist_de_origem = bfs_distancias_matriz(matriz, origem);

            for (int j = 0; j < n; j++) {
                if (dist_de_origem[j] != INFINITO && dist_de_origem[j] > diametro) {
                    diametro = dist_de_origem[j];
                    vertices_diametro = {origem + 1, j + 1};
                }
            }
        }

        cout << "✅ Diâmetro aproximado calculado: " << diametro << "\n";
        return;
    }

    cout << "🔄 Calculando diâmetro exato (" << n << " vértices)...\n";

    for (int i = 0; i < n; i++) {
        vector<int> dist_de_i = bfs_distancias_matriz(matriz, i);

        for (int j = 0; j < n; j++) {

            if (dist_de_i[j] != INFINITO && dist_de_i[j] > diametro) {
                diametro = dist_de_i[j];
                vertices_diametro = {i + 1, j + 1};
            }
        }
    }

    cout << "✅ Diâmetro exato calculado: " << diametro << "\n";
}

void Distancias::calcularDistancias_Lista(const ListaAdjacencia& lista) {

    calcularDiametroAproximado_Lista(lista);
}

int Distancias::getDistancia_Lista(const ListaAdjacencia& lista, int origem, int destino) const {
    return calcularDistanciaEspecifica_Lista(lista, origem, destino);
}

int Distancias::getDistancia_Matriz(const MatrizAdjacencia& matriz, int origem, int destino) const {
    return calcularDistanciaEspecifica_Matriz(matriz, origem, destino);
}

int Distancias::getDiametro() const {
    return diametro;
}

pair<int, int> Distancias::getVerticesDiametro() const {
    return vertices_diametro;
}

void Distancias::imprimirMatrizDistancias() const {
    cout << "⚠️  Matriz de distâncias não é mais armazenada para otimização de memória.\n";
    cout << "💡 Use getDistancia_Lista() ou getDistancia_Matriz() para consultas específicas.\n";
    cout << "   Informações disponíveis:\n";
    cout << "   • Diâmetro: " << diametro << "\n";
    cout << "   • Vértices do diâmetro: " << vertices_diametro.first
         << " e " << vertices_diametro.second << "\n\n";
}

void Distancias::imprimirDiametro() const {
    cout << "Informações sobre o Diâmetro:\n";
    cout << "Diâmetro calculado: " << diametro << "\n";
    cout << "Vértices que formam o diâmetro: " << vertices_diametro.first
         << " e " << vertices_diametro.second << "\n";
    cout << "💡 Para verificar conectividade, use isConexo_Lista() ou isConexo_Matriz()\n";
    cout << "\n";
}

void Distancias::salvarResultado(const string& nomeArquivo) const {
    ofstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw runtime_error("Erro ao criar arquivo de distâncias");
    }

    arquivo << "Análise de Distâncias e Diâmetro (Versão Otimizada)\n";
    arquivo << "===================================================\n\n";

    arquivo << "NOTA: Esta versão otimizada não armazena matriz completa de distâncias\n";
    arquivo << "      para economizar memória. Use funções específicas para consultas.\n\n";

    arquivo << "Análise do Diâmetro:\n";
    arquivo << "- Diâmetro calculado: " << diametro << "\n";
    arquivo << "- Vértices do diâmetro: " << vertices_diametro.first
            << " e " << vertices_diametro.second << "\n";
    arquivo << "- Interpretação: O caminho mínimo mais longo encontrado tem "
            << diametro << " arestas\n\n";

    arquivo << "Estatísticas Básicas:\n";
    arquivo << "- Vértices: " << n << "\n";
    arquivo << "- Algoritmo: Otimizado com amostragem para grafos grandes\n";
    arquivo << "- Memória: O(1) para diâmetro vs O(V²) para matriz completa\n\n";

    arquivo << "Para análise detalhada de conectividade ou distâncias específicas,\n";
    arquivo << "use as funções apropriadas da classe Distancias.\n";
}

bool Distancias::isConexo_Lista(const ListaAdjacencia& lista) const {
    return verificarConectividade_Lista(lista);
}

bool Distancias::isConexo_Matriz(const MatrizAdjacencia& matriz) const {

    BFS bfs(n);
    bfs.executarBFS_Matriz(matriz, 1);

    for (int i = 1; i <= n; i++) {
        if (!bfs.foiVisitado(i)) {
            return false;
        }
    }
    return true;
}

vector<int> Distancias::selecionarVerticesAmostra(int numAmostras) const {
    vector<int> vertices;

    if (n <= numAmostras) {
        for (int i = 0; i < n; i++) {
            vertices.push_back(i);
        }
        return vertices;
    }

    int intervalo = n / numAmostras;
    for (int i = 0; i < numAmostras; i++) {
        vertices.push_back((i * intervalo) % n);
    }

    if (numAmostras < n / 2) {
        vertices.push_back(0);
        vertices.push_back(n - 1);
        vertices.push_back(n / 2);
    }

    return vertices;
}

bool Distancias::verificarConectividade_Lista(const ListaAdjacencia& lista) const {

    BFS bfs(n);
    bfs.executarBFS_Lista(lista, 1);

    for (int i = 1; i <= n; i++) {
        if (!bfs.foiVisitado(i)) {
            return false;
        }
    }
    return true;
}

void Distancias::calcularDiametroAproximado_Lista(const ListaAdjacencia& lista) {
    diametro = 0;
    vertices_diametro = {-1, -1};

    bool grafoConexo = verificarConectividade_Lista(lista);

    int numAmostras;
    if (n <= 100) {
        numAmostras = n;
    } else if (n <= 1000) {
        numAmostras = min(50, n / 4);
    } else {
        numAmostras = min(100, n / 10);
    }

    vector<int> vertices_amostra = selecionarVerticesAmostra(numAmostras);

    cout << "🚀 Calculando diâmetro aproximado usando " << vertices_amostra.size()
         << " vértices de " << n << " (" << (100.0 * vertices_amostra.size() / n)
         << "% amostra)\n";

    if (!grafoConexo) {
        cout << "⚠️  Grafo não é conexo - calculando maior distância finita\n";

        for (int origem : vertices_amostra) {
            vector<int> dist_de_origem = bfs_distancias_lista(lista, origem);

            for (int j = 0; j < n; j++) {
                if (dist_de_origem[j] != INFINITO && dist_de_origem[j] > diametro) {
                    diametro = dist_de_origem[j];
                    vertices_diametro = {origem + 1, j + 1};
                }
            }
        }
    } else {

        for (int origem : vertices_amostra) {
            vector<int> dist_de_origem = bfs_distancias_lista(lista, origem);

            vector<int> destinos_amostra = selecionarVerticesAmostra(numAmostras);

            for (int destino : destinos_amostra) {
                if (origem != destino && dist_de_origem[destino] != INFINITO &&
                    dist_de_origem[destino] > diametro) {
                    diametro = dist_de_origem[destino];
                    vertices_diametro = {origem + 1, destino + 1};
                }
            }

            auto maxDist = max_element(dist_de_origem.begin(), dist_de_origem.end());
            if (maxDist != dist_de_origem.end() && *maxDist != INFINITO && *maxDist > diametro) {
                int maxIndex = maxDist - dist_de_origem.begin();
                diametro = *maxDist;
                vertices_diametro = {origem + 1, maxIndex + 1};
            }
        }
    }

    cout << "✅ Diâmetro aproximado calculado: " << diametro
         << " (vértices " << vertices_diametro.first << " e " << vertices_diametro.second << ")\n";
}

void Distancias::calcularDiametroApenas_Lista(const ListaAdjacencia& lista) {

    calcularDiametroAproximado_Lista(lista);
}
