#include <iostream>
#include "../representacao_leitura/leitor_grafo.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "estatisticas.h"
#include "componentes.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <arquivo.txt>\n";
        cout << "Gera relatório completo de estatísticas do grafo\n";
        return 1;
    }

    try {
        // Lê o grafo
        DadosGrafo dados = LeitorGrafo::lerArquivo(argv[1]);
        
        cout << "RELATÓRIO DE ESTATÍSTICAS - " << argv[1] << "\n";
        cout << "=========================================\n\n";
        
        // Análise com lista
        {
            ListaAdjacencia lista(dados);
            ComponentesConexas comp(dados.numVertices);
            comp.encontrarComponentes_Lista(lista);
            
            Estatisticas stats(dados.numVertices);
            stats.calcularEstatisticas_Lista(lista, dados.arestas.size());
            stats.adicionarInformacoesComponentes(comp);
            
            stats.imprimirEstatisticas();
            
            // Executa análises adicionais
            stats.executarAnaliseCompleta_Lista(lista);
            
            stats.salvarRelatorio("relatorio_completo.txt");
            
            cout << "💾 Relatório detalhado salvo em: relatorio_completo.txt\n";
        }
        
    } catch (exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}
