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


const EstatisticasGrafo& Estatisticas::getEstatisticas() const {
    return stats;
}

// Implementações dos métodos genéricos usando interface IGrafo

vector<int> Estatisticas::calcularGraus(const IGrafo& grafo) {
    vector<int> graus(n, 0);

    for (int i = 0; i < n; i++) {
        vector<int> vizinhos = grafo.getVizinhos(i);
        graus[i] = vizinhos.size();
    }

    return graus;
}

void Estatisticas::calcularEstatisticas(const IGrafo& grafo, int numArestas) {
    stats.numArestas = numArestas;
    vector<int> graus = calcularGraus(grafo);
    calcularEstatisticasGrau(graus);
    calcularDiametro(grafo);
}

void Estatisticas::calcularDiametro(const IGrafo& grafo) {
    Distancias dist(n);

    if (n > 100) {
        cout << "🚀 Usando algoritmo otimizado para cálculo do diâmetro\n";
        // Para grafos grandes, usamos a implementação específica mais eficiente
        // Convertemos para ListaAdjacencia temporariamente
        // Isso poderia ser melhorado criando uma versão genérica do algoritmo otimizado
        cout << "⚠️  Algoritmo otimizado requer conversão para implementação específica\n";
        // dist.calcularDiametroApenas(grafo); // TODO: implementar versão genérica
    } else {
        dist.calcularDistancias(grafo);
    }

    stats.diametro = dist.getDiametro();
    stats.verticesDiametro = dist.getVerticesDiametro();
}

void Estatisticas::analisarBuscas(const IGrafo& grafo) {
    cout << "\n🔍 ANÁLISE DE BUSCAS (BFS e DFS)\n";
    cout << "=================================\n";

    vector<int> vertices_iniciais = {0, 1, 2}; // Usando base 0
    vector<int> vertices_consulta = {9, 19, 29}; // Ajustado para base 0

    for (int inicio : vertices_iniciais) {
        if (inicio >= n) continue;

        cout << "\n--- Iniciando busca a partir do vértice " << (inicio + 1) << " ---\n";

        BFS bfs(n);
        bfs.executarBFS(grafo, inicio + 1); // Convertendo para base 1 para BFS

        vector<int> ordem_bfs = bfs.getOrdemVisitacao();
        vector<int> distancias_bfs = bfs.getNiveis();

        cout << "🔵 BFS - Primeiros vértices visitados: ";
        for (int i = 0; i < min(10, (int)ordem_bfs.size()); i++) {
            cout << ordem_bfs[i] << " ";
        }
        cout << "\n";

        DFS dfs(n);
        dfs.executarDFS(grafo, inicio + 1); // Convertendo para base 1 para DFS

        vector<int> ordem_dfs = dfs.getOrdemVisitacao();

        cout << "🔴 DFS - Primeiros vértices visitados: ";
        for (int i = 0; i < min(10, (int)ordem_dfs.size()); i++) {
            cout << ordem_dfs[i] << " ";
        }
        cout << "\n";

        for (int consulta : vertices_consulta) {
            if (consulta >= n) continue;

            int distancia = distancias_bfs[consulta];
            if (distancia >= 0) { // Verifica se o vértice foi alcançado
                cout << "📏 Distância de " << (inicio + 1) << " até " << (consulta + 1) 
                     << ": " << distancia << "\n";
            }
        }
    }
}

void Estatisticas::calcularDistanciasEspecificas(const IGrafo& grafo) {
    cout << "\n📐 ANÁLISE DE DISTÂNCIAS ESPECÍFICAS\n";
    cout << "=====================================\n";

    vector<pair<int, int>> pares_vertices = {
        {1, n/2}, {1, n-1}, {n/4, 3*n/4}
    };

    for (auto& par : pares_vertices) {
        int origem = par.first - 1; // Converter para base 0
        int destino = par.second - 1; // Converter para base 0
        
        if (origem >= n || destino >= n) continue;

        BFS bfs(n);
        bfs.executarBFS(grafo, origem + 1); // Converter de volta para base 1 para BFS

        vector<int> distancias = bfs.getNiveis();
        int distancia = distancias[destino];

        if (distancia >= 0) { // Verifica se o vértice foi alcançado
            cout << "🎯 Distância entre " << (origem + 1) << " e " << (destino + 1) 
                 << ": " << distancia << "\n";
        } else {
            cout << "❌ Vértices " << (origem + 1) << " e " << (destino + 1) 
                 << " não estão conectados\n";
        }
    }
}

void Estatisticas::executarAnaliseCompleta(const IGrafo& grafo) {
    cout << "\n🔍 EXECUTANDO ANÁLISE COMPLETA DO GRAFO\n";
    cout << "========================================\n";

    analisarBuscas(grafo);
    calcularDistanciasEspecificas(grafo);
}
