#include <iostream>
#include "../representacao_leitura/leitor_grafo.h"
#include "../representacao_leitura/matriz_adjacencia.h"
#include "../representacao_leitura/lista_adjacencia.h"
#include "componentes.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Uso: " << argv[0] << " <arquivo.txt> <tipo>\n";
        cout << "Tipos disponíveis:\n";
        cout << "  matriz - Analisa componentes usando matriz de adjacência\n";
        cout << "  lista  - Analisa componentes usando lista de adjacência\n";
        cout << "  ambos  - Analisa usando ambas as representações\n";
        cout << "Opcionalmente, adicione dois vértices para verificar conectividade:\n";
        cout << "  " << argv[0] << " <arquivo.txt> <tipo> <vertice1> <vertice2>\n";
        cout << "Exemplos:\n";
        cout << "  " << argv[0] << " graph_example.txt matriz\n";
        cout << "  " << argv[0] << " graph_example.txt ambos 1 6\n";
        return 1;
    }

    string tipo = argv[2];
    int v1 = -1, v2 = -1;

    if (argc >= 5) {
        try {
            v1 = stoi(argv[3]);
            v2 = stoi(argv[4]);
        } catch (exception& e) {
            cerr << "Erro: Vértices devem ser números inteiros\n";
            return 1;
        }
    }

    try {

        DadosGrafo dados = LeitorGrafo::lerArquivo(argv[1]);
        cout << "Grafo lido: " << dados.numVertices << " vertices, "
             << dados.arestas.size() << " arestas\n\n";

        ComponentesConexas comp(dados.numVertices);

        if (tipo == "matriz" || tipo == "ambos") {
            cout << "===== COMPONENTES CONEXAS - MATRIZ =====\n";

            MatrizAdjacencia matriz(dados);

            comp.encontrarComponentes_Matriz(matriz);

            comp.imprimirComponentes();
            comp.imprimirEstatisticas();

            if (v1 != -1 && v2 != -1) {
                cout << "Consulta de Conectividade:\n";
                cout << "Vértices " << v1 << " e " << v2 << ": ";
                if (comp.estaoConectados(v1, v2)) {
                    cout << "✅ CONECTADOS (mesma componente "
                         << comp.getComponenteDoVertice(v1) << ")\n";
                } else {
                    cout << "❌ NÃO CONECTADOS (componentes "
                         << comp.getComponenteDoVertice(v1) << " e "
                         << comp.getComponenteDoVertice(v2) << ")\n";
                }
                cout << "\n";
            }

            comp.salvarResultado("componentes_matriz_resultado.txt");
            cout << "✓ Resultado das componentes (matriz) salvo em 'componentes_matriz_resultado.txt'\n\n";
        }

        if (tipo == "lista" || tipo == "ambos") {
            cout << "===== COMPONENTES CONEXAS - LISTA =====\n";

            ListaAdjacencia lista(dados);

            comp.encontrarComponentes_Lista(lista);

            comp.imprimirComponentes();
            comp.imprimirEstatisticas();

            if (v1 != -1 && v2 != -1) {
                cout << "Consulta de Conectividade:\n";
                cout << "Vértices " << v1 << " e " << v2 << ": ";
                if (comp.estaoConectados(v1, v2)) {
                    cout << "✅ CONECTADOS (mesma componente "
                         << comp.getComponenteDoVertice(v1) << ")\n";
                } else {
                    cout << "❌ NÃO CONECTADOS (componentes "
                         << comp.getComponenteDoVertice(v1) << " e "
                         << comp.getComponenteDoVertice(v2) << ")\n";
                }
                cout << "\n";
            }

            comp.salvarResultado("componentes_lista_resultado.txt");
            cout << "✓ Resultado das componentes (lista) salvo em 'componentes_lista_resultado.txt'\n\n";
        }

        if (tipo != "matriz" && tipo != "lista" && tipo != "ambos") {
            cout << "Tipo inválido. Use: matriz, lista ou ambos\n";
            return 1;
        }

        cout << "========== RESUMO FINAL ==========\n";
        cout << "📊 Número de componentes: " << comp.getNumComponentes() << "\n";

        const auto& componentes = comp.getComponentes();
        if (componentes.size() == 1) {
            cout << "🔗 Grafo é CONEXO\n";
        } else {
            cout << "🔗 Grafo é DESCONEXO\n";
            cout << "📈 Maior componente: " << componentes[0].tamanho << " vértices\n";
            cout << "📉 Menor componente: " << componentes.back().tamanho << " vértices\n";
        }

    } catch (exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}
