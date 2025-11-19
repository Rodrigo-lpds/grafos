#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <memory>
#include <dirent.h>
#include <algorithm>
#include <tuple>

#include "representacao_leitura/leitor_grafo.h"
#include "representacao_leitura/lista_adjacencia_peso.h"
#include "representacao_leitura/lista_adjacencia_peso_adapter.h"
#include "busca/bellman_ford.h"
#include "busca/dijkstra.h"

using namespace std;

class MenuEstudoCaso {
private:
    string grafoSelecionado;
    unique_ptr<ListaAdjacenciaPeso> listaPeso;
    int numVertices;
    bool temPesosNegativos;

    // Vértices de origem e destino para o estudo
    const vector<int> verticesOrigem = {10, 20, 30};
    const int verticeDestino = 100;
    const int numRodadas = 10;

    vector<string> listarArquivosGrafo() {
        vector<string> arquivos;
        DIR* dir = opendir("grafos");
        if (dir) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                string nome = entry->d_name;
                if (nome.find(".txt") != string::npos && nome.find("grafo_W") != string::npos) {
                    arquivos.push_back(nome);
                }
            }
            closedir(dir);
        }
        sort(arquivos.begin(), arquivos.end());
        return arquivos;
    }

    bool verificarPesosNegativos() {
        const auto& lista = listaPeso->getLista();
        for (int i = 0; i < numVertices; i++) {
            for (const auto& vizinho : lista[i]) {
                if (vizinho.second < 0) {
                    return true;
                }
            }
        }
        return false;
    }

    // Gera grafo com arestas invertidas para Dijkstra
    unique_ptr<ListaAdjacenciaPeso> gerarGrafoInvertido() {
        DadosGrafoPeso dadosInvertidos;
        dadosInvertidos.numVertices = numVertices;
        dadosInvertidos.direcionado = true;

        const auto& lista = listaPeso->getLista();
        for (int i = 0; i < numVertices; i++) {
            for (const auto& vizinho : lista[i]) {
                // Inverte a aresta: de (i+1 -> vizinho.first) para (vizinho.first -> i+1)
                dadosInvertidos.arestas.push_back(make_tuple(vizinho.first, i + 1, vizinho.second));
            }
        }

        return make_unique<ListaAdjacenciaPeso>(dadosInvertidos);
    }

    void imprimirCabecalho(const string& titulo) {
        cout << "\n" << string(60, '=') << endl;
        cout << titulo << endl;
        cout << string(60, '=') << endl;
    }

    void imprimirTabelaDistancias(const string& algoritmo, const vector<double>& distancias) {
        cout << "\n+------------+------------+" << endl;
        cout << "| Vertice    | Distancia  |" << endl;
        cout << "+------------+------------+" << endl;

        for (int v : verticesOrigem) {
            cout << "| " << setw(10) << v << " | ";
            if (distancias[v - 1] == numeric_limits<double>::infinity()) {
                cout << setw(10) << "INF" << " |" << endl;
            } else {
                cout << setw(10) << fixed << setprecision(2) << distancias[v - 1] << " |" << endl;
            }
        }
        cout << "+------------+------------+" << endl;
        cout << "Destino: vertice " << verticeDestino << endl;
        cout << "Algoritmo: " << algoritmo << endl;
    }

    void imprimirTabelaTempo(const string& algoritmo, double tempoMedio) {
        cout << "\n+----------------------+----------------+" << endl;
        cout << "| Algoritmo            | Tempo Medio(s) |" << endl;
        cout << "+----------------------+----------------+" << endl;
        cout << "| " << setw(20) << left << algoritmo << " | "
             << setw(14) << right << fixed << setprecision(6) << tempoMedio << " |" << endl;
        cout << "+----------------------+----------------+" << endl;
        cout << "Media de " << numRodadas << " rodadas" << endl;
    }

    void imprimirTabelaComparacao(const vector<double>& distBF, const vector<double>& distDijkstra) {
        cout << "\n+------------+----------------+----------------+------------+" << endl;
        cout << "| Vertice    | Bellman-Ford   | Dijkstra       | Diferenca  |" << endl;
        cout << "+------------+----------------+----------------+------------+" << endl;

        for (int v : verticesOrigem) {
            double dBF = distBF[v - 1];
            double dDij = distDijkstra[v - 1];
            double diff = abs(dBF - dDij);

            cout << "| " << setw(10) << v << " | ";

            if (dBF == numeric_limits<double>::infinity()) {
                cout << setw(14) << "INF" << " | ";
            } else {
                cout << setw(14) << fixed << setprecision(2) << dBF << " | ";
            }

            if (dDij == numeric_limits<double>::infinity()) {
                cout << setw(14) << "INF" << " | ";
            } else {
                cout << setw(14) << fixed << setprecision(2) << dDij << " | ";
            }

            cout << setw(10) << fixed << setprecision(4) << diff << " |" << endl;
        }
        cout << "+------------+----------------+----------------+------------+" << endl;
        cout << "Destino: vertice " << verticeDestino << endl;
    }

    void imprimirTabelaTempoComparacao(double tempoBF, double tempoDijkstra) {
        double razao = tempoBF / tempoDijkstra;

        cout << "\n+----------------------+----------------+----------------+" << endl;
        cout << "| Algoritmo            | Tempo Medio(s) | Razao          |" << endl;
        cout << "+----------------------+----------------+----------------+" << endl;
        cout << "| " << setw(20) << left << "Bellman-Ford" << " | "
             << setw(14) << right << fixed << setprecision(6) << tempoBF << " | "
             << setw(14) << fixed << setprecision(2) << 1.0 << " |" << endl;
        cout << "| " << setw(20) << left << "Dijkstra (Heap)" << " | "
             << setw(14) << right << fixed << setprecision(6) << tempoDijkstra << " | "
             << setw(14) << fixed << setprecision(2) << (1.0/razao) << " |" << endl;
        cout << "+----------------------+----------------+----------------+" << endl;
        cout << "Bellman-Ford eh " << fixed << setprecision(2) << razao << "x mais lento que Dijkstra" << endl;
        cout << "Media de " << numRodadas << " rodadas" << endl;
    }

public:
    void executar() {
        int opcao;

        do {
            cout << "\n" << string(60, '=') << endl;
            cout << "       MENU - ESTUDO DE CASO: BELLMAN-FORD vs DIJKSTRA" << endl;
            cout << string(60, '=') << endl;
            cout << "1. Selecionar grafo com peso" << endl;
            cout << "2. Executar estudo completo (Bellman-Ford)" << endl;
            cout << "3. Comparar com Dijkstra (apenas grafos sem pesos negativos)" << endl;
            cout << "4. Executar estudo completo automatico" << endl;
            cout << "0. Sair" << endl;
            cout << string(60, '-') << endl;

            if (!grafoSelecionado.empty()) {
                cout << "Grafo atual: " << grafoSelecionado << endl;
                cout << "Vertices: " << numVertices << endl;
                cout << "Pesos negativos: " << (temPesosNegativos ? "Sim" : "Nao") << endl;
            }

            cout << "\nEscolha uma opcao: ";
            cin >> opcao;

            switch (opcao) {
                case 1:
                    selecionarGrafo();
                    break;
                case 2:
                    if (grafoSelecionado.empty()) {
                        cout << "\nErro: Selecione um grafo primeiro!" << endl;
                    } else {
                        executarEstudoBellmanFord();
                    }
                    break;
                case 3:
                    if (grafoSelecionado.empty()) {
                        cout << "\nErro: Selecione um grafo primeiro!" << endl;
                    } else if (temPesosNegativos) {
                        cout << "\nErro: Grafo possui pesos negativos! Dijkstra nao pode ser usado." << endl;
                    } else {
                        executarComparacaoDijkstra();
                    }
                    break;
                case 4:
                    if (grafoSelecionado.empty()) {
                        cout << "\nErro: Selecione um grafo primeiro!" << endl;
                    } else {
                        executarEstudoCompleto();
                    }
                    break;
                case 0:
                    cout << "\nSaindo..." << endl;
                    break;
                default:
                    cout << "\nOpcao invalida!" << endl;
            }
        } while (opcao != 0);
    }

    void selecionarGrafo() {
        vector<string> arquivos = listarArquivosGrafo();

        if (arquivos.empty()) {
            cout << "\nNenhum grafo com peso encontrado na pasta 'grafos/'!" << endl;
            return;
        }

        cout << "\nGrafos com peso disponiveis:" << endl;
        cout << string(40, '-') << endl;

        for (size_t i = 0; i < arquivos.size(); i++) {
            cout << (i + 1) << ". " << arquivos[i] << endl;
        }

        cout << "\nEscolha o grafo (1-" << arquivos.size() << "): ";
        int escolha;
        cin >> escolha;

        if (escolha < 1 || escolha > (int)arquivos.size()) {
            cout << "Escolha invalida!" << endl;
            return;
        }

        grafoSelecionado = arquivos[escolha - 1];
        string caminhoCompleto = "grafos/" + grafoSelecionado;

        cout << "\nCarregando grafo..." << endl;

        try {
            // Determina se é direcionado (arquivos com _d são direcionados)
            bool direcionado = (grafoSelecionado.find("_d.txt") != string::npos);

            DadosGrafoPeso dados = LeitorGrafo::lerArquivoComPeso(caminhoCompleto, direcionado);
            listaPeso = make_unique<ListaAdjacenciaPeso>(dados);
            numVertices = listaPeso->getNumVertices();
            temPesosNegativos = verificarPesosNegativos();

            cout << "Grafo carregado com sucesso!" << endl;
            cout << "Vertices: " << numVertices << endl;
            cout << "Direcionado: " << (direcionado ? "Sim" : "Nao") << endl;
            cout << "Pesos negativos: " << (temPesosNegativos ? "Sim" : "Nao") << endl;

            if (numVertices < verticeDestino) {
                cout << "\nATENCAO: Grafo tem menos de " << verticeDestino << " vertices!" << endl;
                cout << "O estudo de caso requer vertices 10, 20, 30 e 100." << endl;
            }
        } catch (const exception& e) {
            cout << "Erro ao carregar grafo: " << e.what() << endl;
            grafoSelecionado = "";
        }
    }

    void executarEstudoBellmanFord() {
        imprimirCabecalho("ESTUDO BELLMAN-FORD - " + grafoSelecionado);

        if (numVertices < verticeDestino) {
            cout << "Erro: Grafo nao possui vertice " << verticeDestino << endl;
            return;
        }

        ListaAdjacenciaPesoAdapter adapter(*listaPeso);

        // 1. Calcular distâncias
        cout << "\n1. Calculando distancias dos vertices 10, 20, 30 para o vertice 100..." << endl;

        // Executar Bellman-Ford a partir do vértice 100
        ResultadoBellmanFord resultado = BellmanFord::executar(adapter, verticeDestino - 1);

        if (resultado.temCicloNegativo) {
            cout << "\n" << string(60, '!') << endl;
            cout << "ERRO: CICLO NEGATIVO DETECTADO NO GRAFO!" << endl;
            cout << string(60, '!') << endl;
            cout << "\nO algoritmo de Bellman-Ford nao pode calcular distancias" << endl;
            cout << "minimas validas quando ha ciclo negativo alcancavel." << endl;
            cout << "\nAs distancias abaixo NAO sao validas e servem apenas" << endl;
            cout << "para demonstrar que o ciclo foi detectado." << endl;

            if (!resultado.cicloNegativo.empty()) {
                cout << "\nCiclo negativo detectado nos vertices: ";
                for (size_t i = 0; i < min(resultado.cicloNegativo.size(), (size_t)5); i++) {
                    if (i > 0) cout << " -> ";
                    cout << resultado.cicloNegativo[i];
                }
                if (resultado.cicloNegativo.size() > 5) cout << " ...";
                cout << endl;
            }
            cout << endl;
        }

        imprimirTabelaDistancias("Bellman-Ford", resultado.distancias);

        if (resultado.temCicloNegativo) {
            cout << "\n*** ATENCAO: Valores invalidos devido a ciclo negativo ***" << endl;
            cout << "\nBenchmark de tempo sera executado apenas 1 vez (resultados nao sao validos)." << endl;

            // Executar apenas 1 vez para mostrar o tempo
            auto inicio = chrono::high_resolution_clock::now();
            BellmanFord::executar(adapter, verticeDestino - 1);
            auto fim = chrono::high_resolution_clock::now();

            chrono::duration<double> duracao = fim - inicio;
            cout << "Tempo de execucao: " << fixed << setprecision(6) << duracao.count() << "s" << endl;
            cout << "\n(Benchmark completo pulado devido a ciclo negativo)" << endl;
            return;
        }

        // 2. Calcular tempo médio
        cout << "\n2. Calculando tempo medio de execucao (" << numRodadas << " rodadas)..." << endl;

        double tempoTotal = 0.0;

        for (int i = 0; i < numRodadas; i++) {
            auto inicio = chrono::high_resolution_clock::now();
            BellmanFord::executar(adapter, verticeDestino - 1);
            auto fim = chrono::high_resolution_clock::now();

            chrono::duration<double> duracao = fim - inicio;
            tempoTotal += duracao.count();

            cout << "  Rodada " << (i + 1) << ": " << fixed << setprecision(6) << duracao.count() << "s" << endl;
        }

        double tempoMedio = tempoTotal / numRodadas;
        imprimirTabelaTempo("Bellman-Ford", tempoMedio);
    }

    void executarComparacaoDijkstra() {
        if (temPesosNegativos) {
            cout << "\nErro: Nao eh possivel usar Dijkstra com pesos negativos!" << endl;
            return;
        }

        imprimirCabecalho("COMPARACAO BELLMAN-FORD vs DIJKSTRA - " + grafoSelecionado);

        if (numVertices < verticeDestino) {
            cout << "Erro: Grafo nao possui vertice " << verticeDestino << endl;
            return;
        }

        // Para calcular distâncias DE vértices 10,20,30 PARA o vértice 100,
        // precisamos inverter as arestas e rodar a partir do 100
        cout << "\nGerando grafo com arestas invertidas para Dijkstra..." << endl;
        auto grafoInvertido = gerarGrafoInvertido();

        ListaAdjacenciaPesoAdapter adapterInvertido(*grafoInvertido);

        // 1. Calcular distâncias com ambos algoritmos
        cout << "\n1. Calculando distancias..." << endl;

        // Bellman-Ford no grafo invertido a partir do vértice 100
        // Para calcular dist(v -> 100), invertemos as arestas e calculamos dist(100 -> v)
        ResultadoBellmanFord resultadoBF = BellmanFord::executar(adapterInvertido, verticeDestino - 1);

        // Dijkstra no grafo invertido a partir do vértice 100
        // Dijkstra espera índice 1-based (faz origem - 1 internamente)
        ResultadoDijkstra resultadoDij = Dijkstra::executar(
            adapterInvertido,
            verticeDestino,
            make_unique<DijkstraHeapStrategy>()
        );

        imprimirTabelaComparacao(resultadoBF.distancias, resultadoDij.distancias);

        // 2. Calcular tempos médios
        cout << "\n2. Calculando tempos medios de execucao (" << numRodadas << " rodadas)..." << endl;

        double tempoTotalBF = 0.0;
        double tempoTotalDij = 0.0;

        cout << "\nBellman-Ford:" << endl;
        for (int i = 0; i < numRodadas; i++) {
            auto inicio = chrono::high_resolution_clock::now();
            BellmanFord::executar(adapterInvertido, verticeDestino - 1);
            auto fim = chrono::high_resolution_clock::now();

            chrono::duration<double> duracao = fim - inicio;
            tempoTotalBF += duracao.count();

            cout << "  Rodada " << (i + 1) << ": " << fixed << setprecision(6) << duracao.count() << "s" << endl;
        }

        cout << "\nDijkstra (Heap):" << endl;
        for (int i = 0; i < numRodadas; i++) {
            auto inicio = chrono::high_resolution_clock::now();
            Dijkstra::executar(
                adapterInvertido,
                verticeDestino,
                make_unique<DijkstraHeapStrategy>()
            );
            auto fim = chrono::high_resolution_clock::now();

            chrono::duration<double> duracao = fim - inicio;
            tempoTotalDij += duracao.count();

            cout << "  Rodada " << (i + 1) << ": " << fixed << setprecision(6) << duracao.count() << "s" << endl;
        }

        double tempoMedioBF = tempoTotalBF / numRodadas;
        double tempoMedioDij = tempoTotalDij / numRodadas;

        imprimirTabelaTempoComparacao(tempoMedioBF, tempoMedioDij);
    }

    void executarEstudoCompleto() {
        imprimirCabecalho("ESTUDO COMPLETO - " + grafoSelecionado);

        // Executar Bellman-Ford
        executarEstudoBellmanFord();

        // Se não tem pesos negativos, comparar com Dijkstra
        if (!temPesosNegativos) {
            cout << "\n" << string(60, '-') << endl;
            cout << "Grafo sem pesos negativos - executando comparacao com Dijkstra..." << endl;
            executarComparacaoDijkstra();
        } else {
            cout << "\n" << string(60, '-') << endl;
            cout << "Grafo possui pesos negativos - Dijkstra nao pode ser aplicado." << endl;
        }

        cout << "\n" << string(60, '=') << endl;
        cout << "ESTUDO COMPLETO FINALIZADO" << endl;
        cout << string(60, '=') << endl;
    }
};

int main() {
    MenuEstudoCaso menu;
    menu.executar();
    return 0;
}
