#include "componentes.h"
#include "../busca/dfs.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

ComponentesConexas::ComponentesConexas(int numVertices) : n(numVertices) {
    visitado.resize(n, false);
    componentes.clear();
}

void ComponentesConexas::encontrarComponentes_Matriz(const MatrizAdjacencia& matriz) {
    reset();

    DFS dfs(n);
    int id_componente = 1;

    for (int v = 0; v < n; v++) {
        if (!visitado[v]) {

            dfs.reset();
            dfs.executarDFS_Matriz(matriz, v + 1);

            const vector<int>& ordem_visitacao = dfs.getOrdemVisitacao();

            for (int vertice_1based : ordem_visitacao) {
                int vertice_0based = vertice_1based - 1;
                visitado[vertice_0based] = true;
            }

            componentes.emplace_back(id_componente++, ordem_visitacao);
        }
    }

    sort(componentes.begin(), componentes.end(),
         [](const Componente& a, const Componente& b) {
             if (a.tamanho != b.tamanho) {
                 return a.tamanho > b.tamanho;
             }
             return a.vertices[0] < b.vertices[0];
         });

    for (size_t i = 0; i < componentes.size(); i++) {
        componentes[i].id = i + 1;
    }
}

void ComponentesConexas::encontrarComponentes_Lista(const ListaAdjacencia& lista) {
    reset();

    DFS dfs(n);
    int id_componente = 1;

    for (int v = 0; v < n; v++) {
        if (!visitado[v]) {

            dfs.reset();
            dfs.executarDFS_Lista(lista, v + 1);

            const vector<int>& ordem_visitacao = dfs.getOrdemVisitacao();

            for (int vertice_1based : ordem_visitacao) {
                int vertice_0based = vertice_1based - 1;
                visitado[vertice_0based] = true;
            }

            componentes.emplace_back(id_componente++, ordem_visitacao);
        }
    }

    sort(componentes.begin(), componentes.end(),
         [](const Componente& a, const Componente& b) {
             if (a.tamanho != b.tamanho) {
                 return a.tamanho > b.tamanho;
             }
             return a.vertices[0] < b.vertices[0];
         });

    for (size_t i = 0; i < componentes.size(); i++) {
        componentes[i].id = i + 1;
    }
}

int ComponentesConexas::getNumComponentes() const {
    return componentes.size();
}

const vector<Componente>& ComponentesConexas::getComponentes() const {
    return componentes;
}

int ComponentesConexas::getComponenteDoVertice(int vertice) const {
    if (vertice < 1 || vertice > n) {
        return -1;
    }

    for (const auto& comp : componentes) {
        for (int v : comp.vertices) {
            if (v == vertice) {
                return comp.id;
            }
        }
    }
    return -1;
}

bool ComponentesConexas::estaoConectados(int v1, int v2) const {
    return getComponenteDoVertice(v1) == getComponenteDoVertice(v2) &&
           getComponenteDoVertice(v1) != -1;
}

void ComponentesConexas::imprimirComponentes() const {
    cout << "Componentes Conexas do Grafo:\n";
    cout << "============================\n";
    cout << "Total de componentes: " << componentes.size() << "\n\n";

    for (const auto& comp : componentes) {
        cout << "Componente " << comp.id << " (tamanho: " << comp.tamanho << "):\n";
        cout << "Vértices: ";
        for (size_t i = 0; i < comp.vertices.size(); i++) {
            if (i > 0) cout << ", ";
            cout << comp.vertices[i];
        }
        cout << "\n\n";
    }
}

void ComponentesConexas::salvarResultado(const string& nomeArquivo) const {
    ofstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw runtime_error("Erro ao criar arquivo de componentes");
    }

    arquivo << "Análise de Componentes Conexas\n";
    arquivo << "===============================\n\n";

    arquivo << "Resumo:\n";
    arquivo << "- Número total de vértices: " << n << "\n";
    arquivo << "- Número de componentes conexas: " << componentes.size() << "\n";
    arquivo << "- Grafo é conexo: " << (componentes.size() == 1 ? "Sim" : "Não") << "\n\n";

    arquivo << "Componentes (ordenadas por tamanho decrescente):\n";
    arquivo << "================================================\n\n";

    for (const auto& comp : componentes) {
        arquivo << "Componente " << comp.id << ":\n";
        arquivo << "- Tamanho: " << comp.tamanho << " vértices\n";
        arquivo << "- Vértices: ";
        for (size_t i = 0; i < comp.vertices.size(); i++) {
            if (i > 0) arquivo << ", ";
            arquivo << comp.vertices[i];
        }
        arquivo << "\n";
        arquivo << "- Porcentagem do grafo: "
                << fixed << setprecision(1)
                << (100.0 * comp.tamanho / n) << "%\n\n";
    }

    arquivo << "Estatísticas Detalhadas:\n";
    arquivo << "========================\n";

    if (componentes.size() > 1) {
        arquivo << "- Maior componente: " << componentes[0].tamanho << " vértices\n";
        arquivo << "- Menor componente: " << componentes.back().tamanho << " vértices\n";

        double media = 0;
        for (const auto& comp : componentes) {
            media += comp.tamanho;
        }
        media /= componentes.size();
        arquivo << "- Tamanho médio das componentes: "
                << fixed << setprecision(2) << media << " vértices\n";
    }

    arquivo << "\nMapeamento Vértice → Componente:\n";
    arquivo << "================================\n";
    for (int v = 1; v <= n; v++) {
        arquivo << "Vértice " << setw(2) << v << " → Componente "
                << getComponenteDoVertice(v) << "\n";
    }
}

void ComponentesConexas::imprimirEstatisticas() const {
    cout << "Estatísticas das Componentes:\n";
    cout << "============================\n";
    cout << "• Total de componentes: " << componentes.size() << "\n";
    cout << "• Grafo é conexo: " << (componentes.size() == 1 ? "✅ Sim" : "❌ Não") << "\n";

    if (componentes.size() > 1) {
        cout << "• Maior componente: " << componentes[0].tamanho << " vértices ("
             << fixed << setprecision(1) << (100.0 * componentes[0].tamanho / n)
             << "%)\n";
        cout << "• Menor componente: " << componentes.back().tamanho << " vértices ("
             << fixed << setprecision(1) << (100.0 * componentes.back().tamanho / n)
             << "%)\n";

        cout << "• Distribuição de tamanhos: ";
        for (size_t i = 0; i < componentes.size(); i++) {
            if (i > 0) cout << ", ";
            cout << componentes[i].tamanho;
        }
        cout << "\n";
    }
    cout << "\n";
}

void ComponentesConexas::reset() {
    fill(visitado.begin(), visitado.end(), false);
    componentes.clear();
}
