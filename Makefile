# Makefile para projetos de grafos
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Diretórios
REPR_DIR = representacao_leitura
BUSCA_DIR = busca
ANALISE_DIR = analise

# Arquivos objeto comuns (com caminhos atualizados)
COMMON_OBJS = $(REPR_DIR)/leitor_grafo.o $(REPR_DIR)/matriz_adjacencia.o $(REPR_DIR)/lista_adjacencia.o $(REPR_DIR)/lista_adjacencia_peso.o mapping.o

# Executáveis
all: menu_interativo

# Menu interativo principal
menu_interativo: $(COMMON_OBJS) $(BUSCA_DIR)/bfs.o $(BUSCA_DIR)/dfs.o $(BUSCA_DIR)/dijkstra.o $(ANALISE_DIR)/estatisticas.o $(ANALISE_DIR)/componentes.o $(ANALISE_DIR)/distancias.o menu_interativo.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Teste de Dijkstra isolado
teste_dijkstra: $(COMMON_OBJS) $(BUSCA_DIR)/dijkstra.o teste_dijkstra.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regras para arquivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regras específicas para arquivos em subdiretórios
$(REPR_DIR)/%.o: $(REPR_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUSCA_DIR)/%.o: $(BUSCA_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ANALISE_DIR)/%.o: $(ANALISE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f *.o $(REPR_DIR)/*.o $(BUSCA_DIR)/*.o $(ANALISE_DIR)/*.o
	rm -f menu_interativo teste_dijkstra
	rm -f $(BUSCA_DIR)/*.txt

# Teste do menu interativo
test_menu: menu_interativo
	./menu_interativo

.PHONY: all clean test_menu
