#ifndef MAPPING_H
#define MAPPING_H

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Responsável por mapear nomes de pesquisadores aos seus respectivos IDs
 *        definidos em arquivos de datasets.
 *
 * A classe lê o arquivo de pesquisadores, constrói uma relação bidirecional
 * entre os nomes e os identificadores e valida se o grafo com peso informado
 * contém a mesma quantidade de vértices declarada no arquivo de pesquisadores.
 */
class Mapping {
public:
    /**
     * @param arquivoPesquisadores Caminho para o arquivo CSV no formato "id,nome".
     * @param arquivoGrafoComPeso Caminho para o grafo com peso utilizado para validação.
     * @throws std::runtime_error Em caso de falhas de leitura, duplicidades ou inconsistências.
     */
    Mapping(const std::string& arquivoPesquisadores,
            const std::string& arquivoGrafoComPeso);

    /**
     * @brief Retorna o ID associado ao nome informado.
     * @throws std::out_of_range Caso o nome não exista no mapeamento.
     */
    int obterIdPorNome(const std::string& nome) const;

    /**
     * @brief Retorna todos os IDs associados ao nome informado.
     * @throws std::out_of_range Caso o nome não exista no mapeamento.
     */
    std::vector<int> obterIdsPorNome(const std::string& nome) const;

    /**
     * @brief Retorna o nome associado ao ID informado.
     * @throws std::out_of_range Caso o ID não exista no mapeamento.
     */
    const std::string& obterNomePorId(int id) const;

    /**
     * @return true se o nome existir no mapeamento.
     */
    bool contemNome(const std::string& nome) const;

    /**
     * @return true se o ID existir no mapeamento.
     */
    bool contemId(int id) const;

    /**
     * @return Número total de pesquisadores carregados.
     */
    std::size_t totalPesquisadores() const noexcept;

private:
    static std::string normalizar(const std::string& texto);
    static std::string trim(const std::string& texto);

    void carregarPesquisadores(const std::string& arquivoPesquisadores);
    void validarComGrafo(const std::string& arquivoGrafoComPeso) const;

    std::unordered_map<std::string, std::vector<int>> nomeParaIds; // chave normalizada -> ids
    std::unordered_map<int, std::string> idParaNome;            // id -> nome original
};

#endif
