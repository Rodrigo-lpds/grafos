#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <iomanip>
#include "../representacao_leitura/leitor_grafo.h"
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "../busca/bfs.h"
#include "../busca/dfs.h"

using namespace std;
using namespace chrono;

/**
 * Classe para benchmark de BFS e DFS comparando Matriz vs Lista de Adjacência
 * 
 * METODOLOGIA:
 * - Mede APENAS o tempo de execução puro dos algoritmos BFS e DFS
 * - Exclui tempo de leitura do grafo, criação das estruturas e I/O
 * - Usa high_resolution_clock para medição precisa em microssegundos
 * - Executa múltiplas iterações com vértices de origem aleatórios
 */
class BenchmarkBuscas {
private:
    int numTestes;
    vector<int> verticesOrigem;
    
    // Gera vértices aleatórios para teste
    void gerarVerticesAleatorios(int numVertices) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, numVertices);
        
        verticesOrigem.clear();
        for (int i = 0; i < numTestes; i++) {
            verticesOrigem.push_back(dis(gen));
        }
    }
    
    // Executa benchmark para matriz de adjacência
    double benchmarkMatriz(const MatrizAdjacencia& matriz, int numVertices) {
        BFS bfs(numVertices);
        double tempoTotal = 0.0;
        
        // Executa cada BFS individualmente e mede apenas o tempo do algoritmo
        for (int vertice : verticesOrigem) {
            // Marca o tempo IMEDIATAMENTE antes do algoritmo iniciar
            auto inicio = high_resolution_clock::now();
            
            // Executa apenas o algoritmo BFS (sem I/O)
            bfs.executarBFS_Matriz(matriz, vertice);
            
            // Marca o tempo IMEDIATAMENTE após o algoritmo terminar
            auto fim = high_resolution_clock::now();
            
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempoTotal += duracao.count() / 1000.0; // Acumula em milissegundos
        }
        
        return tempoTotal;
    }
    
    // Executa benchmark para lista de adjacência
    double benchmarkLista(const ListaAdjacencia& lista) {
        BFS bfs(lista.getNumVertices());
        double tempoTotal = 0.0;
        
        // Executa cada BFS individualmente e mede apenas o tempo do algoritmo
        for (int vertice : verticesOrigem) {
            // Marca o tempo IMEDIATAMENTE antes do algoritmo iniciar
            auto inicio = high_resolution_clock::now();
            
            // Executa apenas o algoritmo BFS (sem I/O)
            bfs.executarBFS_Lista(lista, vertice);
            
            // Marca o tempo IMEDIATAMENTE após o algoritmo terminar
            auto fim = high_resolution_clock::now();
            
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempoTotal += duracao.count() / 1000.0; // Acumula em milissegundos
        }
        
        return tempoTotal;
    }
    
    // Executa benchmark DFS para matriz de adjacência
    double benchmarkDFS_Matriz(const MatrizAdjacencia& matriz, int numVertices) {
        DFS dfs(numVertices);
        double tempoTotal = 0.0;
        
        // Executa cada DFS individualmente e mede apenas o tempo do algoritmo
        for (int vertice : verticesOrigem) {
            // Marca o tempo IMEDIATAMENTE antes do algoritmo iniciar
            auto inicio = high_resolution_clock::now();
            
            // Executa apenas o algoritmo DFS (sem I/O)
            dfs.executarDFS_Matriz(matriz, vertice);
            
            // Marca o tempo IMEDIATAMENTE após o algoritmo terminar
            auto fim = high_resolution_clock::now();
            
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempoTotal += duracao.count() / 1000.0; // Acumula em milissegundos
        }
        
        return tempoTotal;
    }
    
    // Executa benchmark DFS para lista de adjacência
    double benchmarkDFS_Lista(const ListaAdjacencia& lista) {
        DFS dfs(lista.getNumVertices());
        double tempoTotal = 0.0;
        
        // Executa cada DFS individualmente e mede apenas o tempo do algoritmo
        for (int vertice : verticesOrigem) {
            // Marca o tempo IMEDIATAMENTE antes do algoritmo iniciar
            auto inicio = high_resolution_clock::now();
            
            // Executa apenas o algoritmo DFS (sem I/O)
            dfs.executarDFS_Lista(lista, vertice);
            
            // Marca o tempo IMEDIATAMENTE após o algoritmo terminar
            auto fim = high_resolution_clock::now();
            
            auto duracao = duration_cast<microseconds>(fim - inicio);
            tempoTotal += duracao.count() / 1000.0; // Acumula em milissegundos
        }
        
        return tempoTotal;
    }
    
public:
    BenchmarkBuscas(int numTestes = 100) : numTestes(numTestes) {}
    
    // Método para executar benchmark apenas com matriz
    void executarBenchmarkMatriz(const string& arquivoGrafo) {
        cout << "=================================================\n";
        cout << "      BENCHMARK BFS & DFS - MATRIZ DE ADJACÊNCIA\n";
        cout << "=================================================\n\n";
        
        cout << "Carregando grafo: " << arquivoGrafo << "\n";
        
        // Carregar dados do grafo
        DadosGrafo dados = LeitorGrafo::lerArquivo(arquivoGrafo);
        
        cout << "Número de vértices: " << dados.numVertices << "\n";
        cout << "Número de arestas: " << dados.arestas.size() << "\n";
        cout << "Número de testes: " << numTestes << "\n\n";
        
        // Gerar vértices aleatórios para os testes
        gerarVerticesAleatorios(dados.numVertices);
        
        // Criar representação matriz
        MatrizAdjacencia matriz(dados);
        
        cout << "Executando benchmarks com Matriz de Adjacência...\n";
        cout << "IMPORTANTE: Medindo APENAS o tempo de execução dos algoritmos BFS e DFS.\n";
        cout << "Excluindo tempo de leitura do grafo, escrita e outras operações I/O.\n\n";
        
        // Benchmark BFS - Matriz de Adjacência
        cout << "Testando BFS com Matriz de Adjacência...\n";
        double tempoBFS_Matriz = benchmarkMatriz(matriz, dados.numVertices);
        double tempoMedioBFS_Matriz = tempoBFS_Matriz / numTestes;
        
        // Benchmark DFS - Matriz de Adjacência
        cout << "Testando DFS com Matriz de Adjacência...\n";
        double tempoDFS_Matriz = benchmarkDFS_Matriz(matriz, dados.numVertices);
        double tempoMedioDFS_Matriz = tempoDFS_Matriz / numTestes;
        
        // Exibir resultados
        cout << "\n=================================================\n";
        cout << "                   RESULTADOS                   \n";
        cout << "=================================================\n\n";
        
        cout << fixed << setprecision(4);
        
        cout << "BFS - MATRIZ DE ADJACÊNCIA:\n";
        cout << "  Tempo total: " << tempoBFS_Matriz << " ms\n";
        cout << "  Tempo médio por BFS: " << tempoMedioBFS_Matriz << " ms\n\n";
        
        cout << "DFS - MATRIZ DE ADJACÊNCIA:\n";
        cout << "  Tempo total: " << tempoDFS_Matriz << " ms\n";
        cout << "  Tempo médio por DFS: " << tempoMedioDFS_Matriz << " ms\n\n";
        
        // BFS vs DFS na Matriz
        cout << "COMPARAÇÕES:\n\n";
        cout << "BFS vs DFS (Matriz):\n";
        double razaoMatriz = tempoMedioBFS_Matriz / tempoMedioDFS_Matriz;
        if (razaoMatriz > 1.0) {
            cout << "  DFS é " << razaoMatriz << "x mais rápido que BFS\n";
        } else {
            cout << "  BFS é " << (1.0/razaoMatriz) << "x mais rápido que DFS\n";
        }
        
        cout << "\n=================================================\n";
    }
    
    // Método para executar benchmark apenas com lista
    void executarBenchmarkLista(const string& arquivoGrafo) {
        cout << "=================================================\n";
        cout << "      BENCHMARK BFS & DFS - LISTA DE ADJACÊNCIA \n";
        cout << "=================================================\n\n";
        
        cout << "Carregando grafo: " << arquivoGrafo << "\n";
        
        // Carregar dados do grafo
        DadosGrafo dados = LeitorGrafo::lerArquivo(arquivoGrafo);
        
        cout << "Número de vértices: " << dados.numVertices << "\n";
        cout << "Número de arestas: " << dados.arestas.size() << "\n";
        cout << "Número de testes: " << numTestes << "\n\n";
        
        // Gerar vértices aleatórios para os testes
        gerarVerticesAleatorios(dados.numVertices);
        
        // Criar representação lista
        ListaAdjacencia lista(dados);
        
        cout << "Executando benchmarks com Lista de Adjacência...\n";
        cout << "IMPORTANTE: Medindo APENAS o tempo de execução dos algoritmos BFS e DFS.\n";
        cout << "Excluindo tempo de leitura do grafo, escrita e outras operações I/O.\n\n";
        
        // Benchmark BFS - Lista de Adjacência
        cout << "Testando BFS com Lista de Adjacência...\n";
        double tempoBFS_Lista = benchmarkLista(lista);
        double tempoMedioBFS_Lista = tempoBFS_Lista / numTestes;
        
        // Benchmark DFS - Lista de Adjacência
        cout << "Testando DFS com Lista de Adjacência...\n";
        double tempoDFS_Lista = benchmarkDFS_Lista(lista);
        double tempoMedioDFS_Lista = tempoDFS_Lista / numTestes;
        
        // Exibir resultados
        cout << "\n=================================================\n";
        cout << "                   RESULTADOS                   \n";
        cout << "=================================================\n\n";
        
        cout << fixed << setprecision(4);
        
        cout << "BFS - LISTA DE ADJACÊNCIA:\n";
        cout << "  Tempo total: " << tempoBFS_Lista << " ms\n";
        cout << "  Tempo médio por BFS: " << tempoMedioBFS_Lista << " ms\n\n";
        
        cout << "DFS - LISTA DE ADJACÊNCIA:\n";
        cout << "  Tempo total: " << tempoDFS_Lista << " ms\n";
        cout << "  Tempo médio por DFS: " << tempoMedioDFS_Lista << " ms\n\n";
        
        // BFS vs DFS na Lista
        cout << "COMPARAÇÕES:\n\n";
        cout << "BFS vs DFS (Lista):\n";
        double razaoLista = tempoMedioBFS_Lista / tempoMedioDFS_Lista;
        if (razaoLista > 1.0) {
            cout << "  DFS é " << razaoLista << "x mais rápido que BFS\n";
        } else {
            cout << "  BFS é " << (1.0/razaoLista) << "x mais rápido que DFS\n";
        }
        
        cout << "\n=================================================\n";
    }
    
    void executarBenchmark(const string& arquivoGrafo) {
        cout << "=================================================\n";
        cout << "      BENCHMARK BFS & DFS - MATRIZ vs LISTA     \n";
        cout << "=================================================\n\n";
        
        cout << "Carregando grafo: " << arquivoGrafo << "\n";
        
        // Carregar dados do grafo
        DadosGrafo dados = LeitorGrafo::lerArquivo(arquivoGrafo);
        
        cout << "Número de vértices: " << dados.numVertices << "\n";
        cout << "Número de arestas: " << dados.arestas.size() << "\n";
        cout << "Número de testes: " << numTestes << "\n\n";
        
        // Gerar vértices aleatórios para os testes
        gerarVerticesAleatorios(dados.numVertices);
        
        // Criar representações
        MatrizAdjacencia matriz(dados);
        ListaAdjacencia lista(dados);
        
        cout << "Executando benchmarks...\n";
        cout << "IMPORTANTE: Medindo APENAS o tempo de execução dos algoritmos BFS e DFS.\n";
        cout << "Excluindo tempo de leitura do grafo, escrita e outras operações I/O.\n\n";
        
        // Verificar se deve executar benchmarks de matriz (muito lento para grafos grandes)
        bool executarMatriz = dados.numVertices < 50000;
        
        if (!executarMatriz) {
            cout << "AVISO: Grafo com " << dados.numVertices << " vértices (>= 50.000)\n";
            cout << "Pulando benchmarks de Matriz de Adjacência devido ao alto custo computacional.\n";
            cout << "Executando apenas benchmarks de Lista de Adjacência.\n\n";
        }
        
        // Inicializar variáveis para resultados
        double tempoBFS_Matriz = 0.0, tempoMedioBFS_Matriz = 0.0;
        double tempoDFS_Matriz = 0.0, tempoMedioDFS_Matriz = 0.0;
        
        // Benchmark BFS - Matriz de Adjacência (se aplicável)
        if (executarMatriz) {
            cout << "Testando BFS com Matriz de Adjacência...\n";
            tempoBFS_Matriz = benchmarkMatriz(matriz, dados.numVertices);
            tempoMedioBFS_Matriz = tempoBFS_Matriz / numTestes;
        }
        
        // Benchmark BFS - Lista de Adjacência
        cout << "Testando BFS com Lista de Adjacência...\n";
        double tempoBFS_Lista = benchmarkLista(lista);
        double tempoMedioBFS_Lista = tempoBFS_Lista / numTestes;
        
        // Benchmark DFS - Matriz de Adjacência (se aplicável)
        if (executarMatriz) {
            cout << "Testando DFS com Matriz de Adjacência...\n";
            tempoDFS_Matriz = benchmarkDFS_Matriz(matriz, dados.numVertices);
            tempoMedioDFS_Matriz = tempoDFS_Matriz / numTestes;
        }
        
        // Benchmark DFS - Lista de Adjacência
        cout << "Testando DFS com Lista de Adjacência...\n";
        double tempoDFS_Lista = benchmarkDFS_Lista(lista);
        double tempoMedioDFS_Lista = tempoDFS_Lista / numTestes;
        
        // Exibir resultados
        cout << "\n=================================================\n";
        cout << "                   RESULTADOS                   \n";
        cout << "=================================================\n\n";
        
        cout << fixed << setprecision(4);
        
        // Resultados de Matriz (se executados)
        if (executarMatriz) {
            cout << "BFS - MATRIZ DE ADJACÊNCIA:\n";
            cout << "  Tempo total: " << tempoBFS_Matriz << " ms\n";
            cout << "  Tempo médio por BFS: " << tempoMedioBFS_Matriz << " ms\n\n";
        } else {
            cout << "BFS - MATRIZ DE ADJACÊNCIA:\n";
            cout << "  NÃO EXECUTADO (grafo muito grande)\n\n";
        }
        
        cout << "BFS - LISTA DE ADJACÊNCIA:\n";
        cout << "  Tempo total: " << tempoBFS_Lista << " ms\n";
        cout << "  Tempo médio por BFS: " << tempoMedioBFS_Lista << " ms\n\n";
        
        // Resultados DFS de Matriz (se executados)
        if (executarMatriz) {
            cout << "DFS - MATRIZ DE ADJACÊNCIA:\n";
            cout << "  Tempo total: " << tempoDFS_Matriz << " ms\n";
            cout << "  Tempo médio por DFS: " << tempoMedioDFS_Matriz << " ms\n\n";
        } else {
            cout << "DFS - MATRIZ DE ADJACÊNCIA:\n";
            cout << "  NÃO EXECUTADO (grafo muito grande)\n\n";
        }
        
        cout << "DFS - LISTA DE ADJACÊNCIA:\n";
        cout << "  Tempo total: " << tempoDFS_Lista << " ms\n";
        cout << "  Tempo médio por DFS: " << tempoMedioDFS_Lista << " ms\n\n";
        
        // Comparações
        cout << "COMPARAÇÕES:\n\n";
        
        if (executarMatriz) {
            // BFS: Matriz vs Lista
            double razaoBFS = tempoMedioBFS_Matriz / tempoMedioBFS_Lista;
            cout << "BFS - Matriz vs Lista:\n";
            if (razaoBFS > 1.0) {
                cout << "  Lista é " << razaoBFS << "x mais rápida que Matriz\n";
            } else {
                cout << "  Matriz é " << (1.0/razaoBFS) << "x mais rápida que Lista\n";
            }
            
            // DFS: Matriz vs Lista
            double razaoDFS = tempoMedioDFS_Matriz / tempoMedioDFS_Lista;
            cout << "DFS - Matriz vs Lista:\n";
            if (razaoDFS > 1.0) {
                cout << "  Lista é " << razaoDFS << "x mais rápida que Matriz\n";
            } else {
                cout << "  Matriz é " << (1.0/razaoDFS) << "x mais rápida que Lista\n";
            }
            
            // BFS vs DFS na Matriz
            cout << "\nBFS vs DFS (Matriz):\n";
            double razaoMatriz = tempoMedioBFS_Matriz / tempoMedioDFS_Matriz;
            if (razaoMatriz > 1.0) {
                cout << "  DFS é " << razaoMatriz << "x mais rápido que BFS\n";
            } else {
                cout << "  BFS é " << (1.0/razaoMatriz) << "x mais rápido que DFS\n";
            }
        } else {
            cout << "Comparações Matriz vs Lista:\n";
            cout << "  NÃO DISPONÍVEIS (matriz não executada)\n\n";
        }
        
        // BFS vs DFS na Lista (sempre disponível)
        cout << "BFS vs DFS (Lista):\n";
        double razaoLista = tempoMedioBFS_Lista / tempoMedioDFS_Lista;
        if (razaoLista > 1.0) {
            cout << "  DFS é " << razaoLista << "x mais rápido que BFS\n";
        } else {
            cout << "  BFS é " << (1.0/razaoLista) << "x mais rápido que DFS\n";
        }
        
        cout << "\n=================================================\n";
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 4) {
        cout << "Uso: " << argv[0] << " <arquivo_do_grafo> [num_testes] [tipo_representacao]\n";
        cout << "Parâmetros:\n";
        cout << "  arquivo_do_grafo: Caminho para o arquivo do grafo\n";
        cout << "  num_testes: Número de execuções BFS (padrão: 100)\n";
        cout << "  tipo_representacao: 'matriz', 'lista' ou 'ambos' (padrão: 'ambos')\n\n";
        cout << "Exemplos:\n";
        cout << "  " << argv[0] << " grafos/grafo_3.txt\n";
        cout << "  " << argv[0] << " grafos/grafo_3.txt 100\n";
        cout << "  " << argv[0] << " grafos/grafo_3.txt 100 lista\n";
        cout << "  " << argv[0] << " grafos/grafo_3.txt 100 matriz\n";
        cout << "  " << argv[0] << " grafos/grafo_3.txt 100 ambos\n";
        return 1;
    }
    
    try {
        // Número padrão de testes
        int numTestes = 100;
        string tipoRepresentacao = "ambos";
        
        // Se foi fornecido um número de testes, usar esse valor
        if (argc >= 3) {
            numTestes = stoi(argv[2]);
            if (numTestes <= 0) {
                cerr << "Erro: Número de testes deve ser positivo\n";
                return 1;
            }
        }
        
        // Se foi fornecido o tipo de representação, usar esse valor
        if (argc == 4) {
            tipoRepresentacao = argv[3];
            if (tipoRepresentacao != "matriz" && tipoRepresentacao != "lista" && tipoRepresentacao != "ambos") {
                cerr << "Erro: Tipo de representação deve ser 'matriz', 'lista' ou 'ambos'\n";
                return 1;
            }
        }
        
        BenchmarkBuscas benchmark(numTestes);
        
        if (tipoRepresentacao == "matriz") {
            benchmark.executarBenchmarkMatriz(argv[1]);
        } else if (tipoRepresentacao == "lista") {
            benchmark.executarBenchmarkLista(argv[1]);
        } else {
            benchmark.executarBenchmark(argv[1]);
        }
        
    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}