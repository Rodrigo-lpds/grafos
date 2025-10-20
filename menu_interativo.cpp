#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <sstream>
#include <limits>

// Includes para as funcionalidades do projeto
#include "representacao_leitura/leitor_grafo.h"
#include "representacao_leitura/matriz_adjacencia.h"
#include "representacao_leitura/lista_adjacencia.h"
#include "representacao_leitura/lista_adjacencia_peso.h"
#include "representacao_leitura/matriz_adjacencia_adapter.h"
#include "representacao_leitura/lista_adjacencia_adapter.h"
#include "representacao_leitura/lista_adjacencia_peso_adapter.h"
#include "busca/bfs.h"
#include "busca/dfs.h"
#include "busca/dijkstra.h"
#include "busca/dijkstra_vetor_strategy.h"
#include "busca/dijkstra_heap_strategy.h"
#include "analise/estatisticas.h"
#include "analise/componentes.h"
#include "analise/distancias.h"

using namespace std;

class MenuInterativo {
private:
    string grafoSelecionado;
    string tipoEstrutura; // "matriz" ou "lista"
    bool grafoPeso;
    string tipoDijkstra; // "vetor" ou "heap"
    
    // Dados carregados
    unique_ptr<MatrizAdjacencia> matriz;
    unique_ptr<ListaAdjacencia> lista;
    unique_ptr<ListaAdjacenciaPeso> listaPeso;
    int numVertices;
    
    void limparTela() {
        system("clear");
    }
    
    void pausar() {
        cout << "\nPressione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
    
    void mostrarCabecalho() {
        cout << "╔═══════════════════════════════════════════════════════════════╗\n";
        cout << "║                    SISTEMA DE ANÁLISE DE GRAFOS               ║\n";
        cout << "╚═══════════════════════════════════════════════════════════════╝\n\n";
    }
    
    vector<string> listarGrafos() {
        vector<string> grafos;
        try {
            for (const auto& entry : filesystem::directory_iterator("grafos/")) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                    grafos.push_back(entry.path().filename().string());
                }
            }
            sort(grafos.begin(), grafos.end());
        } catch (const exception& e) {
            cout << "Erro ao listar grafos: " << e.what() << endl;
        }
        return grafos;
    }
    
    bool verificarSeGrafoPeso(const string& nomeArquivo) {
        return nomeArquivo.find("peso") != string::npos;
    }
    
    void selecionarGrafo() {
        limparTela();
        mostrarCabecalho();
        
        cout << "┌─────────────────────────────────────────────────────────────┐\n";
        cout << "│                      SELEÇÃO DE GRAFO                      │\n";
        cout << "└─────────────────────────────────────────────────────────────┘\n\n";
        
        vector<string> grafos = listarGrafos();
        
        if (grafos.empty()) {
            cout << "❌ Nenhum grafo encontrado na pasta 'grafos/'.\n";
            pausar();
            return;
        }
        
        cout << "Grafos disponíveis:\n\n";
        for (size_t i = 0; i < grafos.size(); i++) {
            string tipo = verificarSeGrafoPeso(grafos[i]) ? " [COM PESO]" : " [SEM PESO]";
            cout << setw(2) << (i + 1) << ". " << grafos[i] << tipo << "\n";
        }
        
        cout << "\n" << setw(2) << "0" << ". Voltar ao menu principal\n\n";
        cout << "👉 Digite sua escolha: ";
        
        int escolha;
        cin >> escolha;
        
        if (escolha == 0) return;
        
        if (escolha < 1 || escolha > static_cast<int>(grafos.size())) {
            cout << "❌ Opção inválida!\n";
            pausar();
            return;
        }
        
        grafoSelecionado = grafos[escolha - 1];
        grafoPeso = verificarSeGrafoPeso(grafoSelecionado);
        
        cout << "\n✅ Grafo selecionado: " << grafoSelecionado;
        if (grafoPeso) {
            cout << " (Grafo com peso)";
        } else {
            cout << " (Grafo sem peso)";
        }
        cout << "\n";
        pausar();
    }
    
    void selecionarEstrutura() {
        limparTela();
        mostrarCabecalho();
        
        cout << "┌─────────────────────────────────────────────────────────────┐\n";
        cout << "│                   TIPO DE ESTRUTURA                        │\n";
        cout << "└─────────────────────────────────────────────────────────────┘\n\n";
        
        if (grafoSelecionado.empty()) {
            cout << "❌ Primeiro selecione um grafo!\n";
            pausar();
            return;
        }
        
        cout << "Grafo atual: " << grafoSelecionado << "\n\n";
        cout << "Escolha o tipo de estrutura de dados:\n\n";
        cout << "1. Matriz de Adjacência\n";
        cout << "2. Lista de Adjacência\n";
        cout << "0. Voltar ao menu principal\n\n";
        cout << "👉 Digite sua escolha: ";
        
        int escolha;
        cin >> escolha;
        
        switch (escolha) {
            case 1:
                tipoEstrutura = "matriz";
                cout << "\n✅ Estrutura selecionada: Matriz de Adjacência\n";
                break;
            case 2:
                tipoEstrutura = "lista";
                cout << "\n✅ Estrutura selecionada: Lista de Adjacência\n";
                break;
            case 0:
                return;
            default:
                cout << "❌ Opção inválida!\n";
                pausar();
                return;
        }
        pausar();
    }
    
    void configurarDijkstra() {
        limparTela();
        mostrarCabecalho();
        
        cout << "┌─────────────────────────────────────────────────────────────┐\n";
        cout << "│                CONFIGURAÇÃO DO DIJKSTRA                    │\n";
        cout << "└─────────────────────────────────────────────────────────────┘\n\n";
        
        if (!grafoPeso) {
            cout << "❌ Esta opção só está disponível para grafos com peso!\n";
            pausar();
            return;
        }
        
        cout << "Escolha a implementação do algoritmo de Dijkstra:\n\n";
        cout << "1. Implementação com Vetor (mais simples)\n";
        cout << "2. Implementação com Heap (mais eficiente)\n";
        cout << "0. Voltar ao menu principal\n\n";
        cout << "👉 Digite sua escolha: ";
        
        int escolha;
        cin >> escolha;
        
        switch (escolha) {
            case 1:
                tipoDijkstra = "vetor";
                cout << "\n✅ Dijkstra configurado: Implementação com Vetor\n";
                break;
            case 2:
                tipoDijkstra = "heap";
                cout << "\n✅ Dijkstra configurado: Implementação com Heap\n";
                break;
            case 0:
                return;
            default:
                cout << "❌ Opção inválida!\n";
                pausar();
                return;
        }
        pausar();
    }
    
    bool carregarGrafo() {
        if (grafoSelecionado.empty() || tipoEstrutura.empty()) {
            cout << "❌ Configure primeiro o grafo e a estrutura de dados!\n";
            pausar();
            return false;
        }
        
        string caminhoArquivo = "grafos/" + grafoSelecionado;
        
        cout << "📂 Carregando grafo: " << grafoSelecionado << "...\n";
        
        try {
            if (grafoPeso) {
                DadosGrafoPeso dados = LeitorGrafo::lerArquivoComPeso(caminhoArquivo);
                listaPeso = make_unique<ListaAdjacenciaPeso>(dados);
                numVertices = listaPeso->getNumVertices();
                cout << "✅ Grafo com peso carregado com sucesso!\n";
            } else {
                DadosGrafo dados = LeitorGrafo::lerArquivo(caminhoArquivo);
                if (tipoEstrutura == "matriz") {
                    matriz = make_unique<MatrizAdjacencia>(dados);
                    numVertices = dados.numVertices;
                } else {
                    lista = make_unique<ListaAdjacencia>(dados);
                    numVertices = dados.numVertices;
                }
                cout << "✅ Grafo carregado com sucesso!\n";
            }
            
            cout << "📊 Número de vértices: " << numVertices << "\n";
            return true;
            
        } catch (const exception& e) {
            cout << "❌ Erro ao carregar grafo: " << e.what() << "\n";
            return false;
        }
    }
    
    void executarBusca() {
        limparTela();
        mostrarCabecalho();
        
        cout << "┌─────────────────────────────────────────────────────────────┐\n";
        cout << "│                    ALGORITMOS DE BUSCA                     │\n";
        cout << "└─────────────────────────────────────────────────────────────┘\n\n";
        
        if (!carregarGrafo()) {
            pausar();
            return;
        }
        
        if (grafoPeso) {
            executarDijkstra();
        } else {
            executarBuscaSemPeso();
        }
    }
    
    void executarBuscaSemPeso() {
        cout << "\nEscolha o algoritmo de busca:\n\n";
        cout << "1. BFS (Busca em Largura)\n";
        cout << "2. DFS (Busca em Profundidade)\n";
        cout << "0. Voltar\n\n";
        cout << "👉 Digite sua escolha: ";
        
        int escolha;
        cin >> escolha;
        
        if (escolha == 0) return;
        
        cout << "\n👉 Digite o vértice inicial (1 a " << numVertices << "): ";
        int verticeInicial;
        cin >> verticeInicial;
        
        if (verticeInicial < 1 || verticeInicial > numVertices) {
            cout << "❌ Vértice inválido!\n";
            pausar();
            return;
        }
        
        cout << "👉 Digite o vértice destino (opcional, 0 para busca completa): ";
        int destino;
        cin >> destino;
        
        if (destino < 0 || destino > numVertices) {
            cout << "❌ Vértice de destino inválido!\n";
            pausar();
            return;
        }
        
        if (destino == 0) destino = -1;
        
        cout << "\n🔍 Executando busca...\n";
        
        try {
            if (escolha == 1) { // BFS
                BFS bfs(numVertices);
                if (tipoEstrutura == "matriz") {
                    bfs.executarBFS_Matriz(*matriz, verticeInicial, destino);
                } else {
                    bfs.executarBFS_Lista(*lista, verticeInicial, destino);
                }
                bfs.imprimirResultado();
                
                if (destino != -1) {
                    int nivel = bfs.getNivel(destino);
                    if (nivel != -1) {
                        cout << "\n📍 Caminho encontrado!\n";
                        cout << "📏 Distância: " << nivel << "\n";
                        
                        // Reconstruir caminho
                        vector<int> caminho;
                        int atual = destino;
                        while (atual != -1) {
                            caminho.push_back(atual);
                            atual = bfs.getPai(atual);
                        }
                        reverse(caminho.begin(), caminho.end());
                        
                        cout << "🛤️  Caminho: ";
                        for (size_t i = 0; i < caminho.size(); i++) {
                            cout << caminho[i];
                            if (i < caminho.size() - 1) cout << " → ";
                        }
                        cout << "\n";
                    } else {
                        cout << "\n❌ Não há caminho entre " << verticeInicial << " e " << destino << "\n";
                    }
                }
                
            } else { // DFS
                DFS dfs(numVertices);
                if (tipoEstrutura == "matriz") {
                    dfs.executarDFS_Matriz(*matriz, verticeInicial);
                } else {
                    dfs.executarDFS_Lista(*lista, verticeInicial);
                }
                dfs.imprimirResultado();
                
                if (destino != -1) {
                    if (dfs.foiVisitado(destino)) {
                        cout << "\n📍 Vértice " << destino << " foi alcançado pela busca!\n";
                    } else {
                        cout << "\n❌ Vértice " << destino << " não foi alcançado pela busca!\n";
                    }
                }
            }
            
        } catch (const exception& e) {
            cout << "❌ Erro durante a execução: " << e.what() << "\n";
        }
        
        pausar();
    }
    
    void executarDijkstra() {
        cout << "\n👉 Digite o vértice de origem (1 a " << numVertices << "): ";
        int origem;
        cin >> origem;
        
        if (origem < 1 || origem > numVertices) {
            cout << "❌ Vértice inválido!\n";
            pausar();
            return;
        }
        
        cout << "\n🔍 Executando Dijkstra (" << tipoDijkstra << ")...\n";
        cout << "🔍 Debug: Vértice origem = " << origem << ", Num vértices = " << numVertices << "\n";
        
        try {
            ResultadoDijkstra resultado;
            
            cout << "🔍 Debug: Iniciando algoritmo...\n";
            
            if (tipoDijkstra == "vetor") {
                resultado = Dijkstra::dijkstraVetor(*listaPeso, origem);
            } else {
                resultado = Dijkstra::dijkstraHeap(*listaPeso, origem);
            }
            
            cout << "🔍 Debug: Algoritmo concluído!\n";
            
            cout << "\n📊 RESULTADOS DO DIJKSTRA:\n";
            cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            cout << "Origem: " << origem << "\n\n";
            
            cout << "Distâncias mínimas:\n";
            for (int i = 1; i <= numVertices; i++) {
                cout << "  Vértice " << setw(3) << i << ": ";
                if (resultado.distancias[i-1] == numeric_limits<double>::infinity()) {
                    cout << "∞ (inacessível)\n";
                } else {
                    cout << fixed << setprecision(2) << resultado.distancias[i-1] << "\n";
                }
            }
            
            cout << "\n👉 Deseja ver o caminho para algum vértice específico? (s/n): ";
            char resposta;
            cin >> resposta;
            
            if (resposta == 's' || resposta == 'S') {
                cout << "👉 Digite o vértice destino: ";
                int destino;
                cin >> destino;
                
                if (destino >= 1 && destino <= numVertices) {
                    int destinoIdx = destino - 1;
                    if (resultado.distancias[destinoIdx] != numeric_limits<double>::infinity()) {
                        cout << "\n📍 Caminho de " << origem << " para " << destino << ":\n";
                        vector<int> caminho;
                        int atual = destino;
                        while (atual != origem && atual != -1) {
                            caminho.push_back(atual);
                            atual = resultado.predecessores[atual-1];
                        }
                        if (atual == origem) {
                            caminho.push_back(origem);
                            
                            reverse(caminho.begin(), caminho.end());
                            
                            for (size_t i = 0; i < caminho.size(); i++) {
                                cout << caminho[i];
                                if (i < caminho.size() - 1) cout << " → ";
                            }
                            cout << "\n📏 Distância total: " << fixed << setprecision(2) 
                                 << resultado.distancias[destinoIdx] << "\n";
                        } else {
                            cout << "\n❌ Erro na reconstrução do caminho!\n";
                        }
                    } else {
                        cout << "\n❌ Vértice " << destino << " não é acessível a partir de " << origem << "\n";
                    }
                }
            }
            
        } catch (const exception& e) {
            cout << "❌ Erro durante a execução: " << e.what() << "\n";
        }
        
        pausar();
    }
    
    void analisarEstatisticas() {
        limparTela();
        mostrarCabecalho();
        
        cout << "┌─────────────────────────────────────────────────────────────┐\n";
        cout << "│                    ANÁLISE ESTATÍSTICA                     │\n";
        cout << "└─────────────────────────────────────────────────────────────┘\n\n";
        
        if (grafoPeso) {
            cout << "❌ Análise estatística não disponível para grafos com peso!\n";
            pausar();
            return;
        }
        
        if (!carregarGrafo()) {
            pausar();
            return;
        }
        
        cout << "🔍 Analisando estatísticas do grafo...\n\n";
        
        try {
            Estatisticas estatisticas(numVertices);
            
            if (tipoEstrutura == "matriz") {
                MatrizAdjacenciaAdapter adapter(*matriz);
                // Calcular número de arestas manualmente
                int numArestas = 0;
                const auto& mat = matriz->getMatriz();
                for (int i = 0; i < numVertices; i++) {
                    for (int j = i + 1; j < numVertices; j++) {
                        if (mat[i][j] == 1) numArestas++;
                    }
                }
                estatisticas.calcularEstatisticas(adapter, numArestas);
            } else {
                ListaAdjacenciaAdapter adapter(*lista);
                // Calcular número de arestas manualmente
                int numArestas = 0;
                const auto& listAdj = lista->getLista();
                for (int i = 0; i < numVertices; i++) {
                    numArestas += listAdj[i].size();
                }
                numArestas /= 2; // Grafo não direcionado
                estatisticas.calcularEstatisticas(adapter, numArestas);
            }
            
            const EstatisticasGrafo& stats = estatisticas.getEstatisticas();
            
            cout << "📊 ESTATÍSTICAS DO GRAFO:\n";
            cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            cout << "🔸 Vértices: " << stats.numVertices << "\n";
            cout << "🔸 Arestas: " << stats.numArestas << "\n";
            cout << "🔸 Grau mínimo: " << stats.grauMinimo << "\n";
            cout << "🔸 Grau máximo: " << stats.grauMaximo << "\n";
            cout << "🔸 Grau médio: " << fixed << setprecision(2) << stats.grauMedio << "\n";
            cout << "🔸 Mediana do grau: " << fixed << setprecision(2) << stats.medianaGrau << "\n";
            cout << "🔸 Componentes conexas: " << stats.numComponentes << "\n";
            cout << "🔸 Grafo conexo: " << (stats.isConexo ? "✅ Sim" : "❌ Não") << "\n";
            
            if (stats.numComponentes > 1) {
                cout << "🔸 Maior componente: " << stats.maiorComponente << " vértices\n";
                cout << "🔸 Menor componente: " << stats.menorComponente << " vértices\n";
                
                cout << "\n📋 Tamanhos das componentes:\n";
                for (size_t i = 0; i < stats.tamanhosComponentes.size(); i++) {
                    cout << "  Componente " << (i + 1) << ": " << stats.tamanhosComponentes[i] << " vértices\n";
                }
            }
            
            cout << "\n👉 Deseja ver mais detalhes? (s/n): ";
            char resposta;
            cin >> resposta;
            
            if (resposta == 's' || resposta == 'S') {
                cout << "\n📊 DISTRIBUIÇÃO DE GRAUS:\n";
                cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                
                // Mostrar alguns vértices com seus graus
                int limite = min(20, static_cast<int>(stats.graus.size()));
                cout << "Primeiros " << limite << " vértices:\n";
                for (int i = 0; i < limite; i++) {
                    cout << "  Vértice " << setw(3) << (i + 1) << ": grau " << stats.graus[i] << "\n";
                }
                
                if (stats.graus.size() > 20) {
                    cout << "  ... (total de " << stats.graus.size() << " vértices)\n";
                }
            }
            
        } catch (const exception& e) {
            cout << "❌ Erro durante a análise: " << e.what() << "\n";
        }
        
        pausar();
    }
    
    void analisarDistancias() {
        limparTela();
        mostrarCabecalho();
        
        cout << "┌─────────────────────────────────────────────────────────────┐\n";
        cout << "│                    ANÁLISE DE DISTÂNCIAS                   │\n";
        cout << "└─────────────────────────────────────────────────────────────┘\n\n";
        
        if (grafoPeso) {
            cout << "❌ Use o Dijkstra para análise de distâncias em grafos com peso!\n";
            pausar();
            return;
        }
        
        if (!carregarGrafo()) {
            pausar();
            return;
        }
        
        cout << "Escolha o tipo de análise:\n\n";
        cout << "1. Distância entre dois vértices específicos\n";
        cout << "2. Calcular diâmetro do grafo (aproximado)\n";
        cout << "3. Distâncias a partir de um vértice\n";
        cout << "0. Voltar\n\n";
        cout << "👉 Digite sua escolha: ";
        
        int escolha;
        cin >> escolha;
        
        if (escolha == 0) return;
        
        try {
            Distancias distancias(numVertices);
            
            switch (escolha) {
                case 1: {
                    cout << "\n👉 Digite o vértice origem (1 a " << numVertices << "): ";
                    int origem;
                    cin >> origem;
                    
                    cout << "👉 Digite o vértice destino (1 a " << numVertices << "): ";
                    int destino;
                    cin >> destino;
                    
                    if (origem < 1 || origem > numVertices || destino < 1 || destino > numVertices) {
                        cout << "❌ Vértices inválidos!\n";
                        break;
                    }
                    
                    int dist;
                    if (tipoEstrutura == "matriz") {
                        MatrizAdjacenciaAdapter adapter(*matriz);
                        dist = distancias.calcularDistanciaEspecifica(adapter, origem, destino);
                    } else {
                        ListaAdjacenciaAdapter adapter(*lista);
                        dist = distancias.calcularDistanciaEspecifica(adapter, origem, destino);
                    }
                    
                    cout << "\n📏 Distância entre " << origem << " e " << destino << ": ";
                    if (dist == Distancias::INFINITO) {
                        cout << "∞ (vértices não conectados)\n";
                    } else {
                        cout << dist << "\n";
                    }
                    break;
                }
                
                case 2: {
                    cout << "\n🔍 Calculando estatísticas de distâncias...\n";
                    
                    if (tipoEstrutura == "matriz") {
                        MatrizAdjacenciaAdapter adapter(*matriz);
                        distancias.calcularDistancias(adapter);
                    } else {
                        ListaAdjacenciaAdapter adapter(*lista);
                        distancias.calcularDistancias(adapter);
                    }
                    
                    int diametro = distancias.getDiametro();
                    pair<int, int> vertices = distancias.getVerticesDiametro();
                    
                    cout << "\n📐 DIÂMETRO DO GRAFO:\n";
                    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                    cout << "🔸 Diâmetro: " << diametro << "\n";
                    cout << "🔸 Vértices mais distantes: " << vertices.first << " e " << vertices.second << "\n";
                    
                    if (diametro == Distancias::INFINITO) {
                        cout << "🔸 O grafo não é conexo!\n";
                    }
                    break;
                }
                
                case 3: {
                    cout << "\n👉 Digite o vértice origem (1 a " << numVertices << "): ";
                    int origem;
                    cin >> origem;
                    
                    if (origem < 1 || origem > numVertices) {
                        cout << "❌ Vértice inválido!\n";
                        break;
                    }
                    
                    vector<int> dists;
                    if (tipoEstrutura == "matriz") {
                        MatrizAdjacenciaAdapter adapter(*matriz);
                        dists = distancias.bfs_distancias(adapter, origem);
                    } else {
                        ListaAdjacenciaAdapter adapter(*lista);
                        dists = distancias.bfs_distancias(adapter, origem);
                    }
                    
                    cout << "\n📊 DISTÂNCIAS A PARTIR DO VÉRTICE " << origem << ":\n";
                    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                    
                    int limite = min(50, static_cast<int>(dists.size()));
                    for (int i = 1; i <= limite; i++) {
                        cout << "  Para vértice " << setw(3) << i << ": ";
                        if (dists[i] == Distancias::INFINITO) {
                            cout << "∞\n";
                        } else {
                            cout << dists[i] << "\n";
                        }
                    }
                    
                    if (dists.size() > 50) {
                        cout << "  ... (mostrando apenas os primeiros 50 vértices)\n";
                    }
                    break;
                }
                
                default:
                    cout << "❌ Opção inválida!\n";
                    break;
            }
            
        } catch (const exception& e) {
            cout << "❌ Erro durante a análise: " << e.what() << "\n";
        }
        
        pausar();
    }
    
    void mostrarConfiguracaoAtual() {
        cout << "\n┌─────────────────────────────────────────────────────────────┐\n";
        cout << "│                    CONFIGURAÇÃO ATUAL                      │\n";
        cout << "└─────────────────────────────────────────────────────────────┘\n";
        
        cout << "🔸 Grafo: " << (grafoSelecionado.empty() ? "❌ Não selecionado" : "✅ " + grafoSelecionado) << "\n";
        cout << "🔸 Estrutura: " << (tipoEstrutura.empty() ? "❌ Não configurada" : "✅ " + tipoEstrutura) << "\n";
        cout << "🔸 Tipo: " << (grafoPeso ? "✅ Com peso" : "✅ Sem peso") << "\n";
        
        if (grafoPeso && !tipoDijkstra.empty()) {
            cout << "🔸 Dijkstra: ✅ " << tipoDijkstra << "\n";
        } else if (grafoPeso) {
            cout << "🔸 Dijkstra: ❌ Não configurado\n";
        }
    }

public:
    void executar() {
        int opcao;
        
        do {
            limparTela();
            mostrarCabecalho();
            
            mostrarConfiguracaoAtual();
            
            cout << "\n┌─────────────────────────────────────────────────────────────┐\n";
            cout << "│                        MENU PRINCIPAL                      │\n";
            cout << "└─────────────────────────────────────────────────────────────┘\n\n";
            
            cout << "⚙️  CONFIGURAÇÃO:\n";
            cout << "   1. Selecionar grafo\n";
            cout << "   2. Definir estrutura de dados (matriz/lista)\n";
            cout << "   3. Configurar Dijkstra (apenas grafos com peso)\n\n";
            
            cout << "🔍 ANÁLISES:\n";
            cout << "   4. Executar algoritmos de busca\n";
            cout << "   5. Analisar estatísticas do grafo\n";
            cout << "   6. Calcular distâncias (grafos sem peso)\n\n";
            
            cout << "   0. Sair\n\n";
            cout << "👉 Digite sua opção: ";
            
            cin >> opcao;
            
            switch (opcao) {
                case 1:
                    selecionarGrafo();
                    break;
                case 2:
                    selecionarEstrutura();
                    break;
                case 3:
                    configurarDijkstra();
                    break;
                case 4:
                    executarBusca();
                    break;
                case 5:
                    analisarEstatisticas();
                    break;
                case 6:
                    analisarDistancias();
                    break;
                case 0:
                    limparTela();
                    cout << "👋 Obrigado por usar o Sistema de Análise de Grafos!\n";
                    cout << "🚀 Desenvolvido com ❤️  para análise eficiente de grafos.\n\n";
                    break;
                default:
                    cout << "❌ Opção inválida! Tente novamente.\n";
                    pausar();
                    break;
            }
            
        } while (opcao != 0);
    }
};

int main() {
    MenuInterativo menu;
    menu.executar();
    return 0;
}