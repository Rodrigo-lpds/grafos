# Makefile para projetos de grafos
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Diretórios
REPR_DIR = representacao_leitura
BUSCA_DIR = busca
ANALISE_DIR = analise

# Arquivos objeto comuns (com caminhos atualizados)
COMMON_OBJS = $(REPR_DIR)/leitor_grafo.o $(REPR_DIR)/matriz_adjacencia.o $(REPR_DIR)/lista_adjacencia.o

# Executáveis
all: conversor bfs_demo dfs_demo busca_demo distancias_demo diametro_demo componentes_demo estatisticas_demo teste_desempenho

# Conversor original
conversor: $(COMMON_OBJS) $(REPR_DIR)/conversor_grafo.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Demo do BFS
bfs_demo: $(COMMON_OBJS) $(BUSCA_DIR)/bfs.o $(BUSCA_DIR)/bfs_demo.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Demo do DFS
dfs_demo: $(COMMON_OBJS) $(BUSCA_DIR)/dfs.o $(BUSCA_DIR)/dfs_demo.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Demo unificado (BFS + DFS)
busca_demo: $(COMMON_OBJS) $(BUSCA_DIR)/bfs.o $(BUSCA_DIR)/dfs.o $(BUSCA_DIR)/busca_demo.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Demo de distâncias entre dois vértices
distancias_demo: $(COMMON_OBJS) $(BUSCA_DIR)/bfs.o $(ANALISE_DIR)/distancias.o $(ANALISE_DIR)/distancias_demo.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Demo de diâmetro
diametro_demo: $(COMMON_OBJS) $(BUSCA_DIR)/bfs.o $(ANALISE_DIR)/distancias.o $(ANALISE_DIR)/diametro_demo.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Demo de componentes conexas
componentes_demo: $(COMMON_OBJS) $(BUSCA_DIR)/dfs.o $(ANALISE_DIR)/componentes.o $(ANALISE_DIR)/componentes_demo.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Demo de estatísticas
estatisticas_demo: $(COMMON_OBJS) $(BUSCA_DIR)/bfs.o $(BUSCA_DIR)/dfs.o $(ANALISE_DIR)/estatisticas.o $(ANALISE_DIR)/componentes.o $(ANALISE_DIR)/distancias.o $(ANALISE_DIR)/estatisticas_demo.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Programa completo (removido - arquivo não existe)

# Teste de desempenho (comparação matriz vs lista)
teste_desempenho: $(COMMON_OBJS) $(BUSCA_DIR)/bfs.o $(BUSCA_DIR)/dfs.o $(ANALISE_DIR)/distancias.o $(ANALISE_DIR)/componentes.o teste_desempenho.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Teste de performance (removido - arquivo não existe)

# Teste de parada antecipada
teste_parada_antecipada: $(COMMON_OBJS) $(BUSCA_DIR)/bfs.o teste_parada_antecipada.o
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
	rm -f conversor bfs_demo dfs_demo busca_demo distancias_demo diametro_demo componentes_demo estatisticas_demo teste_desempenho
	rm -f $(BUSCA_DIR)/*.txt

# Teste rápido BFS
test_bfs: bfs_demo
	./bfs_demo grafos/grafo_1.txt ambos 1

# Teste rápido DFS
test_dfs: dfs_demo
	./dfs_demo grafos/grafo_1.txt ambos 1

# Teste comparativo
test_compare: busca_demo
	./busca_demo grafos/grafo_2.txt ambos matriz 1

# Teste distâncias
test_distancias: distancias_demo
	./distancias_demo grafos/grafo_1.txt ambos

# Teste componentes
test_componentes: componentes_demo
	./componentes_demo grafos/grafo_1.txt ambos

# Teste diâmetro
test_diametro: diametro_demo
	./diametro_demo grafos/grafo_1.txt ambos

# Teste estatísticas
test_estatisticas: estatisticas_demo
	./estatisticas_demo grafos/grafo_1.txt ambos

# Teste completo unificado (removido - arquivo não existe)

# Teste completo
test: bfs_demo dfs_demo distancias_demo
	@echo "=== Testando programas principais ==="
	@echo "Testando BFS..."
	./bfs_demo grafos/grafo_1.txt ambos 1
	@echo "Testando DFS..."
	./dfs_demo grafos/grafo_1.txt ambos 1

# Teste de desempenho
test_desempenho: teste_desempenho
	@echo "=== Executando teste de desempenho ==="
	@echo "Testando com grafos/grafo_1.txt..."
	./teste_desempenho grafos/grafo_1.txt

.PHONY: all clean test test_bfs test_dfs test_compare test_distancias test_componentes test_diametro test_estatisticas test_desempenho
