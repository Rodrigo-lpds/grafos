#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdint>
#include <unistd.h>
#include <sys/resource.h>
#include <filesystem>
#include "representacao_leitura/leitor_grafo.h"
#include "representacao_leitura/matriz_adjacencia.h"
#include "representacao_leitura/lista_adjacencia.h"

using namespace std;
namespace fs = std::filesystem;

// Função para extrair nome do grafo e criar nome do arquivo de saída
string criarNomeArquivoSaida(const string& caminhoGrafo) {
    // Extrai apenas o nome do arquivo sem o caminho
    fs::path caminho(caminhoGrafo);
    string nomeArquivo = caminho.stem(); // Remove extensão
    
    // Remove "grafo_" se presente e adiciona "comparacao_memoria_"
    if (nomeArquivo.find("grafo_") == 0) {
        nomeArquivo = nomeArquivo.substr(6); // Remove "grafo_"
    }
    
    return "comparacao_memoria_grafo_" + nomeArquivo + ".txt";
}

// Função para imprimir simultaneamente na tela e no arquivo
void imprimirDuplo(ofstream& arquivo, const string& texto) {
    cout << texto;
    arquivo << texto;
}

// Classe para medir uso de memória
class MedidorMemoria {
private:
    // Lê a memória RSS (Resident Set Size) do processo atual em KB
    static long lerMemoriaRSS() {
        ifstream file("/proc/self/status");
        string line;
        
        while (getline(file, line)) {
            if (line.substr(0, 6) == "VmRSS:") {
                istringstream iss(line);
                string dummy;
                long memoria;
                iss >> dummy >> memoria; // "VmRSS:" e o valor em KB
                return memoria;
            }
        }
        return -1; // Erro na leitura
    }

    // Lê a memória virtual do processo atual em KB
    static long lerMemoriaVirtual() {
        ifstream file("/proc/self/status");
        string line;
        
        while (getline(file, line)) {
            if (line.substr(0, 6) == "VmSize:") {
                istringstream iss(line);
                string dummy;
                long memoria;
                iss >> dummy >> memoria; // "VmSize:" e o valor em KB
                return memoria;
            }
        }
        return -1; // Erro na leitura
    }

public:
    struct InfoMemoria {
        long memoriaRSS_KB;    // Memória física utilizada
        long memoriaVirtual_KB; // Memória virtual utilizada
        
        double getMemoriaRSS_MB() const {
            return memoriaRSS_KB / 1024.0;
        }
        
        double getMemoriaVirtual_MB() const {
            return memoriaVirtual_KB / 1024.0;
        }
    };
    
    static InfoMemoria medirMemoria() {
        InfoMemoria info;
        info.memoriaRSS_KB = lerMemoriaRSS();
        info.memoriaVirtual_KB = lerMemoriaVirtual();
        return info;
    }
    
    static InfoMemoria calcularDiferenca(const InfoMemoria& antes, const InfoMemoria& depois) {
        InfoMemoria diff;
        diff.memoriaRSS_KB = depois.memoriaRSS_KB - antes.memoriaRSS_KB;
        diff.memoriaVirtual_KB = depois.memoriaVirtual_KB - antes.memoriaVirtual_KB;
        return diff;
    }
};

// Função para verificar se é praticável criar a matriz de adjacência
bool ehPraticavelMatriz(const DadosGrafo& dados) {
    int n = dados.numVertices;
    
    // Calcula memória necessária em bytes (n x n uint8_t - otimizado!)
    long long memoriaMatriz_bytes = (long long)n * n * sizeof(uint8_t);
    double memoriaMatriz_GB = memoriaMatriz_bytes / (1024.0 * 1024.0 * 1024.0);
    
    // Com uint8_t podemos rodar grafos 4x maiores!
    // Considera impraticável se necessitar mais que 8GB ou mais que 100.000 vértices
    return (memoriaMatriz_GB < 8.0 && n < 100000);
}

// Função para medir memória de Matriz de Adjacência
MedidorMemoria::InfoMemoria medirMemoriaMatriz(const DadosGrafo& dados) {
    auto antes = MedidorMemoria::medirMemoria();
    
    // Cria a matriz de adjacência
    MatrizAdjacencia* matriz = new MatrizAdjacencia(dados);
    
    auto depois = MedidorMemoria::medirMemoria();
    
    // Calcula diferença
    auto diferenca = MedidorMemoria::calcularDiferenca(antes, depois);
    
    // Limpa a memória
    delete matriz;
    
    return diferenca;
}

// Função para medir memória de Lista de Adjacência  
MedidorMemoria::InfoMemoria medirMemoriaLista(const DadosGrafo& dados) {
    auto antes = MedidorMemoria::medirMemoria();
    
    // Cria a lista de adjacência
    ListaAdjacencia* lista = new ListaAdjacencia(dados);
    
    auto depois = MedidorMemoria::medirMemoria();
    
    // Calcula diferença
    auto diferenca = MedidorMemoria::calcularDiferenca(antes, depois);
    
    // Limpa a memória
    delete lista;
    
    return diferenca;
}

// Função para calcular memória teórica
void calcularMemoriaTeorica(const DadosGrafo& dados, ofstream& saida, bool matrizPraticavel) {
    int n = dados.numVertices;
    int m = dados.arestas.size();
    
    // Memória teórica da matriz: n x n uint8_t (otimizado de int para uint8_t)
    long long memoriaMatriz_bytes = (long long)n * n * sizeof(uint8_t);
    double memoriaMatriz_MB = memoriaMatriz_bytes / (1024.0 * 1024.0);
    double memoriaMatriz_GB = memoriaMatriz_bytes / (1024.0 * 1024.0 * 1024.0);
    
    // Memória teórica da lista: 
    // - vector de n elementos (cada um é um vector<int>)
    // - cada aresta aparece 2 vezes (grafo não-direcionado)
    // - aproximadamente 2*m elementos int nas listas
    long memoriaLista_bytes = n * sizeof(vector<int>) + 2 * m * sizeof(int);
    double memoriaLista_MB = memoriaLista_bytes / (1024.0 * 1024.0);
    
    imprimirDuplo(saida, "\n=== ANÁLISE TEÓRICA ===\n");
    imprimirDuplo(saida, "Número de vértices: " + to_string(n) + "\n");
    imprimirDuplo(saida, "Número de arestas: " + to_string(m) + "\n");
    imprimirDuplo(saida, "Densidade do grafo: " + to_string((2.0 * m) / (n * (n - 1)) * 100) + "%\n");
    imprimirDuplo(saida, "\nMemória teórica:\n");
    
    if (matrizPraticavel) {
        imprimirDuplo(saida, "Matriz de Adjacência: " + to_string(memoriaMatriz_MB) + " MB\n");
        imprimirDuplo(saida, "Lista de Adjacência: " + to_string(memoriaLista_MB) + " MB\n");
        imprimirDuplo(saida, "Razão (Matriz/Lista): " + to_string(memoriaMatriz_MB / memoriaLista_MB) + "\n");
    } else {
        imprimirDuplo(saida, "Matriz de Adjacência: " + to_string(memoriaMatriz_GB) + " GB (IMPRATICÁVEL - não testada)\n");
        imprimirDuplo(saida, "Lista de Adjacência: " + to_string(memoriaLista_MB) + " MB\n");
        imprimirDuplo(saida, "NOTA: Matriz não foi testada devido ao tamanho excessivo\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <arquivo_grafo>\n";
        cout << "Exemplo: " << argv[0] << " grafos/grafo_1.txt\n";
        return 1;
    }

    string arquivoGrafo = argv[1];
    string arquivoSaida = criarNomeArquivoSaida(arquivoGrafo);
    
    cout << "=== COMPARAÇÃO DE USO DE MEMÓRIA ===\n";
    cout << "Arquivo: " << arquivoGrafo << "\n";
    cout << "Resultados serão salvos em: " << arquivoSaida << "\n";
    
    try {
        // Abre arquivo de saída
        ofstream saida(arquivoSaida);
        if (!saida) {
            cerr << "Erro ao criar arquivo de saída: " << arquivoSaida << endl;
            return 1;
        }
        
        // Carrega o grafo
        DadosGrafo dados = LeitorGrafo::lerArquivo(arquivoGrafo);
        
        // Cabeçalho do arquivo
        time_t agora = time(0);
        char* dataHora = ctime(&agora);
        saida << "=== COMPARAÇÃO DE USO DE MEMÓRIA ===\n";
        saida << "Arquivo: " << arquivoGrafo << "\n";
        saida << "Data/Hora: " << dataHora;
        saida << "============================================\n";
        
        // Verifica se é praticável testar a matriz
        bool matrizPraticavel = ehPraticavelMatriz(dados);
        
        imprimirDuplo(saida, "\n=== MEDIÇÕES PRÁTICAS ===\n");
        imprimirDuplo(saida, "Medindo uso de memória...\n");
        
        if (!matrizPraticavel) {
            imprimirDuplo(saida, "\nAVISO: Matriz de adjacência muito grande para ser testada!\n");
            imprimirDuplo(saida, "Número de vértices: " + to_string(dados.numVertices) + "\n");
            long long memoriaEstimadaBytes = (long long)dados.numVertices * dados.numVertices * sizeof(uint8_t);
            double memoriaEstimadaGB = memoriaEstimadaBytes / (1024.0 * 1024.0 * 1024.0);
            imprimirDuplo(saida, "Memória estimada para matriz (uint8_t): " + to_string(memoriaEstimadaGB) + " GB\n");
            imprimirDuplo(saida, "Testando apenas Lista de Adjacência...\n");
        }
        
        // Força garbage collection e estabiliza a memória
        usleep(100000); // 100ms
        
        MedidorMemoria::InfoMemoria memoriaMatriz;
        
        if (matrizPraticavel) {
            // Mede memória da matriz apenas se for praticável
            memoriaMatriz = medirMemoriaMatriz(dados);
            imprimirDuplo(saida, "\nMatriz de Adjacência:\n");
            imprimirDuplo(saida, "  Memória RSS: " + to_string(memoriaMatriz.getMemoriaRSS_MB()) + " MB\n");
            imprimirDuplo(saida, "  Memória Virtual: " + to_string(memoriaMatriz.getMemoriaVirtual_MB()) + " MB\n");
            
            // Pequena pausa para estabilizar
            usleep(100000);
        }
        
        // Mede memória da lista (sempre testada)
        auto memoriaLista = medirMemoriaLista(dados);
        imprimirDuplo(saida, "\nLista de Adjacência:\n");
        imprimirDuplo(saida, "  Memória RSS: " + to_string(memoriaLista.getMemoriaRSS_MB()) + " MB\n");
        imprimirDuplo(saida, "  Memória Virtual: " + to_string(memoriaLista.getMemoriaVirtual_MB()) + " MB\n");
        
        // Comparação (apenas se matriz foi testada)
        imprimirDuplo(saida, "\n=== COMPARAÇÃO ===\n");
        if (matrizPraticavel) {
            if (memoriaLista.memoriaRSS_KB > 0) {
                double razaoRSS = (double)memoriaMatriz.memoriaRSS_KB / memoriaLista.memoriaRSS_KB;
                imprimirDuplo(saida, "Razão de uso de memória RSS (Matriz/Lista): " + to_string(razaoRSS) + "\n");
            }
            
            if (memoriaLista.memoriaVirtual_KB > 0) {
                double razaoVirtual = (double)memoriaMatriz.memoriaVirtual_KB / memoriaLista.memoriaVirtual_KB;
                imprimirDuplo(saida, "Razão de uso de memória Virtual (Matriz/Lista): " + to_string(razaoVirtual) + "\n");
            }
            
            double diferencaRSS_MB = memoriaMatriz.getMemoriaRSS_MB() - memoriaLista.getMemoriaRSS_MB();
            imprimirDuplo(saida, "Diferença de memória RSS: " + to_string(diferencaRSS_MB) + " MB\n");
            
            if (diferencaRSS_MB > 0) {
                imprimirDuplo(saida, "A Matriz de Adjacência usa " + to_string(diferencaRSS_MB) + " MB a mais que a Lista\n");
            } else {
                imprimirDuplo(saida, "A Lista de Adjacência usa " + to_string(-diferencaRSS_MB) + " MB a mais que a Matriz\n");
            }
        } else {
            imprimirDuplo(saida, "Comparação não disponível - Matriz não foi testada devido ao tamanho\n");
            imprimirDuplo(saida, "Lista de Adjacência: " + to_string(memoriaLista.getMemoriaRSS_MB()) + " MB RSS\n");
        }
        
        // Análise teórica
        calcularMemoriaTeorica(dados, saida, matrizPraticavel);
        
        // Fecha arquivo e informa conclusão
        saida.close();
        cout << "\nResultados salvos com sucesso em: " << arquivoSaida << "\n";
        
    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}