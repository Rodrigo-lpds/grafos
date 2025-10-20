#include <iostream>
#include <string>
#include <filesystem>
#include "../representacao_leitura/leitor_grafo.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "estatisticas.h"
#include "componentes.h"
using namespace std;
namespace fs = std::filesystem;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <arquivo.txt>\n";
        cout << "Gera relatório completo de estatísticas do grafo\n";
        return 1;
    }

    try {

        DadosGrafo dados = LeitorGrafo::lerArquivo(argv[1]);

        string arquivoEntrada = argv[1];
        fs::path pathEntrada(arquivoEntrada);
        string nomeBase = pathEntrada.stem().string();
        string nomeRelatorio = "relatorio_" + nomeBase + ".txt";

        cout << "RELATÓRIO DE ESTATÍSTICAS - " << argv[1] << "\n";
        cout << "=========================================\n\n";

        {
            ListaAdjacencia lista(dados);
            ComponentesConexas comp(dados.numVertices);
            comp.encontrarComponentes_Lista(lista);

            Estatisticas stats(dados.numVertices);
            stats.calcularEstatisticas_Lista(lista, dados.arestas.size());
            stats.adicionarInformacoesComponentes(comp);

            stats.imprimirEstatisticas();

            stats.executarAnaliseCompleta_Lista(lista);

            stats.salvarRelatorio(nomeRelatorio);

            cout << "💾 Relatório detalhado salvo em: " << nomeRelatorio << "\n";
        }

    } catch (exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}
