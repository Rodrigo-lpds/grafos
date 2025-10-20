#include "estatisticas.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <numeric>

Estatisticas::Estatisticas(int numVertices) : n(numVertices) {
    stats.numVertices = numVertices;
    stats.numArestas = 0;
    stats.grauMinimo = 0;
    stats.grauMaximo = 0;
    stats.grauMedio = 0.0;
    stats.medianaGrau = 0.0;
    stats.numComponentes = 0;
    stats.isConexo = false;
    stats.maiorComponente = 0;
    stats.menorComponente = 0;
    stats.diametro = 0;
    stats.verticesDiametro = {-1, -1};
}

vector<int> Estatisticas::calcularGraus_Lista(const ListaAdjacencia& lista) {
    vector<int> graus(n, 0);
    const auto& adj = lista.getLista();

    for (int i = 0; i < n; i++) {
        graus[i] = adj[i].size();
    }

    return graus;
}

void Estatisticas::calcularEstatisticasGrau(const vector<int>& graus) {
    stats.graus = graus;

    auto minMax = minmax_element(graus.begin(), graus.end());
    stats.grauMinimo = *minMax.first;
    stats.grauMaximo = *minMax.second;

    int somaGraus = accumulate(graus.begin(), graus.end(), 0);
    stats.grauMedio = static_cast<double>(somaGraus) / n;

    stats.medianaGrau = calcularMediana(graus);
}

double Estatisticas::calcularMediana(const vector<int>& graus) {
    if (graus.empty()) return 0.0;

    vector<int> valores = graus;
    size_t tamanho = valores.size();

    if (tamanho % 2 == 0) {

        nth_element(valores.begin(), valores.begin() + tamanho/2 - 1, valores.end());
        int primeiro = valores[tamanho/2 - 1];

        nth_element(valores.begin(), valores.begin() + tamanho/2, valores.end());
        int segundo = valores[tamanho/2];

        return (primeiro + segundo) / 2.0;
    } else {

        nth_element(valores.begin(), valores.begin() + tamanho/2, valores.end());
        return valores[tamanho/2];
    }
}

void Estatisticas::calcularEstatisticas_Lista(const ListaAdjacencia& lista, int numArestas) {
    stats.numArestas = numArestas;
    vector<int> graus = calcularGraus_Lista(lista);
    calcularEstatisticasGrau(graus);
    calcularDiametro_Lista(lista);
}

void Estatisticas::adicionarInformacoesComponentes(const ComponentesConexas& componentes) {
    stats.numComponentes = componentes.getNumComponentes();
    stats.isConexo = (stats.numComponentes == 1);
    stats.tamanhosComponentes.clear();

    const auto& comps = componentes.getComponentes();
    for (const auto& comp : comps) {
        stats.tamanhosComponentes.push_back(comp.tamanho);
    }

    if (!stats.tamanhosComponentes.empty()) {
        auto minMax = minmax_element(stats.tamanhosComponentes.begin(), stats.tamanhosComponentes.end());
        stats.menorComponente = *minMax.first;
        stats.maiorComponente = *minMax.second;
    }
}

void Estatisticas::imprimirEstatisticas() const {
    cout << "📊 ESTATÍSTICAS DO GRAFO\n";
    cout << "========================\n";
    cout << "• Número de vértices: " << stats.numVertices << "\n";
    cout << "• Número de arestas: " << stats.numArestas << "\n";
    cout << "• Grau mínimo: " << stats.grauMinimo << "\n";
    cout << "• Grau máximo: " << stats.grauMaximo << "\n";
    cout << "• Grau médio: " << fixed << setprecision(2) << stats.grauMedio << "\n";
    cout << "• Mediana de grau: " << fixed << setprecision(1) << stats.medianaGrau << "\n";
    cout << "\n";

    cout << "🔗 COMPONENTES CONEXAS\n";
    cout << "======================\n";
    cout << "• Número de componentes: " << stats.numComponentes << "\n";

    if (stats.numComponentes == 1) {
        cout << "• O grafo é conexo\n";
    } else {
        cout << "• O grafo não é conexo\n";
        cout << "• Maior componente: " << stats.maiorComponente << " vértices\n";
        cout << "• Menor componente: " << stats.menorComponente << " vértices\n";
        cout << "• Tamanhos das componentes: ";
        for (size_t i = 0; i < stats.tamanhosComponentes.size(); i++) {
            if (i > 0) cout << ", ";
            cout << stats.tamanhosComponentes[i];
        }
        cout << "\n";
    }
    cout << "\n";

    cout << "🎯 DIÂMETRO\n";
    cout << "============\n";
    if (stats.isConexo) {
        cout << "• Diâmetro: " << stats.diametro << "\n";
        cout << "• Vértices do diâmetro: " << stats.verticesDiametro.first
             << " e " << stats.verticesDiametro.second << "\n";
    } else {
        cout << "• Grafo não é conexo - diâmetro infinito\n";
        cout << "• Maior distância finita: " << stats.diametro << "\n";
        if (stats.verticesDiametro.first != -1) {
            cout << "• Vértices: " << stats.verticesDiametro.first
                 << " e " << stats.verticesDiametro.second << "\n";
        }
    }
    cout << "\n";
}

void Estatisticas::salvarRelatorio(const string& nomeArquivo) const {
    ofstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw runtime_error("Erro ao criar arquivo de estatísticas");
    }

    arquivo << "RELATÓRIO DE ESTATÍSTICAS DO GRAFO\n";
    arquivo << "==================================\n\n";

    arquivo << "INFORMAÇÕES BÁSICAS:\n";
    arquivo << "--------------------\n";
    arquivo << "Número de vértices: " << stats.numVertices << "\n";
    arquivo << "Número de arestas: " << stats.numArestas << "\n";
    arquivo << "Grau mínimo: " << stats.grauMinimo << "\n";
    arquivo << "Grau máximo: " << stats.grauMaximo << "\n";
    arquivo << "Grau médio: " << fixed << setprecision(2) << stats.grauMedio << "\n";
    arquivo << "Mediana de grau: " << fixed << setprecision(1) << stats.medianaGrau << "\n";
    arquivo << "\n";

    arquivo << "COMPONENTES CONEXAS:\n";
    arquivo << "--------------------\n";
    arquivo << "Número de componentes: " << stats.numComponentes << "\n";

    if (stats.numComponentes == 1) {
        arquivo << "Status: Grafo conexo\n";
    } else {
        arquivo << "Status: Grafo não conexo\n";
        arquivo << "Maior componente: " << stats.maiorComponente << " vértices\n";
        arquivo << "Menor componente: " << stats.menorComponente << " vértices\n";
        arquivo << "Tamanhos das componentes:\n";
        for (size_t i = 0; i < stats.tamanhosComponentes.size(); i++) {
            arquivo << "  Componente " << (i+1) << ": " << stats.tamanhosComponentes[i] << " vértices\n";
        }
    }
    arquivo << "\n";

    arquivo << "DIÂMETRO:\n";
    arquivo << "---------\n";
    if (stats.isConexo) {
        arquivo << "Diâmetro: " << stats.diametro << "\n";
        arquivo << "Vértices do diâmetro: " << stats.verticesDiametro.first
                << " e " << stats.verticesDiametro.second << "\n";
    } else {
        arquivo << "Diâmetro: ∞ (infinito - grafo não conexo)\n";
        arquivo << "Maior distância finita: " << stats.diametro << "\n";
        if (stats.verticesDiametro.first != -1) {
            arquivo << "Vértices: " << stats.verticesDiametro.first
                    << " e " << stats.verticesDiametro.second << "\n";
        }
    }
}

void Estatisticas::calcularDiametro_Lista(const ListaAdjacencia& lista) {
    Distancias dist(n);

    if (n > 100) {
        cout << "🚀 Usando algoritmo otimizado para cálculo do diâmetro\n";
        dist.calcularDiametroApenas_Lista(lista);
    } else {

        dist.calcularDistancias_Lista(lista);
    }

    stats.diametro = dist.getDiametro();
    stats.verticesDiametro = dist.getVerticesDiametro();
}

void Estatisticas::analisarBuscas_Lista(const ListaAdjacencia& lista) {
    cout << "\n🔍 ANÁLISE DE BUSCAS (BFS e DFS)\n";
    cout << "=================================\n";

    vector<int> vertices_iniciais = {1, 2, 3};
    vector<int> vertices_consulta = {10, 20, 30};

    for (int inicio : vertices_iniciais) {
        if (inicio > n) continue;

        cout << "\n--- Iniciando busca a partir do vértice " << inicio << " ---\n";

        BFS bfs(n);
        bfs.executarBFS_Lista(lista, inicio);

        cout << "BFS - Pais dos vértices:\n";
        for (int v : vertices_consulta) {
            if (v <= n) {
                int pai = bfs.getPai(v);
                cout << "  Vértice " << v << ": pai = " << (pai == -1 ? "raiz/não visitado" : to_string(pai)) << "\n";
            }
        }

        DFS dfs(n);
        dfs.executarDFS_Lista(lista, inicio);

        cout << "DFS - Pais dos vértices:\n";
        for (int v : vertices_consulta) {
            if (v <= n) {
                int pai = dfs.getPai(v);
                cout << "  Vértice " << v << ": pai = " << (pai == -1 ? "raiz/não visitado" : to_string(pai)) << "\n";
            }
        }
    }
}

void Estatisticas::calcularDistanciasEspecificas_Lista(const ListaAdjacencia& lista) {
    cout << "\n📏 DISTÂNCIAS ESPECÍFICAS\n";
    cout << "=========================\n";

    Distancias dist(n);

    vector<pair<int, int>> pares = {{10, 20}, {10, 30}, {20, 30}};

    for (const auto& par : pares) {
        int origem = par.first;
        int destino = par.second;

        if (origem <= n && destino <= n) {
            int distancia = dist.calcularDistanciaEspecifica_Lista(lista, origem, destino);
            cout << "Distância entre vértices " << origem << " e " << destino << ": ";
            if (distancia == 999999) {
                cout << "∞ (não conectados)";
            } else {
                cout << distancia;
            }
            cout << "\n";
        }
    }
}

void Estatisticas::executarAnaliseCompleta_Lista(const ListaAdjacencia& lista) {
    analisarBuscas_Lista(lista);
    calcularDistanciasEspecificas_Lista(lista);
}

const EstatisticasGrafo& Estatisticas::getEstatisticas() const {
    return stats;
}
