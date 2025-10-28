#include "mapping.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

using std::ifstream;
using std::runtime_error;
using std::string;

namespace {
std::string toLower(const std::string& texto) {
    std::string resultado;
    resultado.reserve(texto.size());
    for (unsigned char ch : texto) {
        resultado.push_back(static_cast<char>(std::tolower(ch)));
    }
    return resultado;
}
} // namespace

Mapping::Mapping(const string& arquivoPesquisadores,
                 const string& arquivoGrafoComPeso) {
    carregarPesquisadores(arquivoPesquisadores);
    validarComGrafo(arquivoGrafoComPeso);
}

int Mapping::obterIdPorNome(const string& nome) const {
    const auto ids = obterIdsPorNome(nome);
    if (ids.size() == 1) {
        return ids.front();
    }
    throw runtime_error("Nome de pesquisador refere-se a múltiplos IDs: " + nome);
}

std::vector<int> Mapping::obterIdsPorNome(const string& nome) const {
    const string chave = normalizar(nome);
    auto it = nomeParaIds.find(chave);
    if (it == nomeParaIds.end()) {
        throw std::out_of_range("Nome de pesquisador não encontrado: " + nome);
    }
    return it->second;
}

const string& Mapping::obterNomePorId(int id) const {
    auto it = idParaNome.find(id);
    if (it == idParaNome.end()) {
        throw std::out_of_range("ID de pesquisador não encontrado: " + std::to_string(id));
    }
    return it->second;
}

bool Mapping::contemNome(const string& nome) const {
    const string chave = normalizar(nome);
    return nomeParaIds.find(chave) != nomeParaIds.end();
}

bool Mapping::contemId(int id) const {
    return idParaNome.find(id) != idParaNome.end();
}

std::size_t Mapping::totalPesquisadores() const noexcept {
    return idParaNome.size();
}

std::string Mapping::normalizar(const string& texto) {
    return toLower(trim(texto));
}

std::string Mapping::trim(const string& texto) {
    const auto primeiro = texto.find_first_not_of(" \t\r\n");
    if (primeiro == string::npos) {
        return "";
    }
    const auto ultimo = texto.find_last_not_of(" \t\r\n");
    return texto.substr(primeiro, ultimo - primeiro + 1);
}

void Mapping::carregarPesquisadores(const string& arquivoPesquisadores) {
    ifstream arquivo(arquivoPesquisadores);
    if (!arquivo.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo de pesquisadores: " + arquivoPesquisadores);
    }

    string linha;
    std::size_t numeroLinha = 0;
    while (std::getline(arquivo, linha)) {
        ++numeroLinha;
        if (linha.empty()) {
            continue;
        }

        const auto delimitador = linha.find(',');
        if (delimitador == string::npos) {
            throw runtime_error("Linha inválida no arquivo de pesquisadores na linha " + std::to_string(numeroLinha));
        }

        const string idStr = trim(linha.substr(0, delimitador));
        const string nomeOriginal = trim(linha.substr(delimitador + 1));

        if (idStr.empty() || nomeOriginal.empty()) {
            throw runtime_error("Linha incompleta no arquivo de pesquisadores na linha " + std::to_string(numeroLinha));
        }

        int id = 0;
        try {
            id = std::stoi(idStr);
        } catch (const std::exception&) {
            throw runtime_error("ID inválido na linha " + std::to_string(numeroLinha) + ": " + idStr);
        }

        if (id <= 0) {
            throw runtime_error("ID não pode ser menor ou igual a zero. Linha " + std::to_string(numeroLinha));
        }

        const string chave = normalizar(nomeOriginal);
        if (chave.empty()) {
            throw runtime_error("Nome inválido na linha " + std::to_string(numeroLinha));
        }

        auto& listaIds = nomeParaIds[chave];
        listaIds.push_back(id);

        if (!idParaNome.emplace(id, nomeOriginal).second) {
            throw runtime_error("ID duplicado encontrado: " + std::to_string(id) + " na linha " + std::to_string(numeroLinha));
        }
    }

    if (idParaNome.empty()) {
        throw runtime_error("Nenhum pesquisador encontrado no arquivo " + arquivoPesquisadores);
    }
}

void Mapping::validarComGrafo(const string& arquivoGrafoComPeso) const {
    ifstream arquivo(arquivoGrafoComPeso);
    if (!arquivo.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo de grafo: " + arquivoGrafoComPeso);
    }

    string linha;
    if (!std::getline(arquivo, linha)) {
        throw runtime_error("Arquivo de grafo vazio: " + arquivoGrafoComPeso);
    }

    std::istringstream stream(linha);
    std::size_t numVerticesGrafo = 0;
    if (!(stream >> numVerticesGrafo)) {
        throw runtime_error("Cabeçalho inválido no arquivo de grafo: " + arquivoGrafoComPeso);
    }

    if (numVerticesGrafo != idParaNome.size()) {
        throw runtime_error(
            "Inconsistência detectada entre o número de vértices do grafo (" +
            std::to_string(numVerticesGrafo) + ") e a quantidade de pesquisadores carregados (" +
            std::to_string(idParaNome.size()) + ").");
    }
}
