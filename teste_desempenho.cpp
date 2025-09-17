#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <fstream>
#include <sys/resource.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include "representacao_leitura/leitor_grafo.h"
#include "representacao_leitura/matriz_adjacencia.h"
#include "representacao_leitura/lista_adjacencia.h"
#include "busca/bfs.h"
#include "busca/dfs.h"
#include "analise/distancias.h"
#include "analise/componentes.h"

using namespace std;
using namespace chrono;

struct ResultadoTeste {
    string nomeGrafo;
    int numVertices;
    int numArestas;
    double memoriaMatriz;
    double memoriaLista;
    double tempoBFS_Matriz;
    double tempoBFS_Lista;
    double tempoDFS_Matriz;
    double tempoDFS_Lista;
    int numComponentes;
    int diametro;
    bool ehConexo;
    int tamanhoMaiorComponente;
    int tamanhoMenorComponente;
    
    ResultadoTeste() : memoriaMatriz(0), memoriaLista(0), tempoBFS_Matriz(0), 
                       tempoBFS_Lista(0), tempoDFS_Matriz(0), tempoDFS_Lista(0),
                       numComponentes(0), diametro(-1), ehConexo(false),
                       tamanhoMaiorComponente(0), tamanhoMenorComponente(0) {}
};

class GeradorTabelas {
private:
    vector<ResultadoTeste> resultados;
    
public:
    void adicionarResultado(const ResultadoTeste& resultado) {
        resultados.push_back(resultado);
    }
    
    void gerarTabelaMemoria(ofstream& arquivo) {
        arquivo << "\n=== TABELA DE USO DE MEMÓRIA (MB) ===\n\n";
        arquivo << setw(15) << left << "Grafo" << " | ";
        arquivo << setw(10) << right << "Vértices" << " | ";
        arquivo << setw(10) << right << "Arestas" << " | ";
        arquivo << setw(12) << right << "Matriz(MB)" << " | ";
        arquivo << setw(12) << right << "Lista(MB)" << " | ";
        arquivo << setw(12) << right << "Dif(MB)" << " |\n";
        arquivo << string(85, '-') << "\n";
        
        for (const auto& r : resultados) {
            arquivo << setw(15) << left << r.nomeGrafo << " | ";
            arquivo << setw(10) << right << r.numVertices << " | ";
            arquivo << setw(10) << right << r.numArestas << " | ";
            arquivo << setw(12) << right << fixed << setprecision(2) << r.memoriaMatriz << " | ";
            arquivo << setw(12) << right << fixed << setprecision(2) << r.memoriaLista << " | ";
            arquivo << setw(12) << right << fixed << setprecision(2) << abs(r.memoriaMatriz - r.memoriaLista) << " |\n";
        }
        arquivo << "\n";
    }
    
    void gerarTabelaTemposBFS(ofstream& arquivo) {
        arquivo << "\n=== TABELA DE TEMPOS BFS (ms por busca) ===\n\n";
        arquivo << setw(15) << left << "Grafo" << " | ";
        arquivo << setw(10) << right << "Vértices" << " | ";
        arquivo << setw(10) << right << "Arestas" << " | ";
        arquivo << setw(12) << right << "Matriz(ms)" << " | ";
        arquivo << setw(12) << right << "Lista(ms)" << " | ";
        arquivo << setw(12) << right << "Dif(ms)" << " |\n";
        arquivo << string(85, '-') << "\n";
        
        for (const auto& r : resultados) {
            arquivo << setw(15) << left << r.nomeGrafo << " | ";
            arquivo << setw(10) << right << r.numVertices << " | ";
            arquivo << setw(10) << right << r.numArestas << " | ";
            arquivo << setw(12) << right << fixed << setprecision(3) << r.tempoBFS_Matriz << " | ";
            arquivo << setw(12) << right << fixed << setprecision(3) << r.tempoBFS_Lista << " | ";
            arquivo << setw(12) << right << fixed << setprecision(3) << abs(r.tempoBFS_Matriz - r.tempoBFS_Lista) << " |\n";
        }
        arquivo << "\n";
    }
    
    void gerarTabelaTemposDFS(ofstream& arquivo) {
        arquivo << "\n=== TABELA DE TEMPOS DFS (ms por busca) ===\n\n";
        arquivo << setw(15) << left << "Grafo" << " | ";
        arquivo << setw(10) << right << "Vértices" << " | ";
        arquivo << setw(10) << right << "Arestas" << " | ";
        arquivo << setw(12) << right << "Matriz(ms)" << " | ";
        arquivo << setw(12) << right << "Lista(ms)" << " | ";
        arquivo << setw(12) << right << "Dif(ms)" << " |\n";
        arquivo << string(85, '-') << "\n";
        
        for (const auto& r : resultados) {
            arquivo << setw(15) << left << r.nomeGrafo << " | ";
            arquivo << setw(10) << right << r.numVertices << " | ";
            arquivo << setw(10) << right << r.numArestas << " | ";
            arquivo << setw(12) << right << fixed << setprecision(3) << r.tempoDFS_Matriz << " | ";
            arquivo << setw(12) << right << fixed << setprecision(3) << r.tempoDFS_Lista << " | ";
            arquivo << setw(12) << right << fixed << setprecision(3) << abs(r.tempoDFS_Matriz - r.tempoDFS_Lista) << " |\n";
        }
        arquivo << "\n";
    }
    
    void gerarTabelaComponentes(ofstream& arquivo) {
        arquivo << "\n=== TABELA DE COMPONENTES CONEXAS ===\n\n";
        arquivo << setw(15) << left << "Grafo" << " | ";
        arquivo << setw(10) << right << "Vértices" << " | ";
        arquivo << setw(10) << right << "Conexo" << " | ";
        arquivo << setw(12) << right << "Componentes" << " | ";
        arquivo << setw(10) << right << "Maior" << " | ";
        arquivo << setw(10) << right << "Menor" << " |\n";
        arquivo << string(80, '-') << "\n";
        
        for (const auto& r : resultados) {
            arquivo << setw(15) << left << r.nomeGrafo << " | ";
            arquivo << setw(10) << right << r.numVertices << " | ";
            arquivo << setw(10) << right << (r.ehConexo ? "Sim" : "Não") << " | ";
            arquivo << setw(12) << right << r.numComponentes << " | ";
            arquivo << setw(10) << right << r.tamanhoMaiorComponente << " | ";
            arquivo << setw(10) << right << r.tamanhoMenorComponente << " |\n";
        }
        arquivo << "\n";
    }
    
    void gerarTabelaDiametro(ofstream& arquivo) {
        arquivo << "\n=== TABELA DE DIÂMETRO ===\n\n";
        arquivo << setw(15) << left << "Grafo" << " | ";
        arquivo << setw(10) << right << "Vértices" << " | ";
        arquivo << setw(10) << right << "Arestas" << " | ";
        arquivo << setw(12) << right << "Conexo" << " | ";
        arquivo << setw(12) << right << "Diâmetro" << " |\n";
        arquivo << string(65, '-') << "\n";
        
        for (const auto& r : resultados) {
            arquivo << setw(15) << left << r.nomeGrafo << " | ";
            arquivo << setw(10) << right << r.numVertices << " | ";
            arquivo << setw(10) << right << r.numArestas << " | ";
            arquivo << setw(12) << right << (r.ehConexo ? "Sim" : "Não") << " | ";
            if (r.ehConexo && r.diametro >= 0) {
                arquivo << setw(12) << right << r.diametro << " |\n";
            } else {
                arquivo << setw(12) << right << "∞" << " |\n";
            }
        }
        arquivo << "\n";
    }
    
    void gerarTabelaComparativa(ofstream& arquivo) {
        arquivo << "\n=== TABELA COMPARATIVA DE EFICIÊNCIA ===\n\n";
        arquivo << setw(15) << left << "Grafo" << " | ";
        arquivo << setw(15) << right << "Melhor Memória" << " | ";
        arquivo << setw(15) << right << "Melhor BFS" << " | ";
        arquivo << setw(15) << right << "Melhor DFS" << " | ";
        arquivo << setw(15) << right << "Recomendação" << " |\n";
        arquivo << string(90, '-') << "\n";
        
        for (const auto& r : resultados) {
            arquivo << setw(15) << left << r.nomeGrafo << " | ";
            
            // Melhor memória
            if (r.memoriaLista < r.memoriaMatriz) {
                arquivo << setw(15) << right << "Lista" << " | ";
            } else if (r.memoriaMatriz < r.memoriaLista) {
                arquivo << setw(15) << right << "Matriz" << " | ";
            } else {
                arquivo << setw(15) << right << "Empate" << " | ";
            }
            
            // Melhor BFS
            if (r.tempoBFS_Lista < r.tempoBFS_Matriz) {
                arquivo << setw(15) << right << "Lista" << " | ";
            } else if (r.tempoBFS_Matriz < r.tempoBFS_Lista) {
                arquivo << setw(15) << right << "Matriz" << " | ";
            } else {
                arquivo << setw(15) << right << "Empate" << " | ";
            }
            
            // Melhor DFS
            if (r.tempoDFS_Lista < r.tempoDFS_Matriz) {
                arquivo << setw(15) << right << "Lista" << " | ";
            } else if (r.tempoDFS_Matriz < r.tempoDFS_Lista) {
                arquivo << setw(15) << right << "Matriz" << " | ";
            } else {
                arquivo << setw(15) << right << "Empate" << " | ";
            }
            
            // Recomendação geral
            int pontosLista = 0, pontosMatriz = 0;
            if (r.memoriaLista < r.memoriaMatriz) pontosLista++;
            else if (r.memoriaMatriz < r.memoriaLista) pontosMatriz++;
            
            if (r.tempoBFS_Lista < r.tempoBFS_Matriz) pontosLista++;
            else if (r.tempoBFS_Matriz < r.tempoBFS_Lista) pontosMatriz++;
            
            if (r.tempoDFS_Lista < r.tempoDFS_Matriz) pontosLista++;
            else if (r.tempoDFS_Matriz < r.tempoDFS_Lista) pontosMatriz++;
            
            if (pontosLista > pontosMatriz) {
                arquivo << setw(15) << right << "Lista" << " |\n";
            } else if (pontosMatriz > pontosLista) {
                arquivo << setw(15) << right << "Matriz" << " |\n";
            } else {
                arquivo << setw(15) << right << "Depende uso" << " |\n";
            }
        }
        arquivo << "\n";
    }
    
    void gerarRelatorioCompleto(const string& nomeArquivo) {
        ofstream arquivo(nomeArquivo);
        
        arquivo << "================================================================\n";
        arquivo << "           RELATÓRIO CONSOLIDADO DE DESEMPENHO\n";
        arquivo << "           REPRESENTAÇÕES DE GRAFOS\n";
        arquivo << "================================================================\n";
        arquivo << "Data/Hora: " << __DATE__ << " " << __TIME__ << "\n";
        arquivo << "Número de grafos analisados: " << resultados.size() << "\n";
        arquivo << "================================================================\n";
        
        gerarTabelaMemoria(arquivo);
        gerarTabelaTemposBFS(arquivo);
        gerarTabelaTemposDFS(arquivo);
        gerarTabelaComponentes(arquivo);
        gerarTabelaDiametro(arquivo);
        gerarTabelaComparativa(arquivo);
        
        // Estatísticas gerais
        arquivo << "\n=== ESTATÍSTICAS GERAIS ===\n\n";
        
        int totalMatrizMelhor = 0, totalListaMelhor = 0, totalEmpates = 0;
        double somaMemoriaMatriz = 0, somaMemoriaLista = 0;
        double somaBFS_Matriz = 0, somaBFS_Lista = 0;
        double somaDFS_Matriz = 0, somaDFS_Lista = 0;
        
        for (const auto& r : resultados) {
            somaMemoriaMatriz += r.memoriaMatriz;
            somaMemoriaLista += r.memoriaLista;
            somaBFS_Matriz += r.tempoBFS_Matriz;
            somaBFS_Lista += r.tempoBFS_Lista;
            somaDFS_Matriz += r.tempoDFS_Matriz;
            somaDFS_Lista += r.tempoDFS_Lista;
            
            int pontosLista = 0, pontosMatriz = 0;
            if (r.memoriaLista < r.memoriaMatriz) pontosLista++;
            else if (r.memoriaMatriz < r.memoriaLista) pontosMatriz++;
            
            if (r.tempoBFS_Lista < r.tempoBFS_Matriz) pontosLista++;
            else if (r.tempoBFS_Matriz < r.tempoBFS_Lista) pontosMatriz++;
            
            if (r.tempoDFS_Lista < r.tempoDFS_Matriz) pontosLista++;
            else if (r.tempoDFS_Matriz < r.tempoDFS_Lista) pontosMatriz++;
            
            if (pontosLista > pontosMatriz) totalListaMelhor++;
            else if (pontosMatriz > pontosLista) totalMatrizMelhor++;
            else totalEmpates++;
        }
        
        arquivo << "Recomendação geral:\n";
        arquivo << "  Lista de adjacência melhor: " << totalListaMelhor << " grafos\n";
        arquivo << "  Matriz de adjacência melhor: " << totalMatrizMelhor << " grafos\n";
        arquivo << "  Empates/Depende do uso: " << totalEmpates << " grafos\n\n";
        
        if (resultados.size() > 0) {
            arquivo << "Médias gerais:\n";
            arquivo << "  Memória - Matriz: " << fixed << setprecision(2) 
                   << somaMemoriaMatriz / resultados.size() << " MB\n";
            arquivo << "  Memória - Lista: " << fixed << setprecision(2) 
                   << somaMemoriaLista / resultados.size() << " MB\n";
            arquivo << "  BFS - Matriz: " << fixed << setprecision(3) 
                   << somaBFS_Matriz / resultados.size() << " ms\n";
            arquivo << "  BFS - Lista: " << fixed << setprecision(3) 
                   << somaBFS_Lista / resultados.size() << " ms\n";
            arquivo << "  DFS - Matriz: " << fixed << setprecision(3) 
                   << somaDFS_Matriz / resultados.size() << " ms\n";
            arquivo << "  DFS - Lista: " << fixed << setprecision(3) 
                   << somaDFS_Lista / resultados.size() << " ms\n";
        }
        
        arquivo.close();
        cout << "Relatório consolidado salvo em: " << nomeArquivo << endl;
    }
};

class TesteDesempenho {
private:
    string nomeArquivo;
    DadosGrafo dados;
    bool modoSilencioso;
    
public:
    TesteDesempenho(const string& arquivo, bool silencioso = false) 
        : nomeArquivo(arquivo), modoSilencioso(silencioso) {
        // Carrega o grafo
        dados = LeitorGrafo::lerArquivo(arquivo);
        if (!modoSilencioso) {
            cout << "Grafo carregado: " << dados.numVertices << " vértices, " 
                 << dados.arestas.size() << " arestas" << endl;
        }
    }
    
    // Função para obter uso de memória em MB
    double obterUsoMemoria() {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        // rusage retorna em KB, convertemos para MB
        return usage.ru_maxrss / 1024.0;
    }
    
    // Teste de memória para matriz de adjacência
    double testarMemoriaMatriz() {
        if (!modoSilencioso) {
            cout << "Testando memória - Matriz de Adjacência..." << endl;
        }
        
        // Mede memória antes
        double memoriaAntes = obterUsoMemoria();
        
        // Cria matriz de adjacência
        MatrizAdjacencia* matriz = new MatrizAdjacencia(dados);
        
        // Força o uso completo da matriz (acessa todos os elementos)
        const auto& mat = matriz->getMatriz();
        volatile int soma = 0;
        for (int i = 0; i < dados.numVertices; i++) {
            for (int j = 0; j < dados.numVertices; j++) {
                soma += mat[i][j];
            }
        }
        
        // Mede memória depois
        double memoriaDepois = obterUsoMemoria();
        double memoriaUsada = memoriaDepois - memoriaAntes;
        
        delete matriz;
        return memoriaUsada;
    }
    
    // Teste de memória para lista de adjacência
    double testarMemoriaLista() {
        if (!modoSilencioso) {
            cout << "Testando memória - Lista de Adjacência..." << endl;
        }
        
        // Mede memória antes
        double memoriaAntes = obterUsoMemoria();
        
        // Cria lista de adjacência
        ListaAdjacencia* lista = new ListaAdjacencia(dados);
        
        // Força o uso completo da lista (acessa todos os elementos)
        const auto& lst = lista->getLista();
        volatile int soma = 0;
        for (int i = 0; i < dados.numVertices; i++) {
            for (int j : lst[i]) {
                soma += j;
            }
        }
        
        // Mede memória depois
        double memoriaDepois = obterUsoMemoria();
        double memoriaUsada = memoriaDepois - memoriaAntes;
        
        delete lista;
        return memoriaUsada;
    }
    
    // Teste de tempo para matriz de adjacência (BFS)
    double testarTempoBFS_Matriz() {
        if (!modoSilencioso) {
            cout << "Testando tempo BFS - Matriz de Adjacência..." << endl;
        }
        
        MatrizAdjacencia matriz(dados);
        
        // Gera 100 vértices aleatórios para teste
        vector<int> verticesInicio = gerarVerticesAleatorios(100);
        
        auto inicio = high_resolution_clock::now();
        
        for (int i = 0; i < 100; i++) {
            BFS bfs(dados.numVertices); // Cria instância nova para cada teste
            bfs.executarBFS_Matriz(matriz, verticesInicio[i]);
        }
        
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        
        double tempoTotal = duracao.count() / 1000.0; // em milissegundos
        double tempoMedio = tempoTotal / 100.0;
        
        return tempoMedio;
    }
    
    // Teste de tempo para lista de adjacência (BFS)
    double testarTempoBFS_Lista() {
        if (!modoSilencioso) {
            cout << "Testando tempo BFS - Lista de Adjacência..." << endl;
        }
        
        ListaAdjacencia lista(dados);
        
        // Gera 100 vértices aleatórios para teste (mesmos da matriz para comparação justa)
        vector<int> verticesInicio = gerarVerticesAleatorios(100);
        
        auto inicio = high_resolution_clock::now();
        
        for (int i = 0; i < 100; i++) {
            BFS bfs(dados.numVertices); // Cria instância nova para cada teste
            bfs.executarBFS_Lista(lista, verticesInicio[i]);
        }
        
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        
        double tempoTotal = duracao.count() / 1000.0; // em milissegundos
        double tempoMedio = tempoTotal / 100.0;
        
        return tempoMedio;
    }
    
    // Teste de tempo para matriz de adjacência (DFS)
    double testarTempoDFS_Matriz() {
        if (!modoSilencioso) {
            cout << "Testando tempo DFS - Matriz de Adjacência..." << endl;
        }
        
        MatrizAdjacencia matriz(dados);
        DFS dfs(dados.numVertices);
        
        // Gera 100 vértices aleatórios para teste
        vector<int> verticesInicio = gerarVerticesAleatorios(100);
        
        auto inicio = high_resolution_clock::now();
        
        for (int i = 0; i < 100; i++) {
            dfs.reset();
            dfs.executarDFS_Matriz(matriz, verticesInicio[i]);
        }
        
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        
        double tempoTotal = duracao.count() / 1000.0; // em milissegundos
        double tempoMedio = tempoTotal / 100.0;
        
        return tempoMedio;
    }
    
    // Teste de tempo para lista de adjacência (DFS)
    double testarTempoDFS_Lista() {
        if (!modoSilencioso) {
            cout << "Testando tempo DFS - Lista de Adjacência..." << endl;
        }
        
        ListaAdjacencia lista(dados);
        
        // Gera 100 vértices aleatórios para teste (mesmos da matriz para comparação justa)
        vector<int> verticesInicio = gerarVerticesAleatorios(100);
        
        auto inicio = high_resolution_clock::now();
        
        for (int i = 0; i < 100; i++) {
            DFS dfs(dados.numVertices); // Cria instância nova para cada teste
            dfs.executarDFS_Lista(lista, verticesInicio[i]);
        }
        
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<microseconds>(fim - inicio);
        
        double tempoTotal = duracao.count() / 1000.0; // em milissegundos
        double tempoMedio = tempoTotal / 100.0;
        
        return tempoMedio;
    }
    
    // Análise de componentes conexas - retorna número de componentes e tamanhos
    tuple<int, int, int> analisarComponentesConexas() {
        if (!modoSilencioso) {
            cout << "Analisando componentes conexas..." << endl;
        }
        
        MatrizAdjacencia matriz(dados);
        ComponentesConexas comp(dados.numVertices);
        comp.encontrarComponentes_Matriz(matriz);
        
        int numComp = comp.getNumComponentes();
        const auto& componentes = comp.getComponentes();
        
        int tamanhoMaior = 0, tamanhoMenor = dados.numVertices;
        
        if (numComp > 0) {
            tamanhoMaior = componentes[0].tamanho;
            tamanhoMenor = componentes[numComp-1].tamanho;
        }
        
        return make_tuple(numComp, tamanhoMaior, tamanhoMenor);
    }
    
    // Análise de diâmetro - retorna diâmetro e se é conexo
    pair<int, bool> analisarDiametro() {
        if (!modoSilencioso) {
            cout << "Analisando diâmetro..." << endl;
        }
        
        MatrizAdjacencia matriz(dados);
        Distancias dist(dados.numVertices);
        dist.calcularDistancias_Matriz(matriz);
        
        int diametro = dist.getDiametro();
        bool conexo = dist.isConexo();
        
        return make_pair(diametro, conexo);
    }
    
    // Gera vértices aleatórios para os testes
    vector<int> gerarVerticesAleatorios(int quantidade) {
        vector<int> vertices;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, dados.numVertices);
        
        for (int i = 0; i < quantidade; i++) {
            vertices.push_back(dis(gen));
        }
        
        return vertices;
    }
    
    // Executa todos os testes e retorna o resultado
    ResultadoTeste executarTodosTestes() {
        if (!modoSilencioso) {
            cout << "Executando testes para: " << nomeArquivo << endl;
        }
        
        ResultadoTeste resultado;
        
        // Extrai nome do arquivo sem path
        size_t pos = nomeArquivo.find_last_of("/\\");
        resultado.nomeGrafo = (pos != string::npos) ? nomeArquivo.substr(pos + 1) : nomeArquivo;
        if (resultado.nomeGrafo.length() > 15) {
            resultado.nomeGrafo = resultado.nomeGrafo.substr(0, 12) + "...";
        }
        
        resultado.numVertices = dados.numVertices;
        resultado.numArestas = dados.arestas.size();
        
        // Testes de memória
        resultado.memoriaMatriz = testarMemoriaMatriz();
        resultado.memoriaLista = testarMemoriaLista();
        
        // Testes de tempo BFS
        resultado.tempoBFS_Matriz = testarTempoBFS_Matriz();
        resultado.tempoBFS_Lista = testarTempoBFS_Lista();
        
        // Testes de tempo DFS
        resultado.tempoDFS_Matriz = testarTempoDFS_Matriz();
        resultado.tempoDFS_Lista = testarTempoDFS_Lista();
        
        // Análises específicas
        auto componentes = analisarComponentesConexas();
        resultado.numComponentes = get<0>(componentes);
        resultado.tamanhoMaiorComponente = get<1>(componentes);
        resultado.tamanhoMenorComponente = get<2>(componentes);
        
        auto diametroInfo = analisarDiametro();
        resultado.diametro = diametroInfo.first;
        resultado.ehConexo = diametroInfo.second;
        
        if (!modoSilencioso) {
            cout << "Testes concluídos para: " << resultado.nomeGrafo << endl;
        }
        
        return resultado;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <arquivo1> [arquivo2] [arquivo3] ..." << endl;
        cout << "Exemplo: " << argv[0] << " test_graph.txt grafo_medio.txt grafo_grande.txt" << endl;
        return 1;
    }
    
    GeradorTabelas gerador;
    
    cout << "=============================================" << endl;
    cout << "  ANÁLISE COMPARATIVA DE GRAFOS" << endl;
    cout << "=============================================" << endl;
    
    // Processa cada arquivo de grafo
    for (int i = 1; i < argc; i++) {
        string arquivoGrafo = argv[i];
        
        try {
            cout << "\nProcessando: " << arquivoGrafo << "..." << endl;
            TesteDesempenho teste(arquivoGrafo, true); // modo silencioso
            ResultadoTeste resultado = teste.executarTodosTestes();
            gerador.adicionarResultado(resultado);
            cout << "✓ Concluído: " << resultado.nomeGrafo << endl;
        } catch (const exception& e) {
            cerr << "✗ Erro ao processar " << arquivoGrafo << ": " << e.what() << endl;
        }
    }
    
    // Gera relatório consolidado
    cout << "\nGerando relatório consolidado..." << endl;
    gerador.gerarRelatorioCompleto("relatorio_consolidado.txt");
    
    cout << "\n=============================================" << endl;
    cout << "Análise concluída!" << endl;
    cout << "Verifique o arquivo 'relatorio_consolidado.txt'" << endl;
    cout << "=============================================" << endl;
    
    return 0;
}
